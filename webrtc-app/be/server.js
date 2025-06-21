import express from 'express';
import { createServer } from 'http';
import { Server } from 'socket.io';

const app = express();
const PORT = 8080;
const server = createServer(app);

const io = new Server(server, {
    cors: {
        origin: "*",
        methods: ["GET", "POST"]
    },
    transports: ['websocket', 'polling']
});

// Storage
const users = new Map(); // socketId -> { username, roomId, joinedAt }
const rooms = new Map(); // roomId -> Set of socketIds

const log = (stage, message, data = {}) => {
    console.log(`[${stage}] ${message}`, data);
};

// Utility functions
const getRoomUsers = (roomId) => {
    const userIds = rooms.get(roomId) || new Set();
    return Array.from(userIds).map(id => {
        const user = users.get(id);
        return user ? { id, username: user.username, joinedAt: user.joinedAt } : null;
    }).filter(Boolean);
};

const addUserToRoom = (socketId, username, roomId) => {
    users.set(socketId, { username, roomId, joinedAt: new Date().toISOString() });
    
    if (!rooms.has(roomId)) {
        rooms.set(roomId, new Set());
    }
    rooms.get(roomId).add(socketId);
    
    log('ROOM', `✅ Added user`, { username, socketId, roomId, total: rooms.get(roomId).size });
};

const removeUserFromRoom = (socketId) => {
    const user = users.get(socketId);
    if (!user) return null;
    
    const { username, roomId } = user;
    
    // Remove from room
    if (rooms.has(roomId)) {
        rooms.get(roomId).delete(socketId);
        if (rooms.get(roomId).size === 0) {
            rooms.delete(roomId);
            log('ROOM', `🗑️ Removed empty room`, { roomId });
        }
    }
    
    users.delete(socketId);
    log('ROOM', `👋 Removed user`, { username, socketId, roomId });
    
    return { username, roomId };
};

// Socket handlers
io.on('connection', (socket) => {
    log('SOCKET', `✅ Connected`, { id: socket.id });
    
    socket.on('join-room', ({ room, username }) => {
        try {
            if (!room?.trim() || !username?.trim()) {
                log('JOIN', `❌ Invalid data`, { room, username, socketId: socket.id });
                socket.emit('join-error', { message: 'Invalid room or username' });
                return;
            }
            
            const trimmedRoom = room.trim();
            const trimmedUsername = username.trim();
            
            log('JOIN', `🚪 Join request`, { username: trimmedUsername, room: trimmedRoom, socketId: socket.id });
            
            // Get existing users before adding new one
            const existingUsers = getRoomUsers(trimmedRoom);
            log('JOIN', `👥 Existing users`, { count: existingUsers.length, users: existingUsers.map(u => u.username) });
            
            // Add user to room
            socket.join(trimmedRoom);
            addUserToRoom(socket.id, trimmedUsername, trimmedRoom);
            
            // Send existing users to new joiner
            socket.emit('all-users', existingUsers);
            log('JOIN', `📤 Sent existing users`, { to: trimmedUsername, count: existingUsers.length });
            
            // Notify others about new user
            socket.to(trimmedRoom).emit('user-connected', { 
                id: socket.id, 
                username: trimmedUsername,
                joinedAt: new Date().toISOString()
            });
            
        } catch (error) {
            log('JOIN', `❌ Error`, { error: error.message, socketId: socket.id });
            socket.emit('join-error', { message: 'Failed to join room' });
        }
    });
    
    socket.on('webrtc-offer', ({ to, from, username, offer }) => {
        try {
            if (!to || !from || !username || !offer) {
                log('OFFER', `❌ Invalid data`, { to, from, username, hasOffer: !!offer });
                return;
            }
            
            if (from !== socket.id) {
                log('OFFER', `❌ ID mismatch`, { claimed: from, actual: socket.id });
                return;
            }
            
            const targetUser = users.get(to);
            if (!targetUser) {
                log('OFFER', `❌ Target not found`, { to });
                return;
            }
            
            log('OFFER', `📞 Relaying offer`, { 
                from: username, 
                to: targetUser.username, 
                fromId: from, 
                toId: to,
                offer: offer
            });
            
            io.to(to).emit('webrtc-offer', { from, username, offer });
            
        } catch (error) {
            log('OFFER', `❌ Error`, { error: error.message });
        }
    });
    
    socket.on('webrtc-answer', ({ to, from, answer }) => {
        try {
            if (!to || !from || !answer || from !== socket.id) {
                log('ANSWER', `❌ Invalid data`, { to, from, actualId: socket.id, hasAnswer: !!answer });
                return;
            }
            
            const targetUser = users.get(to);
            const senderUser = users.get(from);
            
            if (!targetUser) {
                log('ANSWER', `❌ Target not found`, { to });
                return;
            }
            
            log('ANSWER', `✅ Relaying answer`, { 
                from: senderUser?.username || 'Unknown', 
                to: targetUser.username,
                fromId: from,
                toId: to,
                answer: answer
            });
            
            io.to(to).emit('webrtc-answer', { from, answer });
            
        } catch (error) {
            log('ANSWER', `❌ Error`, { error: error.message });
        }
    });
    
    socket.on('webrtc-ice-candidate', ({ to, candidate }) => {
        try {
            if (!to || !candidate) {
                log('ICE', `❌ Invalid data`, { to, hasCandidate: !!candidate });
                return;
            }
            
            const targetUser = users.get(to);
            const senderUser = users.get(socket.id);
            
            if (!targetUser) {
                log('ICE', `❌ Target not found`, { to });
                return;
            }
            
            log('ICE', `🧊 Relaying candidate`, { 
                from: senderUser?.username || 'Unknown', 
                to: targetUser.username,
                fromId: socket.id,
                toId: to,
                candidate: candidate
            });
            
            io.to(to).emit('webrtc-ice-candidate', { from: socket.id, candidate });
            
        } catch (error) {
            log('ICE', `❌ Error`, { error: error.message });
        }
    });
    
    socket.on('leave-room', ({ room }) => {
        log('LEAVE', `🚪 Leave request`, { room, socketId: socket.id });
        handleDisconnection(socket.id, 'manual-leave');
    });
    
    socket.on('disconnect', (reason) => {
        log('DISCONNECT', `🔌 Disconnected`, { reason, socketId: socket.id });
        handleDisconnection(socket.id, reason);
    });
    
    socket.on('error', (error) => {
        log('SOCKET', `❌ Socket error`, { error: error.message, socketId: socket.id });
    });
    
    function handleDisconnection(socketId, reason) {
        try {
            const userInfo = removeUserFromRoom(socketId);
            
            if (userInfo) {
                const { username, roomId } = userInfo;
                
                // Notify others in room
                socket.to(roomId).emit('user-disconnected', socketId);
                socket.leave(roomId);
                
                log('DISCONNECT', `✅ Cleaned up`, { username, roomId, reason });
                
                // Log room stats
                const remaining = getRoomUsers(roomId);
                log('STATS', `📊 Room update`, { roomId, remaining: remaining.length });
            }
            
        } catch (error) {
            log('DISCONNECT', `❌ Cleanup error`, { error: error.message, socketId });
        }
    }
});

// Health endpoint
app.get('/health', (req, res) => {
    const stats = {
        status: 'healthy',
        timestamp: new Date().toISOString(),
        activeUsers: users.size,
        activeRooms: rooms.size,
        rooms: {}
    };
    
    rooms.forEach((participants, roomId) => {
        stats.rooms[roomId] = {
            participants: participants.size,
            users: getRoomUsers(roomId).map(u => ({ username: u.username, joinedAt: u.joinedAt }))
        };
    });
    
    res.json(stats);
});

app.get('/', (req, res) => {
    res.json({ 
        message: 'WebRTC Signaling Server',
        status: 'running',
        timestamp: new Date().toISOString()
    });
});

// Error handling
app.use((error, req, res, next) => {
    log('EXPRESS', `❌ Error`, { error: error.message });
    res.status(500).json({ error: 'Internal server error' });
});

// Graceful shutdown
const gracefulShutdown = () => {
    log('SERVER', `🛑 Shutting down`);
    
    io.emit('server-shutdown', { message: 'Server shutting down' });
    
    server.close(() => {
        log('SERVER', `✅ Server closed`);
        io.close(() => {
            log('SERVER', `✅ Socket.IO closed`);
            process.exit(0);
        });
    });
    
    setTimeout(() => {
        log('SERVER', `⚠️ Force exit`);
        process.exit(1);
    }, 30000);
};

process.on('SIGTERM', gracefulShutdown);
process.on('SIGINT', gracefulShutdown);

process.on('uncaughtException', (error) => {
    log('SERVER', `💥 Uncaught exception`, { error: error.message });
    process.exit(1);
});

process.on('unhandledRejection', (reason) => {
    log('SERVER', `💥 Unhandled rejection`, { reason });
    process.exit(1);
});

// Start server
server.listen(PORT, () => {
    log('SERVER', `🚀 Started`, { 
        port: PORT, 
        url: `http://localhost:${PORT}`,
        health: `http://localhost:${PORT}/health`,
        timestamp: new Date().toISOString()
    });
});

// Stats logging every 5 minutes
setInterval(() => {
    log('STATS', `📊 Server stats`, {
        users: users.size,
        rooms: rooms.size,
        roomDetails: rooms.size > 0 ? Object.fromEntries(
            Array.from(rooms.entries()).map(([roomId, participants]) => [
                roomId, 
                { participants: participants.size }
            ])
        ) : {}
    });
}, 5 * 60 * 1000);

import express from 'express'
import { createServer } from "http";
import { Server } from "socket.io";

const app = express()
const port = 8080
const rooms = {};

const httpServer = createServer(app);
const io = new Server(httpServer, { 
    cors: {
        origin: "*"
    }
});

io.on("connection", (socket) => {
   console.log("initial transport", socket.conn.transport.name); // prints "polling"

  socket.conn.once("upgrade", () => {
    // called when the transport is upgraded (i.e. from HTTP long-polling to WebSocket)
    console.log("upgraded transport", socket.conn.transport.name); // prints "websocket"
  });

    socket.on('join-room', ({username, room}) => {
        socket.join(room);
        socket.username = username;
        socket.room = room;

        if (!rooms[room]){
            rooms[room] = []
        }
        rooms[room].push(username);
        io.to(room).emit('update-user-list', rooms[room]);
    });

    socket.on('disconnect', () => {
        const { username, room } = socket;
        if (username && room && rooms[room]) {
            console.log(`${username} disconnected from ${room}`);
            // Remove the user from the specific room's array
            rooms[room] = rooms[room].filter(user => user !== username);
            // Broadcast the updated user list to that specific room
            io.to(room).emit('update-user-list', rooms[room]);
        }
    });
});

httpServer.listen(port, () => {
    console.log(`server running at http://localhost:${port}`);
});

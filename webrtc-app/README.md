# WebRTC Video Calling – Tech Exploration

This is a minimal WebRTC video calling project built using **Next.js** (frontend) and **Express.js + Socket.IO** (backend) as part of deeper technical exploration into real-time peer-to-peer systems. It uses **WebRTC APIs**, **custom signaling over WebSockets**, and **Ngrok** for testing over WAN.

---

## Structure
root/
├── be/ → Express backend + Socket.IO signaling
└── fe/ → Next.js frontend + WebRTC media logic

---

## Stack

- Frontend: React (Next.js), WebRTC
- Backend: Node.js (Express), Socket.IO
- STUN: `stun:stun.l.google.com:19302`
- Tunnel: Ngrok (for public signaling)

---

## Features

- Join via Room ID + Username
- 1-to-1 WebRTC connection
- Video/audio stream exchange
- ICE candidate exchange via WebSockets
- Toggle mic/camera live
- Dynamic UI with remote/local streams

---


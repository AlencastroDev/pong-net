# Pong Net

A multiplayer Pong game built in **C++** with a focus on real-time networking. This is not a game project — it's a networking project that happens to use Pong as its testbed.

The goal was to implement a complete low-level networking stack from scratch: raw BSD sockets over UDP, a custom binary protocol, and an authoritative client-server architecture — with no networking libraries.

Built as the final project for the *Computing for Engineering* course at the **University of Brasília (UnB)**, semester 2026.1.
---

## Architecture

```
   ┌──────────────┐         UDP          ┌───────────────┐         UDP          ┌──────────────┐
   │   Client A   │ ◄──────────────────► │    Server     │ ◄──────────────────► │   Client B   │
   │  (Raylib)    │                      │ (authoritative│                      │  (Raylib)    │
   │              │                      │  simulation)  │                      │              │
   └──────────────┘                      └───────────────┘                      └──────────────┘
                                          runs on any
                                          Linux machine
```

The server is the single source of truth for game state. Clients only send inputs and render the state they receive back. This design:

- Prevents cheating (clients don't decide anything about game state)
- Prevents desynchronization between players
- Matches the same architecture used by competitive titles like Counter-Strike and Valorant

The game logic is **shared** between client and server, ensuring both sides simulate deterministically from the same code path — a prerequisite for any advanced netcode.

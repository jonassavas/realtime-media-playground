# Realtime Media Playground

A lightweight C++20 project built to refresh and explore networking fundamentals in real-time media transport systems such as RTP/WebRTC.

The project models a simplified real-time streaming pipeline:

Sender → (Simulated Network Conditions) → Receiver

Using UDP sockets, the project simulates packet loss and out-of-order delivery while transmitting timestamped packets between a sender and receiver. The receiver uses sequence numbers and timestamps to detect packet loss, identify out-of-order packets, and estimate one-way latency.

---

## Features

### Sender
- UDP packet transmission using Winsock
- Sequence-numbered packets
- Timestamped packets for latency measurement
- Simulated network behavior:
  - Packet loss (~10%)
  - Packet reordering (delayed delivery via queue)
- Simple payload message per packet
- Basic transmission statistics

### Receiver
- UDP packet listener
- Packet loss detection using sequence numbers
- Out-of-order packet detection
- One-way latency estimation (sender → receiver)
- Aggregated runtime statistics:
  - packets received
  - packets lost
  - out-of-order packets
  - average latency

---

## Design Notes

- The system uses UDP intentionally to model real-time communication constraints.
- Sequence numbers are used to detect packet loss and ordering issues.
- Sender simulates network conditions (loss + out-of-order) to emulate unstable networks.
- Receiver acts as a stateful stream processor, tracking continuity and latency over time.
- Latency is estimated using sender timestamps (one-way delay approximation).



## Limitations

- No real serialization format (raw struct transmission)
- No clock synchronization between sender and receiver
- No retransmission or congestion control
- Simplified packet reordering model
- Windows-only (Winsock dependency)
- Purely educational project

---

## Build & Run Instructions

### Requirements
- Platform: Windows
- C++20 compiler (MSVC recommended on Windows)
- CMake 3.20+

### Build:

```bash
cmake -S . -B build
cmake --build build --config Release
```

### Run:

After building the project, run the executables in separate terminals. Instructions are shown from the root directory.

##### Start Receiver first:
```bash
.\build\Release\receiver.exe
```
##### Then start Sender:
```bash
.\build\Release\sender.exe
```

---

## Potential Future Improvements

- Implement jitter buffer on receiver side
- Add proper serialization (Protobuf / bit-level encoding)
- Introduce RTT-based latency measurement (ACK system)
- Add feedback channel (receiver → sender stats)
- Replace payload with real media frames (audio/video)
- Cross-platform networking abstraction (Linux support)
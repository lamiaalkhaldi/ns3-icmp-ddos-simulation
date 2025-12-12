# NS-3 ICMP Ping Flood DDoS Simulation

This project simulates an ICMP Ping Flood Distributed Denial of Service (DDoS) attack using the NS-3 network simulator.  
It was completed as part of a course project at the University of Portland and demonstrates how high-rate ICMP packets can overwhelm a server through a realistic network topology.

---

## Overview

The purpose of the simulation is to:

- Model a realistic ICMP Ping Flood DDoS attack  
- Observe the effect of heavy flooding on a server  
- Compare normal ICMP traffic vs traffic under attack  
- Understand congestion, queuing, and router bottlenecks  

The simulation includes **5 attackers**, **1 legitimate client**, **1 router**, and **1 target server**.

---

## Tools & Technologies

- **NS-3 (Network Simulator 3)**  
- **C++**  
- **NetAnim** (for visualization)  
- **Ubuntu / WSL2**  
- **Git & GitHub**  

---

## Network Topology

Below is a simplified ASCII diagram of the simulated network:
       Attacker 1
       Attacker 2
       Attacker 3
       Attacker 4
       Attacker 5
            \ 
             \
              \ 
               →→→  [ Router ]  →→→  [ Server ]
              /
    [ Client ]


- All attackers send high-rate ICMP echo requests to the server  
- The client sends normal ping traffic  
- The router forwards all traffic, becoming the main bottleneck  

---

## Simulation Behavior

- Client sends **2 pings/sec** (normal behavior)  
- Each attacker sends **1000 pings/sec** with large packet sizes  
- Attackers start at slightly staggered times for realism  
- Server becomes overloaded, demonstrating DDoS symptoms  

---

##  Repository Structure

ns3-icmp-ddos-simulation/
│
├── src/
│   └── icmp-ddos-simulation.cc        # Main simulation code
│
├── results/
│   ├── screenshots/                   # NetAnim screenshots
│   └── video/
│       └── icmp-ddos-demo.mp4         # Demo video
│
├── docs/
│   └── Term Project Technical Report.pdf
│
└── README.md

---

##  Demo Video

A short demonstration of the attack visualization is available here:

`results/video/icmp-ddos-demo.mp4`

---

##  How to Run

1. Install NS-3  
2. Clone the repository:





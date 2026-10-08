# 🚀 EC_Master_Lite - EtherCAT Diagnostic & Hardware Validation Tool
A lightweight graphical application for EtherCAT network diagnostics, slave device information monitoring, and automated hardware validation.
EC_Master_Lite is developed using C++/Qt and the [**SOEM (Simple Open EtherCAT Master)**](https://github.com/OpenEtherCATsociety/SOEM) library.

Compared with the full-featured EC_Master, this Lite version focuses on essential EtherCAT diagnostic and hardware verification functions, providing a simpler and more lightweight interface for development, testing, and production environments.

## 📖 Overview
EC_Master_Lite is designed to help engineers quickly inspect EtherCAT slave devices and verify the health of an EtherCAT network.

## ✨ Features
- Display essential information of each EtherCAT slave connected to the network.
- Monitor the current status and health of the EtherCAT network.
- EC_Master_Lite provides automated hardware checking for EtherCAT slave devices.

## 🏗️Architecture
The application is built around the following components:
```text
┌───────────────────────────────┐
│          Qt GUI               │
│   User Interface / Display    │
└───────────────┬───────────────┘
                │
┌───────────────▼───────────────┐
│      EtherCAT Diagnostic      │
│        & Validation Layer     │
└───────────────┬───────────────┘
                │
┌───────────────▼───────────────┐
│             SOEM              │
│   Simple Open EtherCAT Master │
└───────────────┬───────────────┘
                │
┌───────────────▼───────────────┐
│       EtherCAT Network        │
│                               │
│  Master ── Slave ── Slave ... │
└───────────────────────────────┘
```
## 💻 Supported Platform
Development Environment
- Qt
- CMake
- MSVC
- Git

## ⚙️ Build
```bash
git clone https://github.com/tuanthanhhh/EC-Master-Lite.git
cd EtherCAT_Master_GUI
mkdir build && cd build
cmake ..
cmake --build .
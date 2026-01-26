# STM32 – MCP2515 CAN Interface

CAN is a common industrial and automotive communication bus due to its robustness, real-time characteristics, and fault tolerance.  
This repository provides an educational implementation of interfacing an **STM32** with the **MCP2515 CAN controller** via **SPI**, intended for research and onboarding purposes.

## Target Platform

- **MCU**: STM32F411CEU6 (Blackpill)
- **CAN Controller**: MCP2515 (MCP2515 CAN Module)
- **Toolchain**: STM32CubeIDE (STM32 HAL)
- **Lab Validation Tools**:
  - Cheap logic analyzer + Saleae Logic (SPI decoding)
  - Oscilloscope (signal integrity validation)

## Features (Target)

- MCP2515 register-level driver (SPI based)
- Classic CAN 2.0B support (Standard 11-bit and Extended 29-bit frames)
- Up to 1 Mb/s CAN bitrate (depending on oscillator and bus setup)
- SPI interface (STM32 as Master, MCP2515 as Slave)
- Transmit / Receive support
- Polling-based reception
- Interrupt-based (INT pin) reception *(planned)*
- Lab test procedure and troubleshooting documentation

## Repository Structure

```text
.
├─ docs/        Documentation (CAN fundamentals + testing plan)
├─ hardware/    Wiring notes and hardware setup references
├─ lib/         MCP2515 driver library (HAL-style)
└─ examples/    STM32CubeIDE example projects (planned)
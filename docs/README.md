# Documentation

## Summary 

This documentation supports the firmware development process for interfacing **Controller Area Network (CAN)** on an **STM32** using an external CAN controller **MCP2515** over **SPI**.

The goal is to provide a practical and beginner-friendly guide for:
- understanding CAN bus fundamentals required for lab validation,
- developing an MCP2515 SPI driver from scratch,
- testing TX/RX communication using two nodes in the lab.


## Key Features and Notes

- Compliant with **Classic CAN 2.0** communication (via MCP2515 capabilities).
- MCP2515 breakout modules commonly include an onboard CAN transceiver such as:
  - **TJA1050**
  - **MCP2551**
  - (or other equivalents depending on the vendor)
- Low-cost hardware platform:
  - MCP2515 breakout module is typically affordable (≈ **< Rp80.000,00 / < $5**, depends on region and vendor)
- Useful for STM32 families that **do not provide an integrated CAN peripheral**.
- Simple SPI wiring and integration:
  - **SCK**
  - **MISO**
  - **MOSI**
  - **CS (Chip Select)**
  - **INT** (optional but recommended for interrupt-driven receive)
  - **VCC** (module-dependent: check 3.3V/5V requirements)
  - **GND**
- CAN bus termination support:
  - Many modules provide easy jumper-based termination
  - Standard termination is **120 Ω** (must be applied correctly on the bus)

> Important note: MCP2515 is a CAN **controller**, not a physical-layer transceiver.  
> A CAN transceiver is still required for CANH/CANL communication (often included on the module).


## Hardware
To validate CAN communication in the lab, at least **two nodes** are required.

Suggested setup:
- 2× STM32F411CEU6 Blackpill
- 2× MCP2515 modules (with CAN transceiver)
- CANH/CANL wiring
- Proper termination: **120 Ω at both ends**


# Library Documentation (`lib/`)

This folder contains the reusable driver library implementation for:

- **MCP2515** (Standalone CAN Controller with SPI interface)
- Intended host MCU: **STM32 (HAL-based projects)**

The library is designed to be **portable across STM32 toolchains** (STM32CubeIDE, Keil, IAR, etc.) as long as STM32 HAL is available and the user provides SPI/GPIO initialization in the application.



## Purpose

The main objective of this library is to provide a structured and educational implementation of an MCP2515 driver that:

1. Can be developed from scratch based on the MCP2515 datasheet.
2. Can be tested and validated in the lab using:
   - Logic analyzer (Saleae Logic) for SPI transaction verification
   - Oscilloscope for signal integrity checking
3. Can be used by newcomers with minimal integration effort.

---

## Library Design Philosophy

This driver is written in a style inspired by STM32 HAL conventions:

- Uses `HandleTypeDef` configuration structures
- Uses explicit status return types for error handling
- Separates register definitions from driver logic
- Builds features incrementally in small, testable milestones

This library focuses on **Classic CAN (CAN 2.0)** as supported by MCP2515.

---

## Folder Structure


```text
─ /
   ├─ Inc/
   │  ├─ mcp2515.h         Public API (HAL-style)
   │  └─ mcp2515_reg.h     MCP2515 SPI commands + register definitions
   └─ Src/
      └─ mcp2515.c         Driver implementation

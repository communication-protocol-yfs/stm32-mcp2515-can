# CAN Fundamentals (Classic CAN)

This document provides the minimum CAN knowledge required to successfully develop, use and test this repository.
**STM32F411 Blackpill + MCP2515 (SPI CAN Controller)**.


## Overview
Controller Area Network (CAN) is a robust, message-based communication bus widely used in automotive and industrial systems.
CAN is multi-master and supports arbitration to avoid collisions.

## Physical Layer (CANH / CANL)
CAN uses differential signaling on two lines:
- CANH
- CANL

Important notes for lab testing:
- MCP2515 is a CAN controller, not a transceiver.
- A CAN transceiver (e.g., TJA1050, MCP2551, MCP2562) is required to connect to CANH/CANL.
- Use linear bus wiring and keep stub lines short.

## Termination (120 Ω)
A CAN bus must be terminated correctly:
- 120 Ω at one end
- 120 Ω at the other end

For a properly terminated bus, measuring between CANH and CANL typically gives ~60 Ω (two 120 Ω in parallel).

> Warning: Many breakout modules provide jumper termination. Do not enable termination on every node.

## Bitrate
Both nodes must use the same CAN bitrate.

Common automotive bitrates:
- 125 kbps
- 250 kbps
- 500 kbps
- 1 Mbps

Bitrate depends on:
- MCP2515 oscillator frequency (commonly 8 MHz or 16 MHz)
- MCP2515 CNF1/CNF2/CNF3 bit timing configuration

## CAN Frames (Classic CAN)
### Standard vs Extended ID
- Standard ID: 11-bit identifier
- Extended ID: 29-bit identifier

### DLC and Data
- Classic CAN supports 0–8 bytes of data per frame
- DLC (Data Length Code) defines the payload length

## ACK (Acknowledgment)
In **Normal mode**, successful CAN transmission requires an ACK from another active node on the bus.
If there is no ACK, the transmitter may retry automatically and error counters can increase.

Practical note:
- For real bus validation, use at least 2 active nodes or a CAN analyzer or logic analyzer tool.

## Common Failure Causes
- Wrong bitrate configuration
- Missing termination resistors
- CANH/CANL swapped
- No CAN transceiver on the board/module
- Only one node active (no ACK)

## References
[1] Analog Devices, Inc., “Controller Area Network (CAN) Implementation Guide,” Application Note AN-1123. [Online]. Available: https://www.analog.com/media/en/technical-documentation/app-notes/an-1123.pdf. [Accessed: 23-Jan-2026].

[2] Texas Instruments, “Introduction to the Controller Area Network (CAN),” Application Report SLOA101B. [Online]. Available: https://www.ti.com/lit/an/sloa101b/sloa101b.pdf. [Accessed: 23-Jan-2026].

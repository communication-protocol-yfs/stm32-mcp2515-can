# MCP2515 SPI Instruction Transactions

This document describes the **SPI transaction format and behavior** for all MCP2515 instructions.
It is intended to clarify **CS framing**, **TX/RX behavior**, and **side effects** as defined in the [MCP2515 datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/MCP2515-Stand-Alone-CAN-Controller-with-SPI-20001801J.pdf).

All SPI transactions shown here assume:
- STM32 operates as SPI Master
- MCP2515 operates as SPI Slave
- Chip Select (CS) is **manually controlled**
- SPI Mode 0 (CPOL = 0, CPHA = 0)

This will be implemented in the [implementation code](/lib/MCP2515/src/mcp2515.c) to control the MCP2515 Controller as slave.

> [!IMPORTANT]  
> Notation used:
>   - **CS LOW** : Chip Select pulled LOW
>   - **CS HIGH** : Chip Select pulled HIGH
>   - **TX[]**      : Bytes sent on SI
>   - **RX[]**      : Bytes received on SO
>   - **DUMMY**     : Any value (e.g., 0x00) used to generate SPI clocks
>   - **X**         : Undefined / ignored data

## Instruction Table

| Instruction                                   | Instruction Format | Address Byte | RX Data | Sequential | Side Effects |
|-----------------------------------------------|:------------------:|--------------|---------|------------|--------------|
| [RESET](#reset-instruction)                   |      1100 0000     | X            | X       | X          | Enter CONFIG |
| [READ](#read-instruction)                     |      0000 0011     | Yes          | Yes     | Yes        | None         |
| [READ RX BUFFER](#read-rx-buffer-instruction) |      1001 0nm0     | X            | Yes     | Yes        | Clears RXnIF |
| [WRITE](#write-instruction)                   |      0000 0010     | Yes          | X       | Yes        | None         |
| [LOAD TX BUFFER](#load-tx-buffer-instruction) |      0100 0abc     | X            | X       | Yes        | None         |
| [RTS](#request-to-send-instruction)           |      1000 0nnn     | X            | X       | X          | Start TX     |
| [READ STATUS](#read-status-instruction)       |      1010 0000     | X            | Yes     | X          | None         |
| [RX STATUS](#rx-status-instruction)           |      1011 0000     | X            | Yes     | X          | None         |
| [BIT MODIFY](#bit-modify-instruction)         |      0000 0101     | Yes          | X       | X          | Modify bits  |

If there is RX Data return,
the SPI HAL Function use:
```c
HAL_SPI_TransmitReceive();
```
else, use:
```c
HAL_SPI_Transmit();
```


## RESET Instruction
- Reinitialize the internal registers
- Set the configuration mode.

**SPI Transaction**
```pseudo
CS LOW
TX: [ RESET ]
CS HIGH
```

> [!NOTE]
> - Single-byte instruction  
> - No RX data  
> - Equivalent to asserting the RESET pin  
> - Recommended during power-on initialization  

## READ Instruction

Reads data from the register beginning at selected address.

**Single Register Read**
```pseudo
CS LOW
TX: [ READ | ADDR | DUMMY ]
RX: [  X   |  X   | DATA ]
CS HIGH
```
**Sequential Register Read**
```pseudo
CS LOW
TX: [ READ | ADDR | DUMMY | DUMMY | DUMMY | ... ]
RX: [  X   |  X   |  D0   |  D1   |  D2   | ... ]
CS HIGH
```

>[!NOTE]    
>- Register address auto-increments   
>- RX data is clocked out during TX dummy bytes   
>- CS must remain LOW for the entire transaction

## READ RX BUFFER Instruction
When reading a receive buffer, reduces the overhead of a normal [READ](#read-instruction) command by placing the Address Pointer at one of four locations, as indicated by ‘n,m’.

**SPI Transaction**
```pseudo
CS LOW
TX: [ READ_RX_BUFFER_CMD | DUMMY | DUMMY | ... ]
RX: [        X           |  D0   |  D1   | ... ]
CS HIGH   → RXnIF is automatically cleared
```
>[!NOTE]        
> - The associated RX flag bit, RXnIF (CANINTF), will be cleared after bringing CS high.
> - READ_RX_BUFFER_CMD = 1001 0**nm**0  

| n | m |          Address Points to          | Start Address |
|---|---|:-----------------------------------:|:-------------:|
| 0 | 0 | Receive Buffer 0, Start at RXB0SIDH |      0x61     |
| 0 | 1 |  Receive Buffer 0, Start at RXB0D0  |      0x66     |
| 1 | 0 | Receive Buffer 1, Start at RXB1SIDH |      0x71     |
| 1 | 1 |  Receive Buffer 1, Start at RXB1D0  |      0x76     |

>[!WARNING]      
>- RX interrupt flag (RXnIF) is cleared automatically on CS HIGH
>- Releasing CS early may cause data loss

## WRITE Instruction
Writes data to the register beginning at the selected address.

**Single Register Write**
```pseudo
CS LOW
TX: [ WRITE | ADDR | DATA ]
CS HIGH
```
**Sequential Register Write**
```pseudo
CS LOW
TX: [ WRITE | ADDR | D0 | D1 | D2 | ... ]
CS HIGH
```

>[!NOTE]    
>- Data is committed on the rising edge of SCK for bit D0
>- Partial bytes are discarded if CS is released early
>- Address pointer auto-increments

## LOAD TX BUFFER Instruction
When loading a transmit buffer, reduces the overhead of a normal [WRITE](#write-instruction) command by placing the Address Pointer at one of six locations, as indicated by ‘a,b,c’.

**SPI Transaction**
```pseudo
CS LOW
TX: [ LOAD_TX_BUFFER_CMD | D0 | D1 | D2 | ... ]
CS HIGH
```
>[!NOTE]
>- Command selects TXB0, TXB1, or TXB2 represented by **a, b and c** respectively.
>- Does not initiate transmission

| a | b | c |          Address Points to          | Start Address |
|---|---|---|:-----------------------------------:|:-------------:|
| 0 | 0 | 0 | Transmit Buffer 0, Start at TXB0SIDH |      0x31     |
| 0 | 0 | 1 |  Transmit Buffer 0, Start at TXB0D0  |      0x36     |
| 0 | 1 | 0 | Transmit Buffer 1, Start at TXB1SIDH |      0x41     |
| 0 | 1 | 1 |  Transmit Buffer 1, Start at TXB1D0  |      0x46     |
| 1 | 0 | 0 |  Transmit Buffer 2, Start at TXB2SIDH|      0x51     |
| 1 | 0 | 1 |  Transmit Buffer 2, Start at TXB2D0  |      0x56     |


## REQUEST-TO-SEND Instruction
Instructs controller to begin message transmission sequence for any of the transmit buffers.

**SPI Transaction**
```pseudo
CS LOW
TX: [ RTS | TXBn_MASK ]
CS HIGH    
```
>[!NOTE]
>- Single-byte instruction
>- TXBn_MASK selects TXB0 / TXB1 / TXB2

## READ STATUS Instruction
Quick polling command that reads several status bits for transmit and receive functions.

**SPI Transaction**
```pseudo
CS LOW
TX: [ READ_STATUS | DUMMY ]
RX: [      X      | STATUS ]
CS HIGH
```
**Status**

| Data $n$ |   Status  |     Register     | Address |
|:--------:|:---------:|:----------------:|---------|
|     7    | **TX2IF** |  **CANINTF[4]**  | 0x2C[4] |
|     6    | **TXREQ** | **TXB2CNTRL[3]** | 0x50[3] |
|     5    | **TX1IF** |  **CANINTF[3]**  | 0x2C[3] |
|     4    | **TXREQ** | **TXB1CNTRL[3]** | 0x40[3] |
|     3    | **TX0IF** |  **CANINTF[2]**  | 0x2C[2] |
|     2    | **TXREQ** | **TXB0CNTRL[3]** | 0x30[3] |
|     1    | **RX1IF** |  **CANINTF[1]**  | 0x2C[1] |
|     0    | **RX0IF** |  **CANINTF[0]**  | 0x2C[0] |


## RX STATUS Instruction
Quick polling command that indicates filter match and message type (standard, extended and/or remote) of received message.

**SPI Transaction**
```pseudo
CS LOW
TX: [ RX_STATUS | DUMMY ]
RX: [     X     | STATUS ]
CS HIGH
```

**Status**

**Bit 5 is unused**

|7|6| Received Msg      |
|-|-|:-----------------:|
|0|0|No msg received    |
|0|1|Msg in RXB0        |
|1|0|Msg in RXB1        |
|1|1|Msg in both buffer*|

>[!NOTE] 
> - *Buffer 0 has higher priority; therefore, RXB0 status is reflected in bits[4:0].
> - RXnIF (CANINTF) bits are mapped to bits 7 and 6.

|4|3| Msg Type Received      |
|-|-|:-----------------:|
|0|0|Std. data frame    |
|0|1|Std. remote frame        |
|1|0|Extd. data frame        |
|1|1|Extd. remote frame|

>[!NOTE] 
> - Extd. ID or Std. ID determined by bit 4.
> - RTR or data frame determined by bit 3.

| 2 |   1  |     0     | Filter Match |
|:--------:|:---------:|:----------------:|:---------:|
|     0    | 0 |  0  | RXF0 |
|     0    | 0 | 1 | RXF1 |
|     0    | 1 |  0  | RXF2 |
|     0    | 1 | 1 | RXF3 |
|     1    | 0 |  0  | RXF4 |
|     1    | 0 | 1 | RXF5 |
|     1    | 1 |  0  | RXF0 (rollover to RXB1) |
|     1    | 1 |  1  | RXF1 (rollover to RXB1) |


## BIT MODIFY Instruction
Allows the user to set or clear individual bits in a particular register.

>[!NOTE]
>Not all registers can be bit modified with this command.

**SPI Transaction**
```pseudo
CS LOW
TX: [ BIT_MODIFY | ADDR | MASK | DATA ]
CS HIGH
```

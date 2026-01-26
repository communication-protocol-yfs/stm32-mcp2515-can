/**
  ******************************************************************************
  * @file           mcp2515_reg.h
  * @brief          MCP2515 register map and SPI command definitions
  ******************************************************************************
  * Notes:
  *  - This file contains ONLY definitions (no logic).
  *  - All addresses and bit masks are based on the MCP2515 datasheet.
  ******************************************************************************
  */

#ifndef MCP2515_REG_H
#define MCP2515_REG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* ============================================================================
 * MCP2515 SPI INSTRUCTION SET
 * ============================================================================
 * Reference: MCP2515 datasheet - SPI instructions
 */
#define MCP2515_CMD_RESET          (0xC0U)
#define MCP2515_CMD_READ           (0x03U)
#define MCP2515_CMD_WRITE          (0x02U)
#define MCP2515_CMD_BIT_MODIFY     (0x05U)
#define MCP2515_CMD_READ_STATUS    (0xA0U)
#define MCP2515_CMD_RX_STATUS      (0xB0U)

/* Load TX buffer instructions (base + modifiers) */
#define MCP2515_CMD_LOAD_TX_BUF    (0x40U)

/* Request-To-Send instructions (base + modifiers) */
#define MCP2515_CMD_RTS            (0x80U)

/* Read RX buffer instructions (base + modifiers) */
#define MCP2515_CMD_READ_RX_BUF    (0x90U)


/* ============================================================================
 * REGISTER ADDRESSES
 * ============================================================================
 * Reference: MCP2515 datasheet - Register Map
 */
#define MCP2515_REG_CANSTAT        (0x0EU)
#define MCP2515_REG_CANCTRL        (0x0FU)

#define MCP2515_REG_CNF3           (0x28U)
#define MCP2515_REG_CNF2           (0x29U)
#define MCP2515_REG_CNF1           (0x2AU)

#define MCP2515_REG_CANINTE        (0x2BU)
#define MCP2515_REG_CANINTF        (0x2CU)

#define MCP2515_REG_EFLG           (0x2DU)

#define MCP2515_REG_TEC            (0x1CU)
#define MCP2515_REG_REC            (0x1DU)


/* ============================================================================
 * CANCTRL / CANSTAT MODE CONTROL (REQOP / OPMOD)
 * ============================================================================
 * CANCTRL.REQOP[2:0] selects requested mode.
 * CANSTAT.OPMOD[2:0] indicates current mode.
 *
 * Both fields occupy bits [7:5].
 */
#define MCP2515_OPMODE_MASK               (0xE0U)  /* bits 7:5 */

#define MCP2515_MODE_NORMAL               (0x00U)  /* 000 << 5 */
#define MCP2515_MODE_SLEEP                (0x20U)  /* 001 << 5 */
#define MCP2515_MODE_LOOPBACK             (0x40U)  /* 010 << 5 */
#define MCP2515_MODE_LISTEN_ONLY          (0x60U)  /* 011 << 5 */
#define MCP2515_MODE_CONFIG               (0x80U)  /* 100 << 5 */


/* ============================================================================
 * CANINTE / CANINTF FLAGS (Interrupt Enable / Flag)
 * ============================================================================
 * Only define the bits you will use early. Expand later as required.
 */
#define MCP2515_CANINT_RX0IF               (1U << 0)
#define MCP2515_CANINT_RX1IF               (1U << 1)
#define MCP2515_CANINT_TX0IF               (1U << 2)
#define MCP2515_CANINT_TX1IF               (1U << 3)
#define MCP2515_CANINT_TX2IF               (1U << 4)
#define MCP2515_CANINT_ERRIF               (1U << 5)
#define MCP2515_CANINT_WAKIF               (1U << 6)
#define MCP2515_CANINT_MERRF               (1U << 7)


/* ============================================================================
 * EFLG (Error Flag Register)
 * ============================================================================
 * Helpful for debugging bus errors.
 */
#define MCP2515_EFLG_EWARN                 (1U << 0)
#define MCP2515_EFLG_RXWAR                 (1U << 1)
#define MCP2515_EFLG_TXWAR                 (1U << 2)
#define MCP2515_EFLG_RXEP                  (1U << 3)
#define MCP2515_EFLG_TXEP                  (1U << 4)
#define MCP2515_EFLG_TXBO                  (1U << 5)
#define MCP2515_EFLG_RX0OVR                (1U << 6)
#define MCP2515_EFLG_RX1OVR                (1U << 7)


/* ============================================================================
 * TX / RX BUFFER REGISTERS (Starter set)
 * ============================================================================
 * These will be needed for TX and RX milestones.
 * Start with TXB0/RXB0; add others later if needed.
 */

/* --- TX Buffer 0 Control and ID registers --- */
#define MCP2515_REG_TXB0CTRL               (0x30U)
#define MCP2515_REG_TXB0SIDH               (0x31U)
#define MCP2515_REG_TXB0SIDL               (0x32U)
#define MCP2515_REG_TXB0EID8               (0x33U)
#define MCP2515_REG_TXB0EID0               (0x34U)
#define MCP2515_REG_TXB0DLC                (0x35U)
#define MCP2515_REG_TXB0D0                 (0x36U)  /* D0..D7 consecutive */

/* --- RX Buffer 0 Control and ID registers --- */
#define MCP2515_REG_RXB0CTRL               (0x60U)
#define MCP2515_REG_RXB0SIDH               (0x61U)
#define MCP2515_REG_RXB0SIDL               (0x62U)
#define MCP2515_REG_RXB0EID8               (0x63U)
#define MCP2515_REG_RXB0EID0               (0x64U)
#define MCP2515_REG_RXB0DLC                (0x65U)
#define MCP2515_REG_RXB0D0                 (0x66U)  /* D0..D7 consecutive */


/* ============================================================================
 * RTS (Request-To-Send) modifiers
 * ============================================================================
 * Used with MCP2515_CMD_RTS.
 */
#define MCP2515_RTS_TXB0                   (1U << 0)
#define MCP2515_RTS_TXB1                   (1U << 1)
#define MCP2515_RTS_TXB2                   (1U << 2)


/* ============================================================================
 * Utility macros
 * ============================================================================
 */
#define MCP2515_MIN(a,b)                   (( (a) < (b) ) ? (a) : (b))

#ifdef __cplusplus
}
#endif

#endif /* MCP2515_REG_H */

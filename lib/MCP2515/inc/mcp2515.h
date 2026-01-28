/**
  ******************************************************************************
  * @file           mcp2515.h
  * @brief          MCP2515 public API
  ******************************************************************************
  * @attention
  * This driver assumes SPI and GPIO are configured by the application
  * (e.g., using STM32CubeMX / STM32CubeIDE).
  ******************************************************************************
  */

#ifndef INC_MCP2515_H_
#define INC_MCP2515_H_

#ifdef __cplusplus
extern "C"  {
#endif

/* ============================================================================
 * Includes
 * ============================================================================
 */
#include <stdint.h>
#include <stdbool.h>

#include "mcp2515_reg.h"

typedef enum
{
    MCP2515_OK       = 0x00U,
    MCP2515_ERROR    = 0x01U,
    MCP2515_BUSY     = 0x02U,
    MCP2515_TIMEOUT  = 0x03U
} MCP2515_StatusTypeDef;

/* ============================================================================
 * MCP2515 operating modes
 * ============================================================================
 */
typedef enum
{
    MCP2515_MODE_NORMAL      = 0x00U,
    MCP2515_MODE_SLEEP       = 0x20U,
    MCP2515_MODE_LOOPBACK    = 0x40U,
    MCP2515_MODE_LISTEN_ONLY = 0x60U,
    MCP2515_MODE_CONFIG      = 0x80U
} MCP2515_ModeTypeDef;

/* ============================================================================
 * CAN frame structure (Classic CAN 2.0)
 * ============================================================================
 */
typedef struct
{
    uint32_t Id;        /*!< 11-bit or 29-bit identifier */
    uint8_t  DLC;       /*!< Data length (0..8) */
    uint8_t  Data[8];   /*!< Data bytes */
    uint8_t  IsExt;     /*!< 0 = Standard, 1 = Extended */
    uint8_t  IsRTR;     /*!< 0 = Data frame, 1 = Remote frame */
} MCP2515_CanFrameTypeDef;

/* ============================================================================
 * MCP2515 handle structure
 * ============================================================================
 */
typedef struct
{
    SPI_HandleTypeDef *hspi;   /*!< SPI handle (configured by user) */

    GPIO_TypeDef *CS_Port;     /*!< Chip Select GPIO port */
    uint16_t      CS_Pin;      /*!< Chip Select GPIO pin */

    uint32_t OscillatorHz;     /*!< MCP2515 oscillator frequency */
    uint32_t TimeoutMs;        /*!< SPI timeout */

    uint32_t ErrorCode;        /*!< Driver error tracking */
} MCP2515_HandleTypeDef;

/* ============================================================================
 * Public API
 * ============================================================================
 */

/* Initialization / control */
MCP2515_StatusTypeDef MCP2515_Init(MCP2515_HandleTypeDef *hmcp);
MCP2515_StatusTypeDef MCP2515_Reset(MCP2515_HandleTypeDef *hmcp);

MCP2515_StatusTypeDef MCP2515_SetMode(MCP2515_HandleTypeDef *hmcp,
                                     MCP2515_ModeTypeDef mode);
MCP2515_StatusTypeDef MCP2515_GetMode(MCP2515_HandleTypeDef *hmcp,
                                     MCP2515_ModeTypeDef *mode);

/* Register access */
MCP2515_StatusTypeDef MCP2515_ReadRegister(MCP2515_HandleTypeDef *hmcp,
                                          uint8_t address,
                                          uint8_t *value);

MCP2515_StatusTypeDef MCP2515_WriteRegister(MCP2515_HandleTypeDef *hmcp,
                                           uint8_t address,
                                           uint8_t value);

MCP2515_StatusTypeDef MCP2515_BitModify(MCP2515_HandleTypeDef *hmcp,
                                       uint8_t address,
                                       uint8_t mask,
                                       uint8_t data);

/* Status */
MCP2515_StatusTypeDef MCP2515_ReadStatus(MCP2515_HandleTypeDef *hmcp,
                                        uint8_t *status);

/* CAN operations (implemented later) */
MCP2515_StatusTypeDef MCP2515_Transmit(MCP2515_HandleTypeDef *hmcp,
                                       const MCP2515_CanFrameTypeDef *frame);

MCP2515_StatusTypeDef MCP2515_Receive(MCP2515_HandleTypeDef *hmcp,
                                      MCP2515_CanFrameTypeDef *frame);

#ifdef __cplusplus
}
#endif

#endif /* INC_MCP2515_H_ */
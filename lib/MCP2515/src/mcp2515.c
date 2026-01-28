/**
  ******************************************************************************
  * @file           mcp2515.c
  * @brief          MCP2515 implementation
  ******************************************************************************
  * @attention
  * This driver assumes SPI and GPIO are configured by the application
  * (e.g., using STM32CubeMX / STM32CubeIDE).
  ******************************************************************************
  */

#include "mcp2515.h"

/* ============================================================================
 * Internal helper functions
 * ============================================================================
 */

static inline void MCP2515_CS_Low(MCP2515_HandleTypeDef *hmcp){
    HAL_GPIO_WritePin(hmcp->CS_Port, hmcp->CS_Pin, GPIO_PIN_RESET);
}

static inline void MCP2515_CS_High(MCP2515_HandleTypeDef *hmcp){
    HAL_GPIO_WritePin(hmcp->CS_Port, hmcp->CS_Pin, GPIO_PIN_SET);
}

static MCP2515_StatusTypeDef MCP2515_SPI_Transmit(
    MCP2515_HandleTypeDef *hmcp,
    uint8_t *tx,
    uint8_t *rx,
    uint8_t len
){
    MCP2515_CS_Low(hmcp);
    HAL_StatusTypeDef ret =
        HAL_SPI_TransmitReceive(hmcp->hspi, tx, rx, len, hmcp->TimeoutMs);
    MCP2515_CS_High(hmcp);

    return (ret == HAL_OK)  ? MCP2515_OK : MCP2515_ERROR;
}

/* ============================================================================
 * Initialization and reset
 * ============================================================================
 */

MCP2515_StatusTypeDef MCP2515_Init(MCP2515_HandleTypeDef *hmcp){
    if (hmcp == NULL || hmcp->hspi == NULL) return MCP2515_ERROR;

    if (MCP2515_Reset(hmcp) != MCP2515_OK) return MCP2515_ERROR;

    if (MCP2515_SetMode(hmcp, MCP2515_MODE_CONFIG) != MCP2515_OK)
        return MCP2515_ERROR;

    return MCP2515_OK;
}

MCP2515_StatusTypeDef MCP2515_Reset(MCP2515_HandleTypeDef *hmcp){
    uint8_t cmd = MCP2515_CMD_RESET;

    MCP2515_CS_Low(hmcp);
    HAL_StatusTypeDef ret =
        HAL_SPI_Transmit(hmcp->hspi, &cmd, 1, hmcp->TimeoutMs);
    MCP2515_CS_High(hmcp);

    HAL_Delay(2);
    return (ret == HAL_OK) ? MCP2515_OK : MCP2515_ERROR;
}

MCP2515_StatusTypeDef MCP2515_ReadRegister(
    MCP2515_HandleTypeDef *hmcp,
    uint8_t address,
    uint8_t *value)
{
    if (value == NULL) return MCP2515_ERROR;

    uint8_t tx[3] = { MCP2515_CMD_READ, address, 0x00 };
    uint8_t rx[3] = { 0 };

    if (MCP2515_SPI_Transfer(hmcp, tx, rx, 3) != MCP2515_OK)
        return MCP2515_ERROR;

    *value = rx[2];
    return MCP2515_OK;
}

MCP2515_StatusTypeDef MCP2515_WriteRegister(
    MCP2515_HandleTypeDef *hmcp,
    uint8_t address,
    uint8_t value)
{
    uint8_t tx[3] = { MCP2515_CMD_WRITE, address, value };

    MCP2515_CS_Low(hmcp);
    HAL_StatusTypeDef ret =
        HAL_SPI_Transmit(hmcp->hspi, tx, 3, hmcp->TimeoutMs);
    MCP2515_CS_High(hmcp);

    return (ret == HAL_OK) ? MCP2515_OK : MCP2515_ERROR;
}

MCP2515_StatusTypeDef MCP2515_BitModify(
    MCP2515_HandleTypeDef *hmcp,
    uint8_t address,
    uint8_t mask,
    uint8_t data)
{
    uint8_t tx[4] = {
        MCP2515_CMD_BIT_MODIFY,
        address,
        mask,
        data
    };

    MCP2515_CS_Low(hmcp);
    HAL_StatusTypeDef ret =
        HAL_SPI_Transmit(hmcp->hspi, tx, 4, hmcp->TimeoutMs);
    MCP2515_CS_High(hmcp);

    return (ret == HAL_OK) ? MCP2515_OK : MCP2515_ERROR;
}

MCP2515_StatusTypeDef MCP2515_ReadStatus(
    MCP2515_HandleTypeDef *hmcp,
    uint8_t *status)
{
    if (status == NULL)
        return MCP2515_ERROR;

    uint8_t tx[2] = { MCP2515_CMD_READ_STATUS, 0x00 };
    uint8_t rx[2] = { 0 };

    if (MCP2515_SPI_Transfer(hmcp, tx, rx, 2) != MCP2515_OK)
        return MCP2515_ERROR;

    *status = rx[1];
    return MCP2515_OK;
}

/* ============================================================================
 * Mode control
 * ============================================================================
 */

MCP2515_StatusTypeDef MCP2515_SetMode(
    MCP2515_HandleTypeDef *hmcp,
    MCP2515_ModeTypeDef mode)
{
    /* Request mode via CANCTRL.REQOP */
    if (MCP2515_BitModify(
            hmcp,
            MCP2515_REG_CANCTRL,
            MCP2515_OPMODE_MASK,
            (uint8_t)mode) != MCP2515_OK)
    {
        return MCP2515_ERROR;
    }

    /* Verify mode via CANSTAT.OPMOD */
    uint8_t canstat = 0;
    if (MCP2515_ReadRegister(hmcp, MCP2515_REG_CANSTAT, &canstat) != MCP2515_OK)
        return MCP2515_ERROR;

    if ((canstat & MCP2515_OPMODE_MASK) != (uint8_t)mode)
        return MCP2515_ERROR;

    return MCP2515_OK;
}

MCP2515_StatusTypeDef MCP2515_GetMode(
    MCP2515_HandleTypeDef *hmcp,
    MCP2515_ModeTypeDef *mode)
{
    if (mode == NULL)
        return MCP2515_ERROR;

    uint8_t canstat = 0;
    if (MCP2515_ReadRegister(hmcp, MCP2515_REG_CANSTAT, &canstat) != MCP2515_OK)
        return MCP2515_ERROR;

    *mode = (MCP2515_ModeTypeDef)(canstat & MCP2515_OPMODE_MASK);
    return MCP2515_OK;
}
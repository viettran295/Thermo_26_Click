#include "thermo26.h"
#include "stdint.h"

/**
 * @brief Thermo 26 crc constants.
 * @details Specified constants for crc of Thermo 26 Click driver.
 */
#define THERMO26_POLYNOM                0x31
#define THERMO26_INIT_VALUE             0xFF


/** 
 * @brief Thermo 26 Dallas/Maxim calculation for CRC8 function.
 * @details This function calculates CRC8 with parameteres: 
 * @li @c  Width 8 bit
 * @li @c  Polynomial 0x31 (x8 + x5 + x4 +1)
 * @li @c  Initialization 0xFF
 * @li @c  Reflect input False
 * @li @c  Reflect output False
 * @li @c  Final XOR 0x00
 * @li @c  Example CRC(0xBEEF) = 0x92
 * @param[in] crc_source : 2 bytes array to calculate crc from.
 * @return Calculated CRC8 output.
 * @note None.
 */
static uint8_t thermo26_calculate_crc ( uint8_t *crc_source );

void thermo26_init ( thermo26_t *ctx, I2C_HandleTypeDef * i2c)
{
	ctx->i2c = i2c;
    ctx->slave_address = THERMO26_DEVICE_ADDRESS_0;
    ctx->rst_port = RST_PORT;
    ctx->rst_pin = RST_PIN;
}

HAL_StatusTypeDef thermo26_write_command( thermo26_t *ctx, uint16_t cmd )
{
    uint8_t data_buf[ 2 ] = { 0 };
    // Split 16-bit command in 2 bytes: --Command MSB--ACK--Command LSB
    data_buf[ 0 ] = ( uint8_t ) ( ( cmd >> 8 ) & 0xFF );
    data_buf[ 1 ] = ( uint8_t ) ( cmd & 0xFF );
	return HAL_I2C_Master_Transmit(ctx->i2c, ctx->slave_address, data_buf, 2, 100);
}

HAL_StatusTypeDef thermo26_read_command ( thermo26_t *ctx, uint16_t cmd, uint16_t *data_out )
{
    uint8_t data_buf[ 3 ] = { 0 };
    HAL_StatusTypeDef error_flag = thermo26_write_command ( ctx, cmd );
    error_flag |= HAL_I2C_Master_Receive(ctx->i2c, ctx->slave_address, data_buf, 3, 100);
    if ( ( HAL_OK == error_flag ) && ( data_buf[ 2 ] == thermo26_calculate_crc ( data_buf ) ) )
    {
        // Temperature MSB--Temperature LSB
        *data_out = ( ( uint16_t ) data_buf[ 0 ] << 8 ) | data_buf[ 1 ];
        return HAL_OK;
    }
    return HAL_ERROR;
}

void thermo26_reset_device ( thermo26_t *ctx )
{
    HAL_GPIO_WritePin(ctx->rst_port, ctx->rst_pin, GPIO_PIN_RESET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(ctx->rst_port, ctx->rst_pin, GPIO_PIN_SET);
    HAL_Delay(100);
}

void thermo26_set_rst_pin ( thermo26_t *ctx, uint8_t state )
{
    HAL_GPIO_WritePin(ctx->rst_port, ctx->rst_pin, GPIO_PIN_RESET);
}

HAL_StatusTypeDef thermo26_read_temperature ( thermo26_t *ctx, float *temperature )
{
    uint16_t raw_temp;
    // Start measurement function must be call before reading data
    HAL_StatusTypeDef error_flag = thermo26_start_measurement(ctx, THERMO26_CMD_PERIODIC_1_MPS_REP_HIGH);
    error_flag |= thermo26_read_command ( ctx, THERMO26_CMD_FETCH_DATA, &raw_temp );
    if ( error_flag == HAL_OK )
    {
        *temperature = THERMO26_ABS_MIN_TEMP +
                       ( THERMO26_ABS_MAX_TEMP - THERMO26_ABS_MIN_TEMP ) * raw_temp / THERMO26_DATA_RESOLUTION;
        return HAL_OK;
    }
    return HAL_ERROR;
}

HAL_StatusTypeDef thermo26_start_measurement ( thermo26_t *ctx, uint16_t cmd )
{
	HAL_StatusTypeDef error_flag = thermo26_write_command ( ctx, cmd );
    HAL_Delay(20);
    return error_flag;
}

HAL_StatusTypeDef thermo26_stop_measurement ( thermo26_t *ctx )
{
	HAL_StatusTypeDef error_flag = thermo26_write_command ( ctx, THERMO26_CMD_BREAK );
    HAL_Delay(1);
    return error_flag;
}

static uint8_t thermo26_calculate_crc ( uint8_t *crc_source )
{
    uint8_t crc = THERMO26_INIT_VALUE;
    for ( uint8_t byte_cnt = 0; byte_cnt < 2; byte_cnt++ ) 
    {
        crc ^= crc_source[ byte_cnt ];
        for ( uint8_t bit_cnt = 0; bit_cnt < 8; bit_cnt++ ) 
        {
            if ( ( crc & 0x80 ) != 0 )
            {
                crc = ( uint8_t ) ( ( crc << 1 ) ^ THERMO26_POLYNOM );
            }
            else
            {
                crc <<= 1;
            }
        }
    }
    return crc;
}
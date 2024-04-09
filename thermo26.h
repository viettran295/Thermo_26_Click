/*!
 * @file thermo26.h
 * @brief This file contains API for Thermo 26 Click Driver.
 */

#ifndef THERMO26_H
#define THERMO26_H

#ifdef __cplusplus
extern "C"{
#endif

#include "stm32g4xx_hal.h"
#include "stdint.h"

/**
 * @brief Thermo 26 commands list.
 * @details Specified commands list of Thermo 26 Click driver.
 */
// Single shot mode with enabled clock stretching
#define THERMO26_CMD_SINGLE_SHOT_CLK_STR_EN_REP_HIGH        0x2C06
#define THERMO26_CMD_SINGLE_SHOT_CLK_STR_EN_REP_MID         0x2C0D
#define THERMO26_CMD_SINGLE_SHOT_CLK_STR_EN_REP_LOW         0x2C10
// Single shot mode with enabled clock stretching
#define THERMO26_CMD_SINGLE_SHOT_CLK_STR_DIS_REP_HIGH       0x2400
#define THERMO26_CMD_SINGLE_SHOT_CLK_STR_DIS_REP_MID        0x240B
#define THERMO26_CMD_SINGLE_SHOT_CLK_STR_DIS_REP_LOW        0x2416
#define THERMO26_CMD_PERIODIC_0p5_MPS_REP_HIGH              0x2032
#define THERMO26_CMD_PERIODIC_0p5_MPS_REP_MID               0x2024
#define THERMO26_CMD_PERIODIC_0p5_MPS_REP_LOW               0x202F
#define THERMO26_CMD_PERIODIC_1_MPS_REP_HIGH                0x2130
#define THERMO26_CMD_PERIODIC_1_MPS_REP_MID                 0x2126
#define THERMO26_CMD_PERIODIC_1_MPS_REP_LOW                 0x212D
#define THERMO26_CMD_PERIODIC_2_MPS_REP_HIGH                0x2236
#define THERMO26_CMD_PERIODIC_2_MPS_REP_MID                 0x2220
#define THERMO26_CMD_PERIODIC_2_MPS_REP_LOW                 0x222B
#define THERMO26_CMD_PERIODIC_4_MPS_REP_HIGH                0x2334
#define THERMO26_CMD_PERIODIC_4_MPS_REP_MID                 0x2322
#define THERMO26_CMD_PERIODIC_4_MPS_REP_LOW                 0x2329
#define THERMO26_CMD_PERIODIC_10_MPS_REP_HIGH               0x2737
#define THERMO26_CMD_PERIODIC_10_MPS_REP_MID                0x2721
#define THERMO26_CMD_PERIODIC_10_MPS_REP_LOW                0x272A
#define THERMO26_CMD_FETCH_DATA                             0xE000
#define THERMO26_CMD_BREAK                                  0x3093
#define THERMO26_CMD_SOFT_RESET                             0x30A2
#define THERMO26_CMD_HEATER_ENABLE                          0x306D
#define THERMO26_CMD_HEATER_DISABLE                         0x3066
#define THERMO26_CMD_READ_STATUS                            0xF32D
#define THERMO26_CMD_CLEAR_STATUS                           0x3041
#define THERMO26_CMD_GET_SERIAL_NUM                         0x3780

/**
 * @brief Thermo 26 temperature calculation values.
 * @details Specified temperature calculation values of Thermo 26 Click driver.
 */
#define THERMO26_DATA_RESOLUTION                            65535.0f
#define THERMO26_ABS_MIN_TEMP                               ( -45.0f )
#define THERMO26_ABS_MAX_TEMP                               ( 130.0f )

/**
 * @brief Thermo 26 device address setting.
 * @details Specified setting for device slave address selection of
 * Thermo 26 Click driver.
 */
#define THERMO26_DEVICE_ADDRESS_0                           0x4A << 1 // 7-bit Address
#define THERMO26_DEVICE_ADDRESS_1                           0x4B << 1

#define RST_PORT 											GPIOC
#define RST_PIN 											GPIO_PIN_6

typedef struct
{
	// Output pins
	GPIO_TypeDef * rst_port;        /**< Reset port (Active low). */
	uint8_t rst_pin;				/**< Reset pin (Active low). */

	// I2C slave address
	uint8_t slave_address;      	/**< Device slave address (used for I2C driver). */

	I2C_HandleTypeDef * i2c;        /**< I2C driver object. */

} thermo26_t;


/**
 * @brief Thermo 26 initialization function.
 * @details This function initializes all necessary pins and peripherals used
 * for this click board.
 * @param[out] ctx : Click context object.
 * @param[in] i2c : I2C type.
 * @return None.
 */
void thermo26_init ( thermo26_t *ctx, I2C_HandleTypeDef * i2c );

/**
 * @brief Thermo 26 write command function.
 * @details This function writes a desired command by using I2C serial interface.
 * @param[in] ctx : Click context object.
 * @param[in] cmd : Command word to be written.
 * @return @li @c  HAL_OK - Success,
 *         @li @c HAL_ERROR - Error.
 */
HAL_StatusTypeDef thermo26_write_command ( thermo26_t *ctx, uint16_t cmd );

/**
 * @brief Thermo 26 read command function.
 * @details This function writes a desired command word and then reads its response word with CRC byte.
 * If the read CRC byte matches internal CRC calculation the response word will be stored in @b data_out.
 * @param[in] ctx : Click context object.
 * @param[in] cmd : Command word to be written.
 * @param[out] data_out : Read response word.
 * @return @li @c  HAL_OK - Success,
 *         @li @c HAL_ERROR - Error.
 */
HAL_StatusTypeDef thermo26_read_command ( thermo26_t *ctx, uint16_t cmd, uint16_t *data_out );

/**
 * @brief Thermo 26 reset device function.
 * @details This function resets the device the RST pin.
 * @param[in] ctx : Click context object.
 * @return None.
 */
void thermo26_reset_device ( thermo26_t *ctx );

/**
 * @brief Thermo 26 set rst pin function.
 * @details This function sets the RST pin logic state.
 * @param[in] ctx : Click context object.
 * @param[in] state : Pin logic state.
 * @return None.
 */
void thermo26_set_rst_pin ( thermo26_t *ctx, uint8_t state );

/**
 * @brief Thermo 26 read temperature function.
 * @details This function reads the temperature raw data measurements and converts it to degrees Celsius.
 * @param[in] ctx : Click context object.
 * @param[out] temperature : Temperature measurements in Celsius.
 * @return @li @c  HAL_OK - Success,
 *         @li @c HAL_ERROR - Error.
 */
HAL_StatusTypeDef thermo26_read_temperature ( thermo26_t *ctx, float *temperature );

/**
 * @brief Thermo 26 start measurement function.
 * @details This function starts the measurements by sending the specified command.
 * @param[in] ctx : Click context object.
 * @param[in] cmd : Single-shot or periodic measurements command, refer to the commands list macros.
 * @return @li @c  HAL_OK - Success,
 *         @li @c HAL_ERROR - Error.
 */
HAL_StatusTypeDef thermo26_start_measurement ( thermo26_t *ctx, uint16_t cmd );

/**
 * @brief Thermo 26 stop measurement function.
 * @details This function stops the measurements by sending the break command.
 * @param[in] ctx : Click context object.
 * @return @li @c  HAL_OK - Success,
 *         @li @c HAL_ERROR - Error.
 */
HAL_StatusTypeDef thermo26_stop_measurement ( thermo26_t *ctx );

#ifdef __cplusplus
}
#endif
#endif // THERMO26_H
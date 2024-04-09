# Thermo 26 Click
A compatible version of temperature sensor [Thermo 26 Click (STS31-DIS from Sensirion)](https://www.mikroe.com/thermo-26-click) for STM32.

## I2C configuration
- I2C speed mode: Fast Mode 
- I2C speed frequency: 400 KHz
- Clock no stretch mode: Disabled

## Example code 
- A suitable stm32 board should be included
```c
#include "stm32g4xx_hal.h"
```

- Declare and initialize object
```c
    /* USER CODE BEGIN 2 */
    thermo26_t thermo26;
    float temperature;
    HAL_StatusTypeDef error_flag
    thermo26_init(&thermo26, &hi2c3);
    /* USER CODE END 2 */
```

- Read temperature with 
```c
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  error_flag = thermo26_read_temperature(&thermo26, &temperature);
      if(error_flag != HAL_OK)
      {
        Error_Handle();
      }
	  HAL_Delay(1000);
  }
  /* USER CODE END 3 */
```
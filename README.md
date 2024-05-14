# BMM350 Magnetometer Sensor

## Overview

This library provides functions for interfacing with the BMM350 I2C 3-axis magnetometer used on the CY8CKIT-062S2-AI and SHIELD_XENSIV_A.

Datasheet: https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bmm350-ds001.pdf

GitHub: https://github.com/BoschSensortec/BMM350_SensorAPI

## Quick Start

Follow the steps below to create a simple application which outputs the magnetometer data from the sensor to the UART.

1. Create an empty ModusToolbox™ application.
2. Add this (sensor-orientation-bmm350) library to the application.
3. Add retarget-io library using the Library Manager.
4. Place following code in the main.c file.

```
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"
#include "mtb_bmm350.h"

mtb_bmm350_t magnetometer;

cyhal_i2c_t i2c;
cyhal_i2c_cfg_t i2c_cfg =
{
    .is_slave        = false,
    .address         = 0,
    .frequencyhal_hz = 400000
};

#define BMM_I2C_SDA (CYBSP_I2C_SDA)
#define BMM_I2C_SCL (CYBSP_I2C_SCL)

int main(void)
{
    cy_rslt_t result;
    mtb_bmm350_data_t data;

    /* Initialize the device and board peripherals */
    result = cybsp_init();
    if (CY_RSLT_SUCCESS != result)
    {
        CY_ASSERT(0);
    }

    __enable_irq();

    /* Initialize retarget-io to use the debug UART port */
    result = cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, CY_RETARGET_IO_BAUDRATE);
    if (CY_RSLT_SUCCESS != result)
    {
        CY_ASSERT(0);
    }

    /* Initialize I2C for interfacing with sensor */
    result = cyhal_i2c_init(&i2c, BMM_I2C_SDA, BMM_I2C_SCL, NULL);
    if (CY_RSLT_SUCCESS != result)
    {
        CY_ASSERT(0);
    }
    result = cyhal_i2c_configure(&i2c, &i2c_cfg);
    if (CY_RSLT_SUCCESS != result)
    {
        CY_ASSERT(0);
    }

    /* Initialize the sensor */
    result = mtb_bmm350_init_i2c(&magnetometer, &i2c, MTB_BMM350_ADDRESS_SEC);
    if (CY_RSLT_SUCCESS != result)
    {
        CY_ASSERT(0);
    }

    for (;;)
    {
        /* Get the magnetometer data and temperature, and print the results to the UART */
        result = mtb_bmm350_read(&magnetometer, &data);
        printf("\nMag: %f, %f, %f \r\n", data.sensor_data.x, data.sensor_data.y,
               data.sensor_data.z);
        printf("Temp: %f\r\n\n", data.sensor_data.temperature);
        cyhal_system_delay_ms(1000);
    }
}
```

5. Build the application and program the kit.

## More information

For more information, refer to the following documents:

* [API Reference Guide](./api_reference.md)
* [ModusToolbox Software Environment, Quick Start Guide, Documentation, and Videos](https://www.infineon.com/cms/en/design-support/tools/sdk/modustoolbox-software)
* [Infineon Technologies AG](https://www.infineon.com)

-----
© 2024, Cypress Semiconductor Corporation (an Infineon company) or an affiliate of Cypress Semiconductor Corporation.

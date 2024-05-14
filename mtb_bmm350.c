/******************************************************************************
 * \file mtb_bmm350.c
 *
 * \brief
 *     This file contains the functions for interacting with the
 *     BMM350 motion sensor.
 *
 ********************************************************************************
 * \copyright
 * Copyright 2024 Cypress Semiconductor Corporation
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************/

#include "mtb_bmm350.h"

/******************************************************************************
* Macros
******************************************************************************/
#define _I2C_TIMEOUT_MS            10 // 10 msec
#define _I2C_WRITE_BUFFER_LENGTH   32
#define _SOFT_RESET_DELAY_US       300

/******************************************************************************
* Global variables
******************************************************************************/
static cyhal_i2c_t* _bmm350_i2c = NULL;
uint8_t _dev_addr;

/*****************************************************************************
* Local function Prototypes
*****************************************************************************/

static BMM350_INTF_RET_TYPE _bmm350_i2c_read(uint8_t reg_addr, uint8_t* reg_data, uint32_t length,
                                             void* intf_ptr);

static BMM350_INTF_RET_TYPE _bmm350_i2c_write(uint8_t reg_addr, const uint8_t* reg_data,
                                              uint32_t length,
                                              void* intf_ptr);

static void _bmm350_delay_us(uint32_t period, void* intf_ptr);

/******************************************************************************
* _mtb_bmm350_pins_equal
******************************************************************************/
static inline bool _mtb_bmm350_pins_equal(_mtb_bmm350_interrupt_pin_t ref_pin, cyhal_gpio_t pin)
{
    return (ref_pin.pin == pin);
}


/******************************************************************************
* mtb_bmm350_init_i2c
******************************************************************************/
cy_rslt_t mtb_bmm350_init_i2c(mtb_bmm350_t* dev, cyhal_i2c_t* i2c_instance,
                              mtb_bmm350_address_t address)
{
    cy_rslt_t rslt;
    _dev_addr = address;
    uint8_t int_ctrl, err_reg_data = 0;
    struct bmm350_pmu_cmd_status_0 pmu_cmd_stat_0;
    _bmm350_i2c = i2c_instance;
    dev->sensor.intf_ptr = &_dev_addr;
    dev->sensor.read = _bmm350_i2c_read;
    dev->sensor.write = _bmm350_i2c_write;
    dev->sensor.delay_us = _bmm350_delay_us;

    rslt = bmm350_init(&(dev->sensor));
    if (BMM350_OK != rslt)
    {
        return rslt;
    }

    rslt = bmm350_get_pmu_cmd_status_0(&pmu_cmd_stat_0, &(dev->sensor));
    if (BMM350_OK != rslt)
    {
        return rslt;
    }

    /* Get error data */
    rslt = bmm350_get_regs(BMM350_REG_ERR_REG, &err_reg_data, 1, &(dev->sensor));
    if (BMM350_OK != rslt)
    {
        return rslt;
    }

    /* Configure interrupt settings */
    rslt = bmm350_configure_interrupt(BMM350_PULSED,
                                      BMM350_ACTIVE_HIGH,
                                      BMM350_INTR_PUSH_PULL,
                                      BMM350_UNMAP_FROM_PIN,
                                      &(dev->sensor));
    if (BMM350_OK != rslt)
    {
        return rslt;
    }
    /* Enable data ready interrupt */
    rslt = bmm350_enable_interrupt(BMM350_ENABLE_INTERRUPT, &(dev->sensor));
    if (BMM350_OK != rslt)
    {
        return rslt;
    }

    /* Get interrupt settings */
    rslt = bmm350_get_regs(BMM350_REG_INT_CTRL, &int_ctrl, 1, &(dev->sensor));
    if (BMM350_OK != rslt)
    {
        return rslt;
    }

    /* Set ODR and performance */
    rslt = bmm350_set_odr_performance(BMM350_DATA_RATE_25HZ, BMM350_AVERAGING_8, &(dev->sensor));
    if (BMM350_OK != rslt)
    {
        return rslt;
    }

    /* Enable all axis */
    rslt = bmm350_enable_axes(BMM350_X_EN, BMM350_Y_EN, BMM350_Z_EN, &(dev->sensor));
    if (rslt == BMM350_OK)
    {
        rslt = bmm350_set_powermode(BMM350_NORMAL_MODE, &(dev->sensor));
    }

    return (BMM350_OK == rslt)
            ? CY_RSLT_SUCCESS
            : rslt;
}


/******************************************************************************
* mtb_bmm350_read
******************************************************************************/
cy_rslt_t mtb_bmm350_read(mtb_bmm350_t* dev, mtb_bmm350_data_t* data)
{
    cy_rslt_t rslt;
    rslt = bmm350_get_compensated_mag_xyz_temp_data(&(data->sensor_data), &(dev->sensor));
    return (BMM350_OK == rslt)
            ? CY_RSLT_SUCCESS
            : rslt;
}


/******************************************************************************
* mtb_bmm350_selftest
******************************************************************************/
cy_rslt_t mtb_bmm350_selftest(mtb_bmm350_t* dev)
{
    struct bmm350_self_test out_data;
    cy_rslt_t rslt;
    rslt = bmm350_perform_self_test(&out_data, &(dev->sensor));
    cyhal_system_delay_us(_SOFT_RESET_DELAY_US); //per datasheet, delay needed after reset to reboot
    return (BMM350_OK == rslt)
            ? CY_RSLT_SUCCESS
            : rslt;
}


/******************************************************************************
* mtb_bmm350_free_pin
******************************************************************************/
void mtb_bmm350_free_pin(mtb_bmm350_t* dev)
{
    if (!_mtb_bmm350_pins_equal(dev->intpin, NC))
    {
        cyhal_gpio_free((dev->intpin).pin);
    }
    _bmm350_i2c = NULL;
}


/*****************************************************************************
* Function name: _bmm350_i2c_read
*****************************************************************************
* Summary:
* This internal function reads I2C function map to host MCU
*
* Parameters:
*  reg_addr    8bit register address of the sensor
*  reg_data    Data from the specified address
*  len         Length of the reg_data array
*  intf_ptr    Void pointer that can enable the linking of descriptors for interface related
*  callbacks
*
* Return:
*  int8_t     Status of execution
*
*****************************************************************************/
static BMM350_INTF_RET_TYPE _bmm350_i2c_read(uint8_t reg_addr, uint8_t* reg_data,
                                             uint32_t len, void* intf_ptr)
{
    uint8_t device_addr = *(uint8_t*)intf_ptr;

    return (BMM350_INTF_RET_TYPE)cyhal_i2c_master_mem_read(_bmm350_i2c, device_addr,
                                                           reg_addr, 1,
                                                           reg_data, (uint16_t)len,
                                                           _I2C_TIMEOUT_MS);
}


/*****************************************************************************
* Function name: _bmm350_i2c_write
*****************************************************************************
* Summary:
* This internal function writes I2C function map to host MCU
*
* Parameters:
*  reg_addr    8bit register address of the sensor
*  reg_data    Data from the specified address
*  len         Length of the reg_data array
*  intf_ptr    Void pointer that can enable the linking of descriptors for interface related
*  callbacks
*
* Return:
*  int8_t     Status of execution
*
*****************************************************************************/
static BMM350_INTF_RET_TYPE _bmm350_i2c_write(uint8_t reg_addr, const uint8_t* reg_data,
                                              uint32_t len, void* intf_ptr)
{
    uint8_t device_addr = *(uint8_t*)intf_ptr;

    return (BMM350_INTF_RET_TYPE)cyhal_i2c_master_mem_write(_bmm350_i2c, device_addr,
                                                            reg_addr, 1,
                                                            reg_data,
                                                            (uint16_t)len,
                                                            _I2C_TIMEOUT_MS);
}


/*****************************************************************************
* Function name: _bmm350_delay_us
*****************************************************************************
* Summary:
* This internal function maps delay function to host MCU
*
* Parameters:
*  us    The time period in microseconds
*  intf_ptr  Void pointer that can enable the linking of descriptors for interface related callbacks
*
* Return:
*  void
*
*****************************************************************************/
static void _bmm350_delay_us(uint32_t us, void* intf_ptr)
{
    (void)intf_ptr;

    cyhal_system_delay_us(us);
}


#if defined(__cplusplus)
}
#endif


/* [] END OF FILE */

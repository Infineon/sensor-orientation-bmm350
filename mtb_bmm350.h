/******************************************************************************
 * \file mtb_bmm350.h
 *
 * \brief
 *     This file is the public interface of the BMM350 motion sensor.
 *
 ********************************************************************************
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

#pragma once

#include "bmm350.h"
#include "cyhal.h"
#include "cy_result.h"

#if defined(__cplusplus)
extern "C"
{
#endif

typedef cyhal_gpio_callback_data_t  _mtb_bmm350_interrupt_pin_t;

/**
 * Structure holding the BMM instance specific information.
 *
 * Application code should not rely on the specific content of this struct.
 * They are considered an implementation detail which is subject to change
 * between platforms and/or library releases.
 */
typedef struct
{
    struct bmm350_dev           sensor;
    _mtb_bmm350_interrupt_pin_t intpin;
} mtb_bmm350_t;

/** Structure holding the magnetometer data read from the device. */
typedef struct
{
    /** data */
    struct bmm350_mag_temp_data sensor_data;
} mtb_bmm350_data_t;

/**
 * Enumeration used for selecting I2C address.
 */
typedef enum
{
    /** I2C address */
    MTB_BMM350_ADDRESS_DEFAULT = BMM350_I2C_ADSEL_SET_LOW,
    MTB_BMM350_ADDRESS_SEC     = BMM350_I2C_ADSEL_SET_HIGH
} mtb_bmm350_address_t;

/*****************************************************************************
* Function name: mtb_bmm350_init_i2c
*****************************************************************************
* Summary:
* This function initializes the I2C instance, configures the BMM350, and sets
* platform-dependent function pointers.
*
* Parameters:
*  dev             Pointer to a BMM350 object. The caller must allocate the memory
*               for this object but the init function will initialize its contents.
*  i2c_instance    I2C instance to use for communicating with the BMM350 sensor.
*  address         BMM350 I2C address, set by hardware implementation.
* Return:
*  cy_rslt_t    CY_RSLT_SUCCESS if properly initialized, else an error indicating
*               what went wrong.
*
*****************************************************************************/
cy_rslt_t mtb_bmm350_init_i2c(mtb_bmm350_t* dev, cyhal_i2c_t* i2c_instance,
                              mtb_bmm350_address_t address);

/*****************************************************************************
* Function Name: mtb_bmm350_read
*****************************************************************************
* Summary:
* This function gets the sensor data for Magnetometer
*
* Parameters:
*  dev     Pointer to a BMM350 object. The caller must allocate the memory
*          for this object but the init function will initialize its contents.
*  data    The magnetometer data read from the BMM350 sensor.
*
* Return:
*  cy_rslt_t    CY_RSLT_SUCCESS if properly read, else an error indicating
*               what went wrong.
*
*****************************************************************************/
cy_rslt_t mtb_bmm350_read(mtb_bmm350_t* dev, mtb_bmm350_data_t* data);

/*****************************************************************************
* Function Name: mtb_bmm350_selftest
*****************************************************************************
* Summary:
* Performs Magnetometer self tests.
* Note: These tests cause a soft reset of the device and device should be
* reconfigured after a test.
*
* Parameters:
*  dev    Pointer to a BMM350 object. The caller must allocate the memory
*  for this object but the init function will initialize its contents.
*
* Return:
*  cy_rslt_t    CY_RSLT_SUCCESS if test passed, else an error indicating
*               what went wrong.
*
*****************************************************************************/
cy_rslt_t mtb_bmm350_selftest(mtb_bmm350_t* dev);

/*****************************************************************************
* Function Name: mtb_bmm350_free_pin
*****************************************************************************
* Summary:
* Frees up any resources allocated by the motion_sensor as part of \ref mtb_bmm350_init_i2c().
*
* Parameters:
*  dev    Pointer to a BMM350 object. The caller must allocate the memory
*  for this object but the init function will initialize its contents.
*****************************************************************************/
void mtb_bmm350_free_pin(mtb_bmm350_t* dev);

#if defined(__cplusplus)
}
#endif

/* [] END OF FILE */

/******************************************************************************
 * \file mtb_bmm350.c
 *
 * \brief
 *     This file contains the functions for interacting with the
 *     BMM350 magnetic sensor.
 *
 ********************************************************************************
 * \copyright
 * Copyright (c) 2025 Infineon Technologies AG
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

#include <stdlib.h> // For malloc
#include <string.h> // For memcpy

/******************************************************************************
* Macros
******************************************************************************/
#define _SOFT_RESET_DELAY_US       300
#define _I3C_CMD_LENGTH            (1u)

/******************************************************************************
* Global variables
******************************************************************************/
static cy_stc_i3c_context_t* _bmm350_i3c_context = NULL;
static I3C_CORE_Type* _bmm350_i3c_hw = NULL;
static cy_stc_i3c_device_t* _bmm350_i3c_device = NULL;

/*****************************************************************************
* Local function Prototypes
*****************************************************************************/

static BMM350_INTF_RET_TYPE _bmm350_i3c_read(uint8_t reg_addr, uint8_t* reg_data, uint32_t length,
                                             void* intf_ptr);

static BMM350_INTF_RET_TYPE _bmm350_i3c_write(uint8_t reg_addr, const uint8_t* reg_data,
                                              uint32_t length,
                                              void* intf_ptr);

static void _bmm350_delay_us(uint32_t period, void* intf_ptr);


/******************************************************************************
* mtb_bmm350_init_i3c
******************************************************************************/
cy_rslt_t mtb_bmm350_init_i3c(mtb_bmm350_t* dev, I3C_CORE_Type* i3c_hw,
                              cy_stc_i3c_context_t* i3c_context, cy_stc_i3c_device_t* i3c_device)
{
    cy_rslt_t rslt;
    uint8_t int_ctrl, err_reg_data = 0, soft_reset = 0;
    static struct bmm350_pmu_cmd_status_0 pmu_cmd_stat_0;
    static cy_stc_i3c_ccc_cmd_t cccCmd;
    static cy_stc_i3c_ccc_payload_t cccPayload;

    CY_ASSERT(NULL != i3c_context);
    CY_ASSERT(NULL != dev);

    _bmm350_i3c_context = i3c_context;
    _bmm350_i3c_hw = i3c_hw;
    _bmm350_i3c_device = i3c_device;

    /* Configure I3C CCC settings */
    cccCmd.address = CY_I3C_BROADCAST_ADDR;
    cccCmd.cmd = CY_I3C_CCC_RSTDAA(true);
    cccCmd.data = &cccPayload;
    cccCmd.data->data = NULL;
    cccCmd.data->len = 0U;

    /* Configure BMM350 sensor settings */
    dev->sensor.intf_ptr = NULL;
    dev->sensor.read = _bmm350_i3c_read;
    dev->sensor.write = _bmm350_i3c_write;
    dev->sensor.delay_us = _bmm350_delay_us;
    dev->sensor.mraw_override = NULL;

    rslt = Cy_I3C_SendCCCCmd(_bmm350_i3c_hw, &cccCmd, _bmm350_i3c_context);
    if (CY_RSLT_SUCCESS != rslt)
    {
        return rslt;
    }

    rslt =
        Cy_I3C_ControllerAttachI3CDevice(_bmm350_i3c_hw, _bmm350_i3c_device, _bmm350_i3c_context);
    if (CY_RSLT_SUCCESS != rslt)
    {
        return rslt;
    }

    rslt = Cy_I3C_ControllerStartEntDaa(_bmm350_i3c_hw, _bmm350_i3c_context);
    if (CY_RSLT_SUCCESS != rslt)
    {
        return rslt;
    }

    soft_reset = BMM350_CMD_SOFTRESET;

    rslt = bmm350_set_regs(BMM350_REG_CMD, &soft_reset, _I3C_CMD_LENGTH, &(dev->sensor));
    if (BMM350_OK != rslt)
    {
        return rslt;
    }
    else
    {
        rslt = bmm350_delay_us(BMM350_SOFT_RESET_DELAY, &(dev->sensor));
    }

    rslt = Cy_I3C_SendCCCCmd(_bmm350_i3c_hw, &cccCmd, _bmm350_i3c_context);
    if (CY_RSLT_SUCCESS != rslt)
    {
        return rslt;
    }

    rslt =
        Cy_I3C_ControllerAttachI3CDevice(_bmm350_i3c_hw, _bmm350_i3c_device, _bmm350_i3c_context);
    if (CY_RSLT_SUCCESS != rslt)
    {
        return rslt;
    }

    rslt = Cy_I3C_ControllerStartEntDaa(_bmm350_i3c_hw, _bmm350_i3c_context);
    if (CY_RSLT_SUCCESS != rslt)
    {
        return rslt;
    }

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
    rslt = bmm350_get_regs(BMM350_REG_ERR_REG, &err_reg_data, _I3C_CMD_LENGTH, &(dev->sensor));
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
    rslt = bmm350_get_regs(BMM350_REG_INT_CTRL, &int_ctrl, _I3C_CMD_LENGTH, &(dev->sensor));
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
    if (BMM350_OK == rslt)
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
* mtb_bmm350_set_odr_performance
******************************************************************************/
cy_rslt_t mtb_bmm350_set_odr_performance(enum bmm350_data_rates odr,
                                         enum bmm350_performance_parameters performance,
                                         mtb_bmm350_t* dev)
{
    cy_rslt_t rslt;
    rslt = bmm350_set_odr_performance(odr, performance, &(dev->sensor));
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
    Cy_SysLib_DelayUs(_SOFT_RESET_DELAY_US);
    return (BMM350_OK == rslt)
            ? CY_RSLT_SUCCESS
            : rslt;
}


/******************************************************************************
* mtb_bmm350_free_pin
******************************************************************************/
void mtb_bmm350_free_pin(mtb_bmm350_t* dev)
{
    _bmm350_i3c_context = NULL;
}


/*****************************************************************************
* Function name: _bmm350_i3c_read
*****************************************************************************
* Summary:
* This internal function reads I3C function map to host MCU
*
* Parameters:
*  reg_addr    8-bit register address of the sensor
*  reg_data    Data from the specified address
*  len         Length of the reg_data array
*  intf_ptr    Void pointer that can enable the linking of descriptors for interface related
*  callbacks
*
* Return:
*  int8_t     Status of execution
*
*****************************************************************************/
static BMM350_INTF_RET_TYPE _bmm350_i3c_read(uint8_t reg_addr, uint8_t* reg_data,
                                             uint32_t len, void* intf_ptr)
{
    cy_stc_i3c_controller_xfer_config_t data;
    cy_rslt_t result;

    CY_UNUSED_PARAMETER(intf_ptr);

    result = Cy_I3C_ControllerWriteByte(_bmm350_i3c_hw, _bmm350_i3c_device->dynamicAddress,
                                        reg_addr, _bmm350_i3c_context);

    if (CY_RSLT_SUCCESS != result)
    {
        Cy_I3C_Resume(_bmm350_i3c_hw, _bmm350_i3c_context);
    }

    data.targetAddress = _bmm350_i3c_device->dynamicAddress;
    data.buffer = (void*)reg_data;
    data.bufferSize = len;
    data.toc = true;

    result = Cy_I3C_ControllerRead(_bmm350_i3c_hw, &data, _bmm350_i3c_context);

    if (CY_RSLT_SUCCESS != result)
    {
        Cy_I3C_Resume(_bmm350_i3c_hw, _bmm350_i3c_context);
    }

    while (CY_I3C_CONTROLLER_I3C_SDR_RD_XFER !=
           Cy_I3C_GetBusStatus(_bmm350_i3c_hw, _bmm350_i3c_context))
    {
        /* Wait for read to complete */
    }

    return result;
}


/*****************************************************************************
* Function name: _bmm350_i3c_write
*****************************************************************************
* Summary:
* This internal function writes I3C function map to host MCU
*
* Parameters:
*  reg_addr    8-bit register address of the sensor
*  reg_data    Data from the specified address
*  len         Length of the reg_data array
*  intf_ptr    Void pointer that can enable the linking of descriptors for interface related
*  callbacks
*
* Return:
*  int8_t     Status of execution
*
*****************************************************************************/
static BMM350_INTF_RET_TYPE _bmm350_i3c_write(uint8_t reg_addr, const uint8_t* reg_data,
                                              uint32_t len, void* intf_ptr)
{
    cy_stc_i3c_controller_xfer_config_t data;
    cy_rslt_t result;

    CY_UNUSED_PARAMETER(intf_ptr);

    uint8_t* tmp = (uint8_t*)malloc(len + 1);
    tmp[0] = reg_addr;
    memcpy(&tmp[1], reg_data, len);

    data.targetAddress = _bmm350_i3c_device->dynamicAddress;
    data.buffer = (void*)tmp;
    data.bufferSize = len + 1;
    data.toc = false;

    result = Cy_I3C_ControllerWrite(_bmm350_i3c_hw, &data, _bmm350_i3c_context);
    if (CY_RSLT_SUCCESS != result)
    {
        Cy_I3C_Resume(_bmm350_i3c_hw, _bmm350_i3c_context);
    }

    while (CY_I3C_CONTROLLER_I3C_SDR_WR_XFER !=
           Cy_I3C_GetBusStatus(_bmm350_i3c_hw, _bmm350_i3c_context))
    {
        /* Wait for write to complete */
    }

    return result;
}


/*****************************************************************************
* Function name: _bmm350_delay_us
*****************************************************************************
* Summary:
* This internal function maps delay function to host MCU
*
* Parameters:
*  period    The time period in microseconds
*  intf_ptr  Void pointer that can enable the linking of descriptors for interface related callbacks
*
* Return:
*  void
*
*****************************************************************************/
static void _bmm350_delay_us(uint32_t period, void* intf_ptr)
{
    CY_UNUSED_PARAMETER(intf_ptr);

    Cy_SysLib_DelayUs(period);
}


#if defined(__cplusplus)
}
#endif


/* [] END OF FILE */

# BMM350 API Reference Guide

## General Description

Basic set of API for interacting with the BMM350 sensor.

This provides basic initialization and access to the magnetometer data. It also provides access to the base BMM350 driver for full control. For more information about the sensor, see: https://github.com/boschsensortec/BMM350_SensorAPI/tree/v1.4.0

## Code Snippets

### Snippet 1: Simple initialization with I2C
The following snippet initializes an I2C instance and the BMM350.

```
/* Initialize I2C and BMM350 */
result = cyhal_i2c_init(&i2c_instance, CYBSP_I2C_SDA, CYBSP_I2C_SCL, NULL);
result = mtb_bmm350_init_i2c(&mag, &i2c_instance, MTB_BMM350_ADDRESS_SEC);
```

### Snippet 2: Read magnetometer data
The following snippet demonstrates how to read magnetometer from BMM350.

```
/* Read data from BM350 */
mtb_bmm350_data_t data;
result = mtb_bmm350_read(&bmm, &data);
```

## Data Structure
```
struct mtb_bmm350_t         Structure holding the BMM instance specific information
struct mtb_bmm350_data_t    Structure holding the magnetometer and temperature data read from the device
```

## Enumerations
Enumeration used for selecting I2C address.

 ```
typedef enum
{
    MTB_BMM350_ADDRESS_DEFAULT  = BMM350_I2C_ADSEL_SET_LOW,
    MTB_BMM350_ADDRESS_SEC      = BMM350_I2C_ADSEL_SET_HIGH
} mtb_bmm350_address_t;
```

## Functions
cy_rslt_t `mtb_bmm350_init_i2c(mtb_bmm350_t* dev, cyhal_i2c_t* i2c_instance, mtb_bmm350_address_t address)`
>Initializes the sensor with I2C interface.

cy_rslt_t `mtb_bmm350_read(mtb_bmm350_t* dev, mtb_bmm350_data_t* data)`
>Reads the current magnetometer data from the sensor.

cy_rslt_t `mtb_bmm350_selftest(mtb_bmm350_t* dev)`
>Performs magnetometer self tests.

void `mtb_bmm350_free_pin(mtb_bmm350_t* dev)`
>Frees up any resources allocated by the sensor as part of mtb_bmm350_init_i2c().

## Data Structure Documentation
- mtb_bmm350_t

 Data Fields                      |           object       |    Description
 :-------                       |  :------------         |  :------------
 bmm350_dev                     | sensor                 | Structure to define BMM350 sensor configurations.
 _mtb_bmm350_interrupt_pin_t    | intpin                 | Internal structure containing callback data for pins.

- mtb_bmm350_data_t

 Data Fields                |           object       |    Description
 :-------                 |  :------------         |  :------------
 bmm350_mag_temp_data     | sensor_data            |   Magnetometer data

## Function Documentation
#### mtb_bmm350_init
- cy_rslt_t `mtb_bmm350_init_i2c(mtb_bmm350_t* dev, cyhal_i2c_t* i2c_instance, mtb_bmm350_address_t address)`

> **Summary:** This function initializes the I2C instance, configures the BMM350, and sets platform-dependent function pointers.
>
> **Parameter:**
>  Parameter             |  Description
>  :-------              |  :------------
>  dev                   |  Pointer to a BMM350 object. The caller must allocate the memory for this object but the init function will initialize its contents.
>  i2c_instance          |  I2C instance to use for communicating with the BMM350 sensor.
>  address               |  BMM350 I2C address, set by hardware implementation
>
> Return:
>  - cy_rslt_t           :  CY_RSLT_SUCCESS if properly initialized, else an error indicating what went wrong.

#### mtb_bmm350_read
- cy_rslt_t `mtb_bmm350_read(mtb_bmm350_t* dev, mtb_bmm350_data_t* data)`

>**Summary:** Reads the current Magnetometer data from the sensor.
>
> **Parameter:**
>  Parameter            |  Description
>  :-------             |  :------------
>  dev                  |  Pointer to a BMM350 object. The caller must allocate the memory for this object but the init function will initialize its contents.
>  data                 |  The Magnetometer data read from the sensor.
>
> Return:
>  - cy_rslt_t          :  CY_RSLT_SUCCESS if properly read, else an error indicating what went wrong.

#### mtb_bmm350_selftest
- cy_rslt_t `mtb_bmm350_selftest(mtb_bmm350_t* dev)`

>**Summary:** Performs Magnetometer self tests.
>
> Note these tests cause a soft reset of the device and device should be reconfigured after a test.
>
> **Parameter:**
>  Parameter             |  Description
>  :-------              |  :------------
>  dev                   |  Pointer to a BMM350 object. The caller must allocate the memory for this object but the init function will initialize its contents.
>
> Return:
>  - cy_rslt_t           :  CY_RSLT_SUCCESS if test passed, else an error indicating what went wrong.

#### mtb_bmm350_free_pin
- void `mtb_bmm350_free_pin(mtb_bmm350_t* dev)`

>**Summary:** Frees up any resources allocated by the sensor as part of mtb_bmm350_init_i2c().
>
> **Parameter:**
>  Parameter             |  Description
>  :-------              |  :------------
>  dev                   |  Pointer to a BMM350 object. The caller must allocate the memory for this object but the init function will initialize its contents.

---
© 2024, Cypress Semiconductor Corporation (an Infineon company) or an affiliate of Cypress Semiconductor Corporation.

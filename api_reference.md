# BMM350 API reference guide

## General description

A basic set of API for interacting with the BMM350 sensor over I3C.

This provides basic initialization and access to the magnetometer data. It also provides access to the base BMM350 driver for full control. For more information about the sensor, see [BMM350-Sensor API](https://github.com/boschsensortec/BMM350_SensorAPI/tree/v1.4.0).


## Code snippets

### Snippet 1: Simple initialization with I3C

The following snippet initializes an I3C context and the BMM350.

> **Note:** Perform I3C bus initialization and device attachment prior to calling the sensor initialization.

```
/* Initializes I3C. */
initStatus = Cy_I3C_Init(CYBSP_I3C_CONTROLLER_HW, &CYBSP_I3C_CONTROLLER_config, &CYBSP_I3C_CONTROLLER_context);

/* Hooks interrupt service routine. */
sysStatus = Cy_SysInt_Init(&CYBSP_I3C_CONTROLLER_IRQ_cfg, &CYBSP_I3C_CONTROLLER_Interrupt);

/* Enables interrupt in NVIC. */
NVIC_EnableIRQ((IRQn_Type)CYBSP_I3C_CONTROLLER_IRQ_cfg.intrSrc);
Cy_I3C_Enable(CYBSP_I3C_CONTROLLER_HW, &CYBSP_I3C_CONTROLLER_context);

/* Initializes the BMM350 sensor. */
result = mtb_bmm350_init_i3c(&magnetometer, CYBSP_I3C_CONTROLLER_HW, &CYBSP_I3C_CONTROLLER_context, &i3c_device_address);
```

### Snippet 2: Read magnetometer data

The following snippet demonstrates how to read a magnetometer from BMM350.

```
/* Reads data from BMM350. */
mtb_bmm350_data_t data;
result = mtb_bmm350_read(&magnetometer, &data);
```

## Data

```
struct mtb_bmm350_t         Structure holding the BMM instance-specific information
struct mtb_bmm350_data_t    Structure holding the magnetometer and temperature data read from the device
```

## Enumerations
Enumeration used for selecting I2C/I3C physical address.

 ```
typedef enum
{
    MTB_BMM350_ADDRESS_DEFAULT  = BMM350_I2C_ADSEL_SET_LOW,
    MTB_BMM350_ADDRESS_SEC      = BMM350_I2C_ADSEL_SET_HIGH
} mtb_bmm350_address_t;
```

## Functions

cy_rslt_t `mtb_bmm350_init_i3c(mtb_bmm350_t* dev, I3C_CORE_Type* i3c_hw, cy_stc_i3c_context_t* i3c_context, cy_stc_i3c_device_t* i3c_device)`
- This function initializes the I3C context, configures the BMM350, and sets platform-dependent function pointers

cy_rslt_t `mtb_bmm350_read(mtb_bmm350_t* dev, mtb_bmm350_data_t* data)`
- This function gets the sensor data for Magnetometer

cy_rslt_t `mtb_bmm350_set_odr_performance(enum bmm350_data_rates odr, enum bmm350_performance_parameters performance, mtb_bmm350_t* dev)`
- This function sets the ODR and averaging factor

cy_rslt_t `mtb_bmm350_selftest(mtb_bmm350_t* dev)`
- Performs magnetometer self-tests

void `mtb_bmm350_free_pin(mtb_bmm350_t* dev)`
- Frees up any resources allocated by the sensor as part of mtb_bmm350_init_i3c()


## Data structure documentation

- mtb_bmm350_t

Data fields                    |           object       |    Description
:-------                       |  :------------         |  :------------
bmm350_dev                     | sensor                 | Structure to define BMM350 sensor configurations

<br>

- mtb_bmm350_data_t

Data fields              |           object       |    Description
:-------                 |  :------------         |  :------------
bmm350_mag_temp_data     | sensor_data            |   Magnetometer data

<br>

## Function documentation

#### mtb_bmm350_init_i3c

- cy_rslt_t `mtb_bmm350_init_i3c(mtb_bmm350_t* dev, I3C_CORE_Type* i3c_hw, cy_stc_i3c_context_t* i3c_context, cy_stc_i3c_device_t* i3c_device)`

  **Summary:** This function initializes the I3C context, configures the BMM350, and sets platform-dependent function pointers.

  **Parameter**
  Parameter             |  Description
  :-------              |  :------------
  dev                   |  Pointer to a BMM350 object. The caller must allocate the memory for this object but the `init` function will initialize its contents
  i3c_hw                |  I3C core to use for communicating with the BMM350 sensor
  i3c_context           |  I3C context to use for communicating with the BMM350 sensor
  i3c_device            |  I3C device structure to use for communicating with the BMM350 sensor

  **Return**
  - cy_rslt_t: CY_RSLT_SUCCESS if properly initialized, else an error indicating what went wrong


#### mtb_bmm350_read

- cy_rslt_t `mtb_bmm350_read(mtb_bmm350_t* dev, mtb_bmm350_data_t* data)`

  **Summary:** Reads the current Magnetometer data from the sensor.

  **Parameter**
  Parameter            |  Description
  :-------             |  :------------
  dev                  |  Pointer to a BMM350 object. The caller must allocate the memory for this object but the `init` function will initialize its contents
  data                 |  The Magnetometer data read from the sensor

  **Return**
  - cy_rslt_t: CY_RSLT_SUCCESS if properly read, else an error indicating what went wrong


#### mtb_bmm350_set_odr_performance

- cy_rslt_t `mtb_bmm350_set_odr_performance(enum bmm350_data_rates odr, enum bmm350_performance_parameters performance, mtb_bmm350_t* dev)`

  **Summary:** This function sets the ODR and averaging factor

  **Parameter**
  Parameter            |  Description
  :-------             |  :------------
  odr                  |  BMM350 ODR data rate
  performance          |  BMM350 averaging performance parameter
  dev                  |  Pointer to a BMM350 object. The caller must allocate the memory for this object but the `init` function will initialize its contents

  **Return**
  - cy_rslt_t: CY_RSLT_SUCCESS if test passed, else an error indicating what went wrong


#### mtb_bmm350_selftest

- cy_rslt_t `mtb_bmm350_selftest(mtb_bmm350_t* dev)`

  **Summary:** Performs Magnetometer self-tests.

  Note that these tests cause a soft reset of the device and the device should be reconfigured after a test.

  **Parameter**
  Parameter             |  Description
  :-------              |  :------------
  dev                   |  Pointer to a BMM350 object. The caller must allocate the memory for this object but the `init` function will initialize its contents

  **Return**
  - cy_rslt_t: CY_RSLT_SUCCESS if test passed, else an error indicating what went wrong


#### mtb_bmm350_free_pin

- void `mtb_bmm350_free_pin(mtb_bmm350_t* dev)`

  **Summary:** Frees up any resources allocated by the sensor as part of mtb_bmm350_init_i3c().

  **Parameter**
  Parameter             |  Description
  :-------              |  :------------
  dev                   |  Pointer to a BMM350 object. The caller must allocate the memory for this object but the `init` function will initialize its contents

---
© 2025, Cypress Semiconductor Corporation (an Infineon company) or an affiliate of Cypress Semiconductor Corporation.
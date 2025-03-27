# BMM350 Magnetometer Sensor Release Notes

This library provides functions for interfacing with the BMM350 I2C 3-axis magnetometer used on the CY8CKIT-062S2-AI and SHIELD_XENSIV_A.

## What's Included?

- APIs for initializing/de-initializing the driver
- APIs for reading magnetometer
- API for testing BMM350 sensor

Refer to the [README.md](./README.md) and the [API Reference Guide](./api_reference.md) for a complete description of the sensor-orientation-bmm350 library.

## What Changed?

#### v0.5.2
- The `mraw_override` member in the `bmm350_dev` structure has been set to NULL by default.

#### v0.5.1
- Added API reference manual.

#### v0.5.0
- Initial release

## Supported Software and Tools

This version of the sensor-orientation-bmm350 was validated for the compatibility with the following Software and Tools:

| Software and Tools                                      | Version |
| :---                                                    | :----:  |
| ModusToolbox Software Environment                       | 3.1.0   |
| mtb-hal-cat1                                            | 2.6.0   |
| GCC Compiler                                            | 11.3.1  |
| IAR Compiler                                            | 9.50.2  |
| ARM Compiler 6                                          | 6.22    |

## More information

For more information, refer to the following documents:

* [API Reference Guide](./api_reference.md)
* [ModusToolbox Software Environment, Quick Start Guide, Documentation, and Videos](https://www.infineon.com/cms/en/design-support/tools/sdk/modustoolbox-software)
* [PSoC™ 6 Code Examples using ModusToolbox™ IDE](https://github.com/infineon/Code-Examples-for-ModusToolbox-Software)
* [Infineon Technologies AG](https://www.infineon.com)

-----
© 2024, Cypress Semiconductor Corporation (an Infineon company) or an affiliate of Cypress Semiconductor Corporation.

# BMM350 magnetometer sensor release notes

This library provides functions for interfacing with the BMM350 I3C 3-axis magnetometer used on the [PSOC&trade; Edge E84 Evaluation Kit](https://www.infineon.com/KIT_PSE84_EVAL) and [PSOC&trade; Edge E84 AI Kit](https://www.infineon.com/KIT_PSE84_AI).


## What's included?

- APIs for initializing/de-initializing the driver
- APIs for reading magnetometer
- API for testing BMM350 sensor

See the [README.md](./README.md) and the [API reference guide](./api_reference.md) for a complete description of the sensor-orientation-bmm350 library.


## What changed?

#### v1.0.0

- Initial release for PSOC&trade; Edge Kit

#### v0.5.2

- The `mraw_override` member in the `bmm350_dev` structure has been set to NULL by default

#### v0.5.1

- Added API reference guide

#### v0.5.0

- Initial release


## Supported software and tools

This version of the sensor-orientation-bmm350 is validated for the compatibility with the following software and tools:

Software and tools                                      | Version
:---                                                    | :----:
ModusToolbox&trade; software environment                | 3.6.0
GCC Compiler                                            | 14.2.1
IAR Compiler                                            | 9.50.2
Arm&reg; Compiler                                       | 6.22
LLVM_ARM Compiler                                       | 19.1.5

<br>

## More information

For more information, see the following documents:

* [API reference guide](./api_reference.md)
* [ModusToolbox&trade; software environment, quick start guide, documentation, and videos](https://www.infineon.com/modustoolbox)
* [AN239191](https://www.infineon.com/AN239191) – Getting started with graphics on PSOC&trade; Edge MCU
* [Infineon Technologies AG](https://www.infineon.com)

---
© 2025, Cypress Semiconductor Corporation (an Infineon company) or an affiliate of Cypress Semiconductor Corporation.

# Change Log

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).


## Unreleased
(v1.4.0 targeted for 2023-08-31) ([GitHub compare v1.3.1...master](https://github.com/eeros-project/bbblue-eeros/compare/v1.3.1...master))

### Added Features


## v1.3.1
(2023-04-04) ([GitHub compare v1.3.0...v1.3.1](https://github.com/eeros-project/bbblue-eeros/compare/v1.3.0...v1.3.1))

### Fixes
* Fix CMAKE_MODULE_PATH issue
* Increase recommended EEROS version


## v1.3.0
(2021-07-13) ([GitHub compare v1.2.0...v1.3.0](https://github.com/eeros-project/bbblue-eeros/compare/v1.2.0...v1.3.0))

### Added Features
* Update version
* Add servo channels
* Add support for imu and barometer
* Change to librobotcontrol version 1.1.0


## v1.2.0
(2019-10-22) ([GitHub compare v1.1.0...v1.2.0](https://github.com/eeros-project/bbblue-eeros/compare/v1.1.0...v1.2.0))

### Added Features
* **AnalogOut:** Allow reading back of analog output value. ([2d0268a](https://github.com/eeros-project/bbblue-eeros/commit/2d0268a04e3c4df49c087f609da2e8aade722a60))
* **cmake:** Simplify versioning and dependency management. ([pull request #7](https://github.com/eeros-project/bbblue-eeros/pull/7))

### Notes
A package config file and a package version file are generated and installed. A project using this wrapper library can use *"find_package(bbblue-eeros ${VERSION_TO_FIND} EXACT REQUIRED)"* to find the library in the needed version.


## v1.1.0
(2019-08-06) ([Github compare v1.0.0...v1.1.0](https://github.com/eeros-project/bbblue-eeros/compare/v1.0.0...v1.1.0))

### Added Features
* **Fqd:** Add set position function to Fqd. ([pull request #6](https://github.com/eeros-project/bbblue-eeros/pull/6))


## v1.0.0
(2019-02-28)

### Notes
This is the first official release. The bbblue-eeros wrapper library enables the
usage of the BBB hardware with the [EEROS Robotics Framework](http://eeros.org).

### Features
* **BBBlueDevice:** Initialize / cleanup the robotics cape library
* **AnalogIn:** Read the analog input, battery voltage, DC jack voltage values
* **AnalogOut:** Set the motor outputs on the BBB
* **DigIn:** Read the pause and mode buttons and the GPIOs
* **DigOut:** Set the green and red leds and the GPIOs
* **Fqd:** Read the encoder positions

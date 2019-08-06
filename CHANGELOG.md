# Change Log

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).


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

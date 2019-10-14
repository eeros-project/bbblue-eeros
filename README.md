# bbblue-eeros

Hardware wrapper library for the [BeagleBone® Blue](https://beagleboard.org/blue) board.

The bbblue-eeros wrapper library enables the usage of the BBB hardware with the [EEROS Robotics Framework](https://github.com/eeros-project/eeros-framework). 


## Documentation

- Project Website: (http://www.eeros.org)
- Project Wiki: (http://wiki.eeros.org)
- Hardware Abstraction Layer: (http://wiki.eeros.org/eeros_architecture/hal/start) 


## Getting Started

The easiest way to get started using EEROS on the BeagleBone® Blue is described in [Installation and Setup](https://wiki.eeros.org/getting_started/install_and_setup_development_environment) and more specifically in [Use on a Beaglebone Blue Board](https://wiki.eeros.org/getting_started/install_and_setup_development_environment/use_on_bbb).

The EEROS framework uses various [hardware libraries](http://wiki.eeros.org/eeros_architecture/hal/hardware_libraries) to access the underlying hardware. For the BeagleBone® Blue, the [Robot Control Library](https://github.com/StrawsonDesign/librobotcontrol) is used through this hardware wrapper library. 

The EEROS [Hardware Abstraction Layer](http://wiki.eeros.org/eeros_architecture/hal/start) needs a [configuration file](http://wiki.eeros.org/eeros_architecture/hal/configuration_file) which describes the hardware. A hardware configuration file describing all features in this hardware wrapper library can be found in the root of this repository. It is called * *HwConfigBBBlue.json* *.


## Projects using EEROS on the BeagleBone® Blue

### EEDURO Delta Robot
The EEDURO Delta Robot is an open robotics platform. The goal is to develop a family of different, small sized low cost robots. More information is found at http://hw.eeros.org/eeduro/.


## How to contribute to bbblue-eeros

The [EEROS Team](http://eeros.org/eeros-team/) would love to accept your contributions! The development on the EEROS Framework is done with the work flow “**develop with a fork**”. So please fork the repository, develop and test your code changes. For code quality, please follow the guidelines put together [here](http://wiki.eeros.org/for_developers/start). In general, the code should adheres to the existing style in the project. Once the changes are ready, a pull request is submitted. Each logical change should be submitted separately to ensure that the history will be understandable.

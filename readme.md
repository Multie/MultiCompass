MultiCompass Library
====================

The MultiCompass library is a platformIO library for Arduino that provides an interface for reading data from compass sensors like the HMC5883L. The library contains two classes:

*   MultiCompass: This is the generic compass class that each sensor inherits from.
*   MultiCompassHMC5883L: This is a specific class for the HMC5883L compass sensor.

Installation
------------

To install this library, simply add `multie/MultiCompass` to your `platformio.ini` dependencies:

```` ini
[env:your_environment] 
platform = your_platform
board = your_board 
... 
lib_deps = 
    multie/MultiCompass
````
Usage
-----

### MultiCompass Class

The `MultiCompass` class is a generic class that serves as the parent for all the specific compass classes. It provides methods for reading the raw data from the sensor, calibrating the sensor, and setting the mode and settings of the sensor.

### MultiCompassHMC5883L Class

The `MultiCompassHMC5883L` class is a specific class for the HMC5883L compass sensor. It inherits from the `MultiCompass` class and provides methods for configuring and reading data from the HMC5883L sensor.

Examples
--------

The library comes with one example, `compassHMC5883L.ino`, which demonstrates how to use the `MultiCompassHMC5883L` class to read data from the HMC5883L sensor.

File Structure
--------------
````
MultiCompass
├── examples
│   └── compassHMC5883L.ino
├── include
│   ├── MultiCompass.h
│   └── MultiCompassHMC5883L.h
└── src
    ├── MultiCompass.cpp
    └── MultiCompassHMC5883L.cpp
````


Conclusion
----------

The MultiCompass library is a useful tool for communicating with compass sensors like the HMC5883L on Arduino boards. It provides a simple interface for reading raw data, calibrating, and configuring the sensors.
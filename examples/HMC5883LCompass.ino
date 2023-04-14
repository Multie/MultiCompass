/*
# A step by step tutorial:

## Step 1: Install the required libraries
This sketch uses two libraries: Arduino.h and Wire.h, which are included with the Arduino IDE, and the custom library MultiCompassHMC5883L, which needs to be installed manually. To install the MultiCompassHMC5883L library, follow these steps:

    Open the Arduino IDE.
    Go to Sketch > Include Library > Manage Libraries.
    Search for "MultiCompassHMC5883L" and click Install.

## Step 2: Set up the hardware
This sketch assumes that you have an HMC5883L compass sensor connected to your Arduino board using the I2C bus. Connect the SDA and SCL pins of the compass to the appropriate pins on your Arduino board.

## Step 3: Open the sketch in the Arduino IDE

    Open the Arduino IDE.
    Go to File > Open and select the provided sketch.

## Step 4: Upload the sketch to the Arduino board

    Connect your Arduino board to your computer using a USB cable.
    Select your board and serial port under the Tools menu.
    Click the Upload button to upload the sketch to your board.

## Step 5: Monitor the serial output

    Open the Serial Monitor by clicking the magnifying glass icon on the top right corner of the Arduino IDE.
    Set the baud rate to 115200.
    You should see "Start" printed in the serial monitor, indicating that the sketch has started running.

## Step 6: Calibrate the compass

    Hold the compass sensor flat and parallel to the ground.
    Move the sensor in a figure-eight pattern until the calibration is successful.
    The serial monitor will display the calibration settings, which you can copy and paste into the setup() function for later use.

## Step 7: Monitor the compass data

    After calibration, the sketch will continuously read the compass data and display it in the serial monitor.
    The X, Y, and Z values represent the magnetic field strength in the respective axes.
    The H value represents the heading in degrees.

Congratulations, you have successfully run the provided sketch and calibrated the compass! You can now use the compass data in your own projects.

*/

#include <Arduino.h> // Include the Arduino core library
#include <Wire.h> // Include the Wire library for I2C communication

#include "MultiCompassHMC5883L.h" // Include the custom MultiCompassHMC5883L library

TwoWire I2C1 = TwoWire(0); // Create a TwoWire object named I2C1 using the 0th I2C bus
MultiCompassHMC5883L compass(&I2C1); // Create a MultiCompassHMC5883L object named compass, using the I2C1 bus

void setup()
{
  Serial.begin(115200); // Initialize the serial communication at 115200 baud rate
  delay(1000); // Wait for 1 second to allow the serial communication to initialize
  Serial.println("Start"); // Print "Start" to the serial monitor

  Serial.print("Mode:"); // Print "Mode:" to the serial monitor
  I2C1.begin(26, 25); // Initialize the I2C communication with SDA pin at 26 and SCL pin at 25

  compass.setMode(HMC5883L_Mode::HMC5883L_MODE_CONTINOUS); // Set the HMC5883L compass sensor mode to continuous measurement mode
  compass.setAveragedSamples(HMC5883L_Samples::HMC5883L_SAMPLES_8); // Set the number of averaged samples to 8

  CompassSetting settings = {}; // Create a new CompassSetting object named settings
  settings.minX = 0; // Set the minimum X value of the calibration settings to 0
  settings.minY = 0; // Set the minimum Y value of the calibration settings to 0
  settings.minZ = 0; // Set the minimum Z value of the calibration settings to 0
  settings.maxX = 0; // Set the maximum X value of the calibration settings to 0
  settings.maxY = 0; // Set the maximum Y value of the calibration settings to 0
  settings.maxZ = 0; // Set the maximum Z value of the calibration settings to 0
  compass.setCalibration(&settings); // Set the calibration settings for the compass sensor
}

// Define a boolean variable to keep track of the old calibration status
bool oldCalibration = false;

// Start the main loop
void loop()
{
  // Define a CompassData structure to hold the sensor data
  CompassData data = {};

  // Read the sensor data and store it in the CompassData structure
  compass.getData(&data);

  // Call the calibration function to check if calibration is needed
  bool calibration = compass.calibration(&data);

  // Check if the calibration was successful
  if (calibration)
  {
    // Print a message indicating that the calibration was successful
    Serial.println("Calibrated");

    // Get the sensor data again and scale it based on the calibration values
    compass.getData(&data);
    compass.scaleData(&data);

    // Calculate the heading based on the scaled sensor data
    compass.calculateHeading(&data, 0, 0, 1);

    // Print the scaled sensor data and the calculated heading
    Serial.print(" X:");
    Serial.print(data.scaledX, 3);
    Serial.print(" Y:");
    Serial.print(data.scaledY, 3);
    Serial.print(" Z:");
    Serial.print(data.scaledZ, 3);
    Serial.print("  H:");
    Serial.print(data.heading / (2 * PI) * 360);

    // Check if this is the first successful calibration
    if (!oldCalibration) {
      // If it is, print the calibration settings for later use
      Serial.println("Copy this calibration at the bottom of the setup:");
      Serial.println("CompassSetting settings = {};");
      Serial.println("settings.minX = " + String(compass.settings.minX) +";");
      Serial.println("settings.minY = " + String(compass.settings.minY) +";");
      Serial.println("settings.minZ = " + String(compass.settings.minZ) +";");
      Serial.println("settings.maxX = " + String(compass.settings.maxX) +";");
      Serial.println("settings.maxY = " + String(compass.settings.maxY) +";");
      Serial.println("settings.maxZ = " + String(compass.settings.maxZ) +";");
      Serial.println("compass.setCalibration(&settings);");
    }

    // Print a newline character and wait for a second before looping again
    Serial.println();
    delay(1000);
  }
  else
  {
    // If the calibration was not successful, print the current calibration settings
    Serial.print("Calibrating:");
    Serial.print(" minX:");
    Serial.print(compass.settings.minX);
    Serial.print(" minY:");
    Serial.print(compass.settings.minY);
    Serial.print(" minZ:");
    Serial.print(compass.settings.minZ);
    Serial.print(" maxX:");
    Serial.print(compass.settings.maxX);
    Serial.print(" maxY:");
    Serial.print(compass.settings.maxY);
    Serial.print(" maxZ:");
    Serial.print(compass.settings.maxZ);
    Serial.println();

    // Wait for 10 milliseconds before looping again
    delay(10);
  }

  // Update the old calibration status
  oldCalibration = calibration;
}
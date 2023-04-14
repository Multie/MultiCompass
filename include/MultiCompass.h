/**
 * @file MultiCompass.h
 * @brief Header file for MultiCompass class
 * This file contains the declarations for the MultiCompass class which reads data from
 * multiple compass sensors and provides scaled and calibrated data.
    */

#ifndef MULTICOMPASS_H
#define MULTICOMPASS_H

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

typedef struct
{
    float rawX;
    float rawY;
    float rawZ;
    float scaledX;
    float scaledY;
    float scaledZ;
    float heading;
} CompassData;

typedef struct
{
    float minX;
    float minY;
    float minZ;
    float maxX;
    float maxY;
    float maxZ;
    float heading;
    long long lastCalibration;
} CompassSetting;

/**
 * @class MultiCompass
 * @brief Class for reading data from multiple compass sensors and providing scaled and calibrated data.
 * This class provides methods for setting the declination angle, calibration settings, and retrieving scaled and calibrated
 * compass data. It also includes methods for scaling and calculating the heading based on the raw sensor data.
 */
class MultiCompass
{
public:
    /**
     * @brief Constructor for MultiCompass class.
     * @param wire Pointer to the Wire object for I2C communication.
     */
    MultiCompass(TwoWire *wire);

    /**
     * @brief Sets the declination angle for the compass.
     * @param declinationAngle The declination angle in degrees.
     */
    void setDeclinationAngle(float declinationAngle);

    /**
     * @brief Sets the calibration settings for the compass.
     * @param settings A pointer to a CompassSetting struct containing the calibration settings.
     */
    void setCalibration(CompassSetting *settings);

    /**
     * @brief Gets the calibration settings for the compass.
     * @param settings A pointer to a CompassSetting struct where the calibration settings will be stored.
     */
    void getCalibration(CompassSetting *settings);

    /**
     * @brief Scales the raw sensor data.
     * @param data A pointer to a CompassData struct containing the raw sensor data to be scaled.
     */
    void scaleData(CompassData *data);

    /**
     * @brief Calculates the heading based on the raw sensor data and current calibration settings.
     * @param data A pointer to a CompassData struct containing the raw sensor data.
     * @param x The X axis of the sensor.
     * @param y The Y axis of the sensor.
     * @param z The Z axis of the sensor.
     */
    void calculateHeading(CompassData *data, int x, int y, int z);

    /**
     * @brief Writes a byte to the specified register on the compass sensor.
     * @param reg The register to write to.
     * @param value The value to write to the register.
     */
    void writeByte(uint8_t reg, uint8_t value);

    /**
     * @brief Reads a byte from the specified register on the compass sensor.
     * @param reg The register to read from.
     * @return The value read from the register.
     */
    uint8_t readByte(uint8_t reg);

    /**
     * @brief Reads two bytes from the specified register on the compass sensor.
     * @param reg The register to read from.
     * @param LSB Whether to read the least significant byte or most significant byte.
     * @return The value read from the register.
     */
    int readTwoBytes(uint8_t reg, bool LSB);

    /**
     * @brief Reads the raw sensor data and calculates the scaled and calibrated compass data.
     * @param data A pointer to a CompassData struct where the scaled and calibrated data will be stored.
     */
    void getData(CompassData *data);

    /**
     * @brief Calibrates the compass based on the current sensor data.
     * @param data A pointer to a CompassData struct containing the current sensor data.
     * @return true if calibration was successful, false otherwise.
     */
    bool calibration(CompassData *data);

    CompassSetting settings; /**< The current calibration settings. */
    TwoWire *mywire;         /**< Pointer to the Wire object for I2C communication. */
    uint8_t adress;          /**< The I2C address of the compass sensor. */
private:
};

#endif
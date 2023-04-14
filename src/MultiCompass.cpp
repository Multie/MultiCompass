/*
 *   Copyright (c) 2023 Malte Hering
 *   All rights reserved.

 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at

 *   http://www.apache.org/licenses/LICENSE-2.0

 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#include "MultiCompass.h"
#include <math.h>
/**
 * @brief Create a new MultiCompass instance with the given TwoWire object.
 * @param wire A pointer to the TwoWire object to use for communication.
 */
MultiCompass::MultiCompass(TwoWire *wire)
{
    // Store a reference to the provided TwoWire object.
    mywire = wire;

    // Initialize the compass's calibration settings to default values.
    settings.heading = 0;
    settings.minX = 100000;
    settings.minY = 100000;
    settings.minZ = 100000;
    settings.maxX = -100000;
    settings.maxY = -100000;
    settings.maxZ = -100000;
}
/**
 * @brief Set the magnetic declination angle for the compass.
 * @param declinationAngle The new magnetic declination angle in radians.
 */
void MultiCompass::setDeclinationAngle(float declinationAngle)
{
    // Update the heading calibration setting to the new magnetic declination angle.
    settings.heading = declinationAngle;
}

/**
 * @brief Set the calibration settings for the compass.
 * @param setting A pointer to a CompassSetting object containing the new calibration settings.
 */
void MultiCompass::setCalibration(CompassSetting *setting)
{
    // Set the new calibration settings based on the provided object.
    settings.minX = setting->minX;
    settings.minY = setting->minY;
    settings.minZ = setting->minZ;
    settings.maxX = setting->maxX;
    settings.maxY = setting->maxY;
    settings.maxZ = setting->maxZ;
    settings.heading = setting->heading;
    settings.lastCalibration = 0;
};

/**
 * @brief Get the current calibration settings for the compass.
 * @param settings A pointer to a CompassSetting object where the current calibration settings will be stored.
 */
void getCalibration(CompassSetting *settings)
{
    // Copy the current calibration settings to the provided object.
    memcpy(settings, &settings, sizeof(settings));
};

/**
 * @brief Scale the provided CompassData object based on the minimum and maximum values in the settings.
 * @param data A pointer to a CompassData object containing the necessary data to be scaled.
 */
void MultiCompass::scaleData(CompassData *data)
{
    // Calculate the scaled X, Y, and Z values.
    data->scaledX = data->rawX - (settings.maxX + settings.minX) / 2;
    data->scaledY = data->rawY - (settings.maxY + settings.minY) / 2;
    data->scaledZ = data->rawZ - (settings.maxZ + settings.minZ) / 2;

    data->scaledX /= (abs(settings.maxX) + abs(settings.minX)) / 2;
    data->scaledY /= (abs(settings.maxY) + abs(settings.minY)) / 2;
    data->scaledZ /= (abs(settings.maxZ) + abs(settings.minZ)) / 2;
};

/**
 * @brief Calculate the heading using the provided CompassData object and axis values.
 * @param data A pointer to a CompassData object containing the necessary data for calculation.
 * @param x An integer value representing the x-axis value to use in the calculation. Default value is 0.
 * @param y An integer value representing the y-axis value to use in the calculation. Default value is 0.
 * @param z An integer value representing the z-axis value to use in the calculation. Default value is 1.
 */
void MultiCompass::calculateHeading(CompassData *data, int x = 0, int y = 0, int z = 1)
{
    float axis1 = 0, axis2 = 0;
    if (x != 0)
    {
        axis1 = data->scaledY * x;
        axis2 = data->scaledZ * x;
    }
    else if (y != 0)
    {
        axis1 = data->scaledX * y;
        axis2 = data->scaledZ * y;
    }
    else if (z != 0)
    {
        axis1 = data->scaledX * z;
        axis2 = data->scaledY * z;
    }
    
    // Calculate heading using the provided axis values.
    data->heading = atan2f(axis2, axis1);
    
    // Add the heading offset from the settings.
    data->heading += settings.heading;
    
    // Normalize the heading to be between 0 and 2*PI.
    if (data->heading < 0)
    {
        data->heading += 2 * PI;
    }
    else if (data->heading > 2 * PI)
    {
        data->heading -= 2 * PI;
    }
};

/**
 * @brief Retrieve data from the MultiCompass sensor and store it in a CompassData object.
 * @param data A pointer to a CompassData object where the retrieved data will be stored.
 */
void getData(CompassData *data)
{
}

/**
 * @brief Calibrate the MultiCompass sensor using the provided data.
 * @param data A pointer to a CompassData object containing the data to use for calibration.
 * @return A boolean value indicating whether the calibration was successful or not.
 */
bool calibration(CompassData *data)
{
    return false;
}

/**
 * @brief Write a byte of data to a specified register of the MultiCompass sensor.
 * @param reg The register to write to.
 * @param value The value to write to the register.
 */
void MultiCompass::writeByte(uint8_t reg, uint8_t value)
{
    // Begin transmission to the specified address.
    mywire->beginTransmission(adress);

    // Write the register and value to the wire object.
#if ARDUINO >= 100
    mywire->write(reg);
    mywire->write(value);
#else
    mywire->send(reg);
    mywire->send(value);
#endif

    // End the transmission.
    mywire->endTransmission();
};

/**
 * @brief This function reads a single byte from a given register address using I2C communication.
 * @param reg: The register address from where a byte will be read.
 * @return An unsigned 8-bit integer representing the byte read from the given register address.
 */
uint8_t MultiCompass::readByte(uint8_t reg)
{
    uint8_t value = 0;
    mywire->beginTransmission(adress); // Start the transmission with the given address.
#if ARDUINO >= 100
    mywire->write(reg); // Send the register address to read from.
#else
    mywire->send(reg);
#endif
   mywire->endTransmission(); // End the transmission

    mywire->requestFrom(adress, (uint8_t)1); // Request one byte from the given address.
    while (mywire->available() == 0)         // Wait until data is available to be read.
    {
    };
#if ARDUINO >= 100
    value = mywire->read(); // Read the byte.
#else
    value = mywire->receive();
#endif
    return value; // Return the byte read.
}

/**
 * @brief This function reads two bytes from a given register address using I2C communication.
 * @param reg: The register address from where two bytes will be read.
 * @param LSB: A boolean flag indicating the order in which bytes will be read. If true, LSB is read first.
 * @return An integer value representing the two bytes read from the given register address.
 */
int MultiCompass::readTwoBytes(uint8_t reg, bool LSB)
{
    int16_t value = 0;
    uint8_t vha, vla;
    mywire->beginTransmission(adress); // Start the transmission with the given address.
#if ARDUINO >= 100
    mywire->write(reg); // Send the register address to read from.
#else
    mywire->send(reg);
#endif
    mywire->endTransmission();                        // End the transmission.
    mywire->requestFrom((uint8_t)adress, (uint8_t)2); // Request two bytes from the given address.
    while (!mywire->available())                      // Wait until data is available to be read.
    {
    };
    if (LSB) // If LSB is to be read first.
    {
#if ARDUINO >= 100
        vha = mywire->read(); // Read the LSB.
        vla = mywire->read(); // Read the MSB.
#else
        vha = mywire->receive();
        vla = mywire->receive();
#endif
    }
    else // If MSB is to be read first.
    {
#if ARDUINO >= 100
        vla = mywire->read(); // Read the MSB.
        vha = mywire->read(); // Read the LSB.
#else
        vla = mywire->receive();
        vha = mywire->receive();
#endif
    }
    value = vha << 8 | vla; // Combine the two bytes to get the final value.
    return value;           // Return the final value.
}
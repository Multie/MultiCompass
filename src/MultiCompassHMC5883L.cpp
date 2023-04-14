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

#include "MultiCompassHMC5883L.h"

/**
 * @brief Construct a new MultiCompassHMC5883L object
 * @param wire1 A pointer to a TwoWire object that will be used for I2C communication
 */
MultiCompassHMC5883L::MultiCompassHMC5883L(TwoWire *wire1) : MultiCompass(wire1)
{
    adress = HMC5883L_ADDRESS + 1;
};

/**
 * @brief Set the operating mode of the HMC5883L magnetometer
 * @param mode The desired operating mode, as a HMC5883L_Mode enum value
 */
void MultiCompassHMC5883L::setMode(HMC5883L_Mode mode)
{
    uint8_t value = readByte(HMC5883L_REGISTER_MODE);
    value &= 0b11111100;
    value |= mode;
    writeByte(HMC5883L_REGISTER_MODE, value);
}
/**
 * @brief Get the current operating mode of the HMC5883L magnetometer
 * @return The current operating mode, as a HMC5883L_Mode enum value
 */
HMC5883L_Mode MultiCompassHMC5883L::getMode()
{
    uint8_t value = readByte(HMC5883L_REGISTER_MODE);
    value &= 0b00000011;
    return (HMC5883L_Mode)value;
}
/**
 * @brief Set the magnetic field range of the HMC5883L magnetometer
 * @param range The desired field range, as a HMC5883L_FieldRange enum value
 */
void MultiCompassHMC5883L::setFieldRange(HMC5883L_FieldRange range)
{
    writeByte(HMC5883L_REGISTER_CONFIG_B, range << 5);
}

/**
 * @brief Get the current magnetic field range of the HMC5883L magnetometer
 * 
 * @return The current magnetic field range, as a HMC5883L_FieldRange enum value
 */
HMC5883L_FieldRange MultiCompassHMC5883L::getFieldRange()
{
    // Read the current value of the configuration register for the HMC5883L
    uint8_t value = readByte(HMC5883L_REGISTER_CONFIG_B);
    // Extract the magnetic field range bits (bits 5-6) from the register value and cast to HMC5883L_FieldRange
    HMC5883L_FieldRange ret = (HMC5883L_FieldRange)((value >> 5) & 0b00000011);
    return ret;
}

/**
 * @brief Set the output data rate of the HMC5883L magnetometer
 * 
 * @param samplerate The desired output data rate, as a HMC5883L_OutputRate enum value
 */
void MultiCompassHMC5883L::setOutputRate(HMC5883L_OutputRate samplerate)
{
    uint8_t value = readByte(HMC5883L_REGISTER_CONFIG_A);
    // Clear bits 2-4 of the current value to make room for the new output data rate
    value &= 0b11100011;
    // Set the output data rate bits in the value
    value |= (samplerate << 2);
    // Write the new value to the configuration register
    writeByte(HMC5883L_REGISTER_CONFIG_A, value);
}
/**
 * @brief Get the current output data rate of the HMC5883L magnetometer
 * 
 * @return The current output data rate, as a HMC5883L_OutputRate enum value
 */
HMC5883L_OutputRate MultiCompassHMC5883L::getOutputRate()
{
    uint8_t value = readByte(HMC5883L_REGISTER_CONFIG_A);
    // Clear all but the bits 3-5 of the value to extract the output data rate
    value &= 0b00011100;
    // Shift the output data rate bits to the right to obtain the final value
    value >>= 2;
    return (HMC5883L_OutputRate)value;
}

/**
 * @brief Set the number of averaged samples for each measurement in the HMC5883L magnetometer
 * @param samples The desired number of averaged samples, as a HMC5883L_Samples enum value
 */
void MultiCompassHMC5883L::setAveragedSamples(HMC5883L_Samples samples)
{
    uint8_t value = readByte(HMC5883L_REGISTER_CONFIG_A);
    // Clear bits 5-6 of the current value to make room for the new number of averaged samples
    value &= 0b10011111;
    // Set the averaged samples bits in the value
    value |= (samples << 5);
    // Write the new value to the configuration register
    writeByte(HMC5883L_REGISTER_CONFIG_A, value);
}

/**
 * @brief Get the raw magnetic field data from the HMC5883L magnetometer
 * @param data Pointer to a CompassData struct to store the raw magnetic field data
 */
void MultiCompassHMC5883L::getData(CompassData *data)
{
    // Read the raw X, Y, and Z magnetic field data from the sensor
    data->rawX = readTwoBytes(HMC5883L_REGISTER_OUT_X_M, true);
    data->rawY = readTwoBytes(HMC5883L_REGISTER_OUT_Y_M, true);
    data->rawZ = readTwoBytes(HMC5883L_REGISTER_OUT_Z_M, true);
}
/**
 * @brief Calibration function for the HMC5883L compass module.
 * Updates the min and max values for each axis based on the current readings
 * from the compass sensor. Calibration is complete when the time since last calibration
 * is greater than the calibration period.
 * @param data pointer to a CompassData object containing the raw X, Y, and Z values
 * from the compass sensor
 * @return true if calibration is complete, false otherwise
 */
    bool MultiCompassHMC5883L::calibration(CompassData *data)
    {
    // Update minimum values for each axis
    if (data->rawX < settings.minX)
    {
    settings.minX = data->rawX;
    settings.lastCalibration = millis();
    }
    if (data->rawY < settings.minY)
    {
    settings.minY = data->rawY;
    settings.lastCalibration = millis();
    }
    if (data->rawZ < settings.minZ)
    {
    settings.minZ = data->rawZ;
    settings.lastCalibration = millis();
    }

    // Update maximum values for each axis
    if (data->rawX > settings.maxX)
    {
    settings.maxX = data->rawX;
    settings.lastCalibration = millis();
    }
    if (data->rawY > settings.maxY)
    {
    settings.maxY = data->rawY;
    settings.lastCalibration = millis();
    }
    if (data->rawZ > settings.maxZ)
    {
    settings.maxZ = data->rawZ;
    settings.lastCalibration = millis();
    }

    // Check if calibration is complete
    return (millis() - settings.lastCalibration) > calibrationPeriod;
    }
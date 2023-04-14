
/**
 * @file MultiCompass_HMC5883L.h
 * @brief This is the header file for the MultiCompassHMC5883L class, which is derived from the MultiCompass class.
 * This class provides a way to interface with the HMC5883L compass module via the Arduino framework and the Wire library.
 * It provides methods to set and get various parameters of the module, and also to obtain compass data and perform calibration.
 */

#ifndef MULTICOMPASS_HMC5883L_H
#define MULTICOMPASS_HMC5883L_H

#include <Arduino.h>
#include <Wire.h>

#include "MultiCompass.h"

const uint8_t HMC5883L_ADDRESS = (0x1E); ///< I2C address of the HMC5883L module

#define HMC5883L_REGISTER_CONFIG_A (0x00)
#define HMC5883L_REGISTER_CONFIG_B (0x01)
#define HMC5883L_REGISTER_MODE (0x02)
#define HMC5883L_REGISTER_OUT_X_M (0x03)
#define HMC5883L_REGISTER_OUT_X_L (0x04)
#define HMC5883L_REGISTER_OUT_Z_M (0x05)
#define HMC5883L_REGISTER_OUT_Z_L (0x06)
#define HMC5883L_REGISTER_OUT_Y_M (0x07)
#define HMC5883L_REGISTER_OUT_Y_L (0x08)
#define HMC5883L_REGISTER_STATUS (0x09)
#define HMC5883L_REGISTER_IDENT_A (0x0A)
#define HMC5883L_REGISTER_IDENT_B (0x0B)
#define HMC5883L_REGISTER_IDENT_C (0x0C)

typedef enum
{
    HMC5883L_SAMPLES_8 = 0b11,
    HMC5883L_SAMPLES_4 = 0b10,
    HMC5883L_SAMPLES_2 = 0b01,
    HMC5883L_SAMPLES_1 = 0b00,
} HMC5883L_Samples;

typedef enum
{
    HMC5883L_OUTPUTRATE_75HZ = 0b110,
    HMC5883L_OUTPUTRATE_30HZ = 0b101,
    HMC5883L_OUTPUTRATE_15HZ = 0b100,
    HMC5883L_OUTPUTRATE_7_5HZ = 0b011,
    HMC5883L_OUTPUTRATE_3HZ = 0b010,
    HMC5883L_OUTPUTRATE_1_5HZ = 0b001,
    HMC5883L_OUTPUTRATE_0_75_HZ = 0b000,
} HMC5883L_OutputRate;

typedef enum
{
    HMC5883L_FIELDRANGE_8_1GA = 0b111,
    HMC5883L_FIELDRANGE_5_6GA = 0b110,
    HMC5883L_FIELDRANGE_4_7GA = 0b101,
    HMC5883L_FIELDRANGE_4GA = 0b100,
    HMC5883L_FIELDRANGE_2_5GA = 0b011,
    HMC5883L_FIELDRANGE_1_9GA = 0b010,
    HMC5883L_FIELDRANGE_1_3GA = 0b001,
    HMC5883L_FIELDRANGE_0_88GA = 0b000,
} HMC5883L_FieldRange;

typedef enum
{
    HMC5883L_MODE_IDLE = 0b10,
    HMC5883_MODE_SINGLE = 0b01,
    HMC5883L_MODE_CONTINOUS = 0b00,
} HMC5883L_Mode;

/**
 * @class MultiCompassHMC5883L
 * @brief A class for interfacing with the HMC5883L compass module.
 * This class provides a way to interface with the HMC5883L compass module via the Arduino framework and the Wire library.
 * It provides methods to set and get various parameters of the module, and also to obtain compass data and perform calibration.
 */
class MultiCompassHMC5883L : public MultiCompass
{
public:
    /**
     * @brief Constructor for the MultiCompassHMC5883L class.
     * @param wire1 A pointer to a TwoWire object representing the I2C bus to which the HMC5883L module is connected.
     */
    MultiCompassHMC5883L(TwoWire *wire1);

    /**
     * @brief Sets the measurement mode of the HMC5883L module.
     * @param mode The measurement mode to set.
     */
    void setMode(HMC5883L_Mode mode);

    /**
     * @brief Gets the measurement mode of the HMC5883L module.
     * @return The current measurement mode.
     */
    HMC5883L_Mode getMode();

    /**
     * @brief Sets the field range of the HMC5883L module.
     * @param range The field range to set.
     */
    void setFieldRange(HMC5883L_FieldRange range);

    /**
     * @brief Gets the field range of the HMC5883L module.
     * @return The current field range.
     */
    HMC5883L_FieldRange getFieldRange();

    /**
     * @brief Sets the output data rate of the HMC5883L module.
     * @param samplerate The output data rate to set.
     */
    void setOutputRate(HMC5883L_OutputRate samplerate);

    /**
     * @brief Gets the output data rate of the HMC5883L module.
     * @return The current output data rate.
     */
    HMC5883L_OutputRate getOutputRate();

    /**
     * @brief Sets the number of averaged samples for the HMC5883L module.
     * @param samples The number of averaged samples to set.
     */
    void setAveragedSamples(HMC5883L_Samples samples);

    /**
     * @brief Gets the number of averaged samples for the HMC5883L module.
     * @return The current number of averaged samples.
     */
    HMC5883L_Samples getAveragedSamples();

    /**
     * @brief Obtains compass data from the HMC5883L module.
     * @param data A pointer to a CompassData object in which to store the compass data.
     */
    void getData(CompassData *data);

    /**
     * @brief Performs calibration of the HMC5883L module.
     *
     * This method performs calibration of the HMC5883L module by taking a number of readings and calculating the offset values.
     * The calibration period can be set by modifying the calibrationPeriod variable.
     *
     * @param data A pointer to a CompassData object in which to store the compass data.
     *
     * @return A boolean indicating whether the calibration was successful or not.
     */
    bool calibration(CompassData *data);

    int calibrationPeriod = 1000; ///< The calibration period, in milliseconds.

private:
};

#endif

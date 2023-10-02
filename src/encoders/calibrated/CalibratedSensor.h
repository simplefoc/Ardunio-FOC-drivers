#ifndef __CALIBRATEDSENSOR_H__
#define __CALIBRATEDSENSOR_H__

#include "common/base_classes/Sensor.h"
#include "BLDCMotor.h"
#include "common/base_classes/FOCMotor.h"
#include <Preferences.h>



// Type for saving and loading calibration parameters to/from EEPROM
const int CALIBRATION_DATA_LEN = sizeof(float) + 128 * sizeof(float) + sizeof(int);
typedef union {
    struct __attribute__((packed)){
        float zero_electric_angle;

        // Eccentricity LUT
        float calibration_lut[128];

        Direction direction;
    };

    uint8_t raw[CALIBRATION_DATA_LEN];
} calibration_data_t;


class CalibratedSensor: public Sensor{

public:
    // constructor of class with pointer to base class sensor and driver
    CalibratedSensor(Sensor& wrapped);
    ~CalibratedSensor();

    /*
    Override the update function
    */
    virtual void update() override;

    /**
    * Calibrate method computes the LUT for the correction
    * name is a unique name for the motor to be used in the EEPROM
    */
    virtual void calibrate(BLDCMotor& motor, const char* name);

    /**
     * Load calibration data from EEPROM
     * Returns true if data was loaded successfully
     * Returns data was not found or was otherwise invalid
     */
    virtual bool loadCalibrationData(BLDCMotor& motor, const char* name);

    // voltage to run the calibration: user input
    float voltage_calibration = 1;

protected:

    /**
    * getSenorAngle() method of CalibratedSensor class.
    * This should call getAngle() on the wrapped instance, and then apply the correction to
    * the value returned.
    */
    virtual float getSensorAngle() override;
    /**
    * init method of CaibratedSensor - call after calibration
    */
    virtual void init() override;
    /**
    * delegate instance of Sensor class
    */
    Sensor& _wrapped;

     // lut size, currently constan. Perhaps to be made variable by user?
    const int  n_lut { 128 } ;
    // create pointer for lut memory
    float* calibrationLut = new float[n_lut]();

    // Init inital angles
    float theta_actual { 0.0 };
    float elecAngle { 0.0 };
    float elec_angle { 0.0 };
    float theta_absolute_post { 0.0 };
    float theta_absolute_init { 0.0 };
    float theta_init { 0.0 };
    float avg_elec_angle { 0.0 };
};

#endif

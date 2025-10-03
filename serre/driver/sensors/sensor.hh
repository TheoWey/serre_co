#ifndef SENSOR_HH
#define SENSOR_HH

// Includes
#include "adc.h"
#include <cstdint>
/**
 * @namespace sensor
 * @brief Contains classes and methods for handling various sensors.
 */

namespace sensor {

/**
 * @class Sensor
 * @brief Abstract base class for sensor management.
 *
 * This class provides an interface for initializing, reading, and processing
 * data from sensors. It also includes a private configuration method and
 * allows for timeout configuration.
 *
 * @note Derived classes must implement the virtual methods to provide specific
 *       sensor functionality.
 */
class Sensor {
  public:
    virtual ~Sensor();

    virtual void init();
    virtual void readData();
    virtual void processData();

  protected:
    // Protected methods
    virtual void configure();
};

} // namespace sensor

#endif // SENSOR_HH
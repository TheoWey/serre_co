/**
 * @file sensor_manager.hh
 * @brief SensorManager class for handling sensor operations and coordination.
 *
 * This header defines the SensorManager singleton which centralizes ADC-driven
 * sensor read and processing flows for temperature and soil humidity sensors.
 *
 * @authors ThéoWey, ThybaltCarratala
 * @date 2024-06-10
 * @version 1.0
 */

#ifndef SENSOR_MANAGER_HH
#define SENSOR_MANAGER_HH

#include "../soil_hum_sensor/inc/soil_hum.hh"
#include "../temp_sensor/inc/temp_sensor.hh"

namespace sensor {

/**
 * @brief Maximum number of sensors managed per sensor type.
 *
 * This constant defines the fixed-size arrays used to store sensor pointers.
 */
constexpr uint8_t MAX_SENSORS = 5;

/**
 * @brief Subscribe a sensor pointer into a fixed-size array.
 *
 * Adds the provided sensor pointer into the first available nullptr slot of
 * the provided fixed-size array. No duplication or overflow error is
 * reported; callers should ensure space is available or extend this helper
 * to return a status if needed.
 *
 * @tparam T Sensor type (e.g., TempSensor or SoilHumSensor).
 * @param sensorArray Reference to the fixed-size array to populate.
 * @param sensor Pointer to the sensor to subscribe.
 */
template <typename T>
void SensorSubscribe(T *(&sensorArray)[MAX_SENSORS], T *sensor) {
    for (size_t i = 0; i < MAX_SENSORS; ++i) {
        if (sensorArray[i] == nullptr) {
            sensorArray[i] = sensor;
            return;
        }
    }
}

/**
 * @brief Apply a member operation to one or all sensors in an array.
 *
 * Calls the provided member function pointer on either the sensor at the
 * specified index or on every non-null sensor when index is >= MAX_SENSORS.
 * This helper is intended to be used with member-function pointers such as
 * &TempSensor::readData or &SoilHumSensor::processData.
 *
 * @tparam T Sensor type.
 * @tparam MemFn Type of member-function pointer (e.g. void (T::*)()).
 * @param sensorArray Reference to the fixed-size array of sensor pointers.
 * @param OpeFunc Member-function pointer to invoke on the sensor(s).
 * @param index Index of the sensor to operate on (0..MAX_SENSORS-1) or
 *              MAX_SENSORS to indicate "all".
 */
template <typename T, typename MemFn>
inline void SensorDataOperation(T *(&sensorArray)[MAX_SENSORS], MemFn OpeFunc,
                         uint8_t index) noexcept {
    // If index is out of range, treat as "broadcast" to all sensors.
    if (index >= MAX_SENSORS) {
        for (size_t i = 0; i < MAX_SENSORS; ++i) {
            if (sensorArray[i] != nullptr) {
                (sensorArray[i]->*OpeFunc)();
            }
        }
    } else {
        // Single sensor operation
        if (sensorArray[index] != nullptr) {
            (sensorArray[index]->*OpeFunc)();
        }
    }
}

class SensorManager {
  public:
    /**
     * @brief Retrieve the global SensorManager singleton instance.
     *
     * The instance is lazily created on first call and lives for the program
     * lifetime. Thread-safety depends on the environment; this is intended for
     * simple embedded usage.
     *
     * @return SensorManager& Reference to the singleton instance.
     */
    static SensorManager &getInstance();

    /**
     * @brief Destructor.
     *
     * Cleans up resources held by the manager. Does not delete sensor pointers
     * (ownership remains with the caller).
     */
    ~SensorManager();

    /**
     * @brief Subscribe a temperature sensor to the manager.
     *
     * The sensor is appended to the internal tempSensor array if space is
     * available. Duplicate checks are not performed.
     *
     * @param sensor Pointer to a TempSensor instance to subscribe.
     */
    void subscribeTempSensor(TempSensor *sensor);

    /**
     * @brief Subscribe a soil humidity sensor to the manager.
     *
     * @param sensor Pointer to a SoilHumSensor instance to subscribe.
     */
    void subscribeSoilHumSensor(SoilHumSensor *sensor);

    /**
     * @brief Start or request a read for a temperature sensor.
     *
     * If index is omitted or equals MAX_SENSORS, behavior is implementation
     * defined (e.g., initiate reads for all or the next available sensor).
     *
     * @param index Index of the sensor to read (0..MAX_SENSORS-1) or
     * MAX_SENSORS to indicate a default/all behavior.
     */
    void readTempData(uint8_t index = MAX_SENSORS);

    /**
     * @brief Start or request a read for a soil humidity sensor.
     *
     * @param index Index of the sensor to read or MAX_SENSORS for default/all.
     */
    void readSoilHumData(uint8_t index = MAX_SENSORS);

    /**
     * @brief Process temperature data for a sensor.
     *
     * Processing may convert raw ADC values to engineering units, filter data,
     * and forward results to higher-level logic.
     *
     * @param index Index of the sensor to process or MAX_SENSORS for
     * default/all.
     */
    void processTempData(uint8_t index = MAX_SENSORS);

    /**
     * @brief Process soil humidity data for a sensor.
     *
     * @param index Index of the sensor to process or MAX_SENSORS for
     * default/all.
     */
    void processSoilHumData(uint8_t index = MAX_SENSORS);

    /**
     * @brief Initiate reads for all registered sensors.
     *
     * This function typically triggers DMA/ADC conversions for every subscribed
     * sensor in both categories.
     */
    void readAllSensors();

    /**
     * @brief Process data for all registered sensors.
     *
     * Calls processing routines for every subscribed sensor in both categories.
     */
    void processAllSensors();

    /**
     * @brief Update all sensors by reading and processing their data.
     *
     * This is a convenience function that combines readAllSensors() and
     * processAllSensors().
     */
    void updateAllSensors();

  private:
    /**
     * @brief Private constructor for singleton pattern.
     *
     * Initializes internal state. Use getInstance() to obtain the singleton.
     */
    SensorManager();

    // Non-copyable, non-assignable
    SensorManager(const SensorManager &) = delete;
    SensorManager &operator=(const SensorManager &) = delete;

    /**
     * @brief Array of pointers to managed temperature sensors.
     *
     * Elements not in use are nullptr.
     */
    TempSensor *tempSensor[MAX_SENSORS] = {nullptr};

    /**
     * @brief Array of pointers to managed soil humidity sensors.
     *
     * Elements not in use are nullptr.
     */
    SoilHumSensor *soilHumSensor[MAX_SENSORS] = {nullptr};
};

} // namespace sensor

#endif // SENSOR_MANAGER_HH

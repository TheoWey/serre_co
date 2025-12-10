/**
 * @file sensor_manager.hh
 * @brief SensorManager class for handling sensor operations and coordination.
 * @author ThéoWey, ThybaltCarratala
 * @date 2024-06-10
 * @version 1.1
 *
 * This header defines the SensorManager singleton which centralizes ADC-driven
 * sensor read and processing flows for temperature and soil humidity sensors.
 */

#ifndef SENSOR_MANAGER_HH
#define SENSOR_MANAGER_HH

#include "../soil_hum_sensor/inc/soil_hum.hh"
#include "../temp_sensor/inc/temp_sensor.hh"

/**
 * @namespace driver
 * @brief Contains classes and methods for various drivers.
 */
namespace driver {
/**
 * @namespace sensor
 * @brief Contains classes and methods for handling various sensors.
 */
namespace sensor {

/**
 * @brief Maximum number of sensors managed per sensor type.
 *
 * This constant defines the fixed-size arrays used to store sensor pointers.
 * Applies to both temperature and soil humidity sensor arrays.
 */
constexpr size_t MAX_SENSORS = 5;

/**
 * @brief Initialize the sensor subsystem.
 *
 * Implemented as a weak function in sensor_manager.cc to allow
 * platform-specific initialization overrides.
 */
void init_sensors(void);

/**
 * @brief Subscribe a sensor pointer into a fixed-size array.
 *
 * Adds the provided sensor pointer into the first available nullptr slot of
 * the provided fixed-size array.
 *
 * @tparam T Sensor type (e.g., TempSensor or SoilHumSensor).
 * @param sensorArray Reference to the fixed-size array of pointers.
 * @param sensor Pointer to the sensor to subscribe.
 *
 * @note No duplication or overflow error is reported. Callers should ensure
 *       space is available or extend this helper to return a status if needed.
 */
template <typename T>
void SensorSubscribe(T *sensorArray[MAX_SENSORS], T *sensor) {
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
 *
 * @tparam T Sensor type.
 * @tparam MemFn Type of member-function pointer (e.g., void (T::*)()).
 * @param sensorArray Reference to the fixed-size array of sensor pointers.
 * @param OpeFunc Member-function pointer to invoke on the sensor(s).
 * @param index Index of the sensor to operate on (0..MAX_SENSORS-1) or
 *              MAX_SENSORS to indicate "all sensors".
 *
 * @note This helper is intended for use with member-function pointers such as
 *       &TempSensor::readData or &SoilHumSensor::processData.
 */
template <typename T, typename MemFn>
inline void SensorDataOperation(T *sensorArray[MAX_SENSORS], MemFn OpeFunc,
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
     * @brief Initialize the SensorManager (optional placeholder).
     *
     * Provided for consistency with other manager classes.
     * @return void
     */
    static void initialize();

    /**
     * @brief Retrieve the global SensorManager singleton instance.
     *
     * The instance is lazily created on first call and persists for the program
     * lifetime. Thread-safety depends on the execution environment; this is
     * intended for simple embedded usage.
     *
     * @return SensorManager& Reference to the singleton instance.
     */
    static SensorManager &getInstance();

    /**
     * @brief Destructor.
     *
     * Cleans up resources held by the manager. Does not delete sensor pointers
     * as ownership remains with the caller.
     */
    ~SensorManager() = default;

    /**
     * @brief Subscribe a temperature sensor to the manager.
     *
     * The sensor is appended to the internal tempSensor array if space is
     * available.
     *
     * @param sensor Pointer to a TempSensor instance to subscribe.
     * @return void
     *
     * @note Duplicate checks are not performed.
     */
    void subscribeTempSensor(TempSensor *sensor);

    /**
     * @brief Subscribe a soil humidity sensor to the manager.
     *
     * @param sensor Pointer to a SoilHumSensor instance to subscribe.
     * @return void
     */
    void subscribeSoilHumSensor(SoilHumSensor *sensor);

    /**
     * @brief Start or request a read for a temperature sensor.
     *
     * @param index Index of the sensor to read (0..MAX_SENSORS-1) or
     *              MAX_SENSORS to indicate default/all behavior.
     * @return void
     *
     * @note If index is omitted or equals MAX_SENSORS, the behavior is
     *       implementation-defined (e.g., initiate reads for all or next
     *       available sensor).
     */
    void readTempData(uint8_t index = MAX_SENSORS);

    /**
     * @brief Start or request a read for a soil humidity sensor.
     *
     * @param index Index of the sensor to read (0..MAX_SENSORS-1) or
     *              MAX_SENSORS for default/all behavior.
     * @return void
     */
    void readSoilHumData(uint8_t index = MAX_SENSORS);

    /**
     * @brief Process temperature data for a sensor.
     *
     * Processing may convert raw ADC values to engineering units, filter data,
     * and forward results to higher-level logic.
     *
     * @param index Index of the sensor to process (0..MAX_SENSORS-1) or
     *              MAX_SENSORS for default/all behavior.
     * @return void
     */
    void processTempData(uint8_t index = MAX_SENSORS);

    /**
     * @brief Process soil humidity data for a sensor.
     *
     * @param index Index of the sensor to process (0..MAX_SENSORS-1) or
     *              MAX_SENSORS for default/all behavior.
     * @return void
     */
    void processSoilHumData(uint8_t index = MAX_SENSORS);

    /**
     * @brief Calibrate a temperature sensor.
     *
     * @param index Index of the sensor to calibrate (0..MAX_SENSORS-1) or
     *              MAX_SENSORS to calibrate all temperature sensors.
     * @return void
     */
    void calibrateTempSensor(uint8_t index = MAX_SENSORS);

    /**
     * @brief Calibrate a soil humidity sensor.
     *
     * @param index Index of the sensor to calibrate (0..MAX_SENSORS-1) or
     *              MAX_SENSORS to calibrate all soil humidity sensors.
     * @return void
     */
    void calibrateSoilHumSensor(uint8_t index = MAX_SENSORS);

    /**
     * @brief Initiate reads for all registered sensors.
     *
     * This function typically triggers DMA/ADC conversions for every subscribed
     * sensor in both temperature and soil humidity categories.
     * @return void
     */
    void readAllSensors();

    /**
     * @brief Process data for all registered sensors.
     *
     * Calls processing routines for every subscribed sensor in both temperature
     * and soil humidity categories.
     * @return void
     */
    void processAllSensors();

    /**
     * @brief Update all sensors by reading and processing their data.
     *
     * Convenience function that combines readAllSensors() and
     * processAllSensors().
     * @return void
     */
    void updateAllSensors();

    /**
     * @brief Calibrate all registered sensors.
     *
     * Triggers calibration for all subscribed sensors in both temperature
     * and soil humidity categories.
     * @return void
     */
    void calibrateAllSensors();

    /**
     * @brief Get a temperature sensor by index.
     *
     * @param index Index of the sensor (0..MAX_SENSORS-1).
     * @return Pointer to TempSensor instance or nullptr if invalid index or
     *         uninitialized.
     */
    TempSensor *getTempSensor(uint8_t index);

    /**
     * @brief Get a soil humidity sensor by index.
     *
     * @param index Index of the sensor (0..MAX_SENSORS-1).
     * @return Pointer to SoilHumSensor instance or nullptr if invalid index or
     *         uninitialized.
     */
    SoilHumSensor *getSoilHumSensor(uint8_t index);

  private:
    /**
     * @brief Private constructor for singleton pattern.
     *
     * Initializes internal state. Use getInstance() to obtain the singleton.
     */
    SensorManager() = default;
    SensorManager(const SensorManager &) = delete;
    SensorManager &operator=(const SensorManager &) = delete;

    /**
     * @brief Array of pointers to managed temperature sensors.
     *
     * Elements not in use are nullptr.
     */
    TempSensor *tempSensor[MAX_SENSORS] = {nullptr, nullptr, nullptr, nullptr,
                                           nullptr};

    /**
     * @brief Array of pointers to managed soil humidity sensors.
     *
     * Elements not in use are nullptr.
     */
    SoilHumSensor *soilHumSensor[MAX_SENSORS] = {nullptr, nullptr, nullptr,
                                                 nullptr, nullptr};
};

} // namespace sensor
} // namespace driver

#endif // SENSOR_MANAGER_HH

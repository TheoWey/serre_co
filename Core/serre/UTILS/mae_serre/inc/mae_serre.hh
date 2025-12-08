#ifndef MAE_SERRE_HH
#define MAE_SERRE_HH

#include "stdint.h"

namespace utils {
namespace mae_serre {

enum class SerreMode : uint8_t { NORMAL, SETTING, STANDBY, ERROR, MAINTENANCE };

class SerreController {
  public:
    static SerreController &getInstance();

    void processLoop();

    void pwm_control_loop(void);

    void sensor_update_loop(void);

    inline SerreMode getCurrentMode() const {
        return currentMode_;
    }

    inline void setMode(SerreMode newMode) {
        currentMode_ = newMode;
    };

    inline void SetTemperatureSetpoint(uint16_t setpoint) {
        temperatureSetpoint_ = setpoint;
    }

    inline void SetTemperatureResetPoint(uint16_t resetPoint) {
        temperatureResetPoint_ = resetPoint;
    }

    inline void SetHumiditySetpoint(uint16_t setpoint) {
        humiditySetpoint_ = setpoint;
    }

    inline void SetHumidityResetPoint(uint16_t resetPoint) {
        humidityResetPoint_ = resetPoint;
    }

    inline uint16_t GetTemperatureSetpoint() const {
        return temperatureSetpoint_;
    }

    inline uint16_t GetTemperatureResetPoint() const {
        return temperatureResetPoint_;
    }

    inline uint16_t GetHumiditySetpoint() const {
        return humiditySetpoint_;
    }

    inline uint16_t GetHumidityResetPoint() const {
        return humidityResetPoint_;
    }

  private:
    SerreController() : currentMode_(SerreMode::NORMAL) {
    }
    SerreController(const SerreController &) = delete;
    SerreController &operator=(const SerreController &) = delete;

    SerreMode currentMode_;
    uint32_t lastSensorUpdateTick_ = 0;
    uint32_t lastPwmUpdateTick_ = 0;
    uint32_t sensorUpdatePeriodMs_ = 2000;
    uint32_t pwmUpdatePeriodMs_ = 3000;
    uint32_t lastSettingModeTick_ = 0;
    uint32_t settingModeTimeoutMs_ = 60000;

    uint16_t temperatureSetpoint_ = 25;
    uint16_t temperatureResetPoint_ = 20;
    uint16_t humiditySetpoint_ = 50;
    uint16_t humidityResetPoint_ = 40;
};

void pwm_control_loop(void);
void sensor_update_loop(void);

} // namespace mae_serre
} // namespace utils

#endif // MAE_SERRE_HH

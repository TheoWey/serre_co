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

    inline SerreMode getCurrentMode() const {
        return currentMode_;
    }

    inline void setMode(SerreMode newMode) {
        currentMode_ = newMode;
    };

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
};

void pwm_control_loop(void);
void sensor_update_loop(void);

} // namespace mae_serre
} // namespace utils

#endif // MAE_SERRE_HH

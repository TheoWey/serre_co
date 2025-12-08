#include "../inc/pwm_manager.hh"

namespace driver {
namespace pwm {

__WEAK void init_pwm(void) {
    // Weak implementation for user override
}

void PWMManager::initialize(pwm_handler_t *pwm_handlers, size_t num_channels) {
    PWMManager &instance = getInstance();
    for (size_t i = 0; i < num_channels && i < MAX_PWM_CHANNELS; ++i) {
        if (pwm_handlers == nullptr) {
            break;
        }
        // Store handler configuration by value
        PWM pwm_instance(pwm_handlers[i]);
        instance.pwm_instances[i] = pwm_instance;
        instance.initialized[i] = true;
        instance.active_channels_ = i + 1;
    }
}

PWMManager &PWMManager::getInstance() {
    static PWMManager instance;
    return instance;
}

void PWMManager::subscribePWM(const PWM pwm_instance) {
    for (size_t i = 0; i < MAX_PWM_CHANNELS; ++i) {
        if (!this->initialized[i]) {
            this->pwm_instances[i] = pwm_instance;
            this->initialized[i] = true;
            if (i + 1 > this->active_channels_) {
                this->active_channels_ = i + 1;
            }
            return;
        }
    }
}

void PWMManager::unsubscribePWM(size_t index) {
    if (index >= MAX_PWM_CHANNELS) {
        Error_Handler();
    }
    this->pwm_instances[index] = PWM({nullptr, 0});
    this->initialized[index] = false;
    // Update active_channels_ if needed
    if (index + 1 == this->active_channels_) {
        while (this->active_channels_ > 0 &&
               !this->initialized[this->active_channels_ - 1]) {
            --this->active_channels_;
        }
    }
}

PWM *PWMManager::getPWM(size_t index) {
    if (index < MAX_PWM_CHANNELS && this->initialized[index]) {
        return &this->pwm_instances[index];
    }
    return nullptr;
}

void PWMManager::setAllDutyCycles(float duty_ratio) {
    for (size_t i = 0; i < MAX_PWM_CHANNELS; ++i) {
        if (this->initialized[i]) {
            this->pwm_instances[i].setDutyCycle(duty_ratio);
        }
    }
}

void PWMManager::setDutyCycle(size_t index, float duty_ratio) {
    if (index < MAX_PWM_CHANNELS && this->initialized[index]) {
        this->pwm_instances[index].setDutyCycle(duty_ratio);
    }
}
void PWMManager::enableAll(bool on) {

    for (size_t i = 0; i < MAX_PWM_CHANNELS; ++i) {
        if (this->initialized[i]) {
            this->pwm_instances[i].enable(on);
        }
    }
}

void PWMManager::enable(size_t index, bool on) {
    if (index < MAX_PWM_CHANNELS && this->initialized[index]) {
        this->pwm_instances[index].enable(on);
    }
}

} // namespace pwm
} // namespace driver

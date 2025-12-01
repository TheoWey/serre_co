#include "../inc/pwm_manager.hh"

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
        instance.pwm_handlers[i] = pwm_handlers[i];
        instance.initialized[i] = true;
        instance.active_channels_ = i + 1;
    }
}

PWMManager &PWMManager::getInstance() {
    static PWMManager instance;
    return instance;
}

void PWMManager::subscribePWM(pwm_handler_t handler) {
    for (size_t i = 0; i < MAX_PWM_CHANNELS; ++i) {
        if (!this->initialized[i]) {
            this->pwm_handlers[i] = handler;
            this->initialized[i] = true;
            if (i + 1 > this->active_channels_) {
                this->active_channels_ = i + 1;
            }
            return;
        } else if (this->pwm_handlers[i].htim == handler.htim &&
                   this->pwm_handlers[i].channel == handler.channel) {
            this->pwm_handlers[i] = handler;
            return;
        }
    }
}

void PWMManager::unsubscribePWM(size_t index) {
    if (index >= MAX_PWM_CHANNELS) {
        Error_Handler();
    }
    this->pwm_handlers[index] = {nullptr, 0};
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
    // Note: This returns nullptr as storing PWM objects would require
    // dynamic allocation. Use setDutyCycle/enable methods directly instead.
    (void)index;
    return nullptr;
}

void PWMManager::setAllDutyCycles(float duty_ratio) {
    for (size_t i = 0; i < MAX_PWM_CHANNELS; ++i) {
        if (this->initialized[i]) {
            PWM pwm(this->pwm_handlers[i]);
            pwm.setDutyCycle(duty_ratio);
        }
    }
}

void PWMManager::setDutyCycle(size_t index, float duty_ratio) {
    if (index < MAX_PWM_CHANNELS && this->initialized[index]) {
        PWM pwm(this->pwm_handlers[index]);
        pwm.setDutyCycle(duty_ratio);
    }
}

void PWMManager::enableAll(bool on) {
    for (size_t i = 0; i < MAX_PWM_CHANNELS; ++i) {
        if (this->initialized[i]) {
            PWM pwm(this->pwm_handlers[i]);
            pwm.enable(on);
        }
    }
}

void PWMManager::enable(size_t index, bool on) {
    if (index < MAX_PWM_CHANNELS && this->initialized[index]) {
        PWM pwm(this->pwm_handlers[index]);
        pwm.enable(on);
    }
}

} // namespace pwm
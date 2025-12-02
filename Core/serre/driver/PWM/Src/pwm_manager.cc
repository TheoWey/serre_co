#include "../inc/pwm_manager.hh"

namespace pwm {

PWMManager::PWMManager() {
}

PWMManager::~PWMManager() {
}

PWMManager &PWMManager::getInstance() {
    static PWMManager instance;
    return instance;
}

void PWMManager::initialize(pwm_handler_t *pwm_handlers, size_t count) {
    PWMManager &instance = getInstance();
    for (size_t i = 0; i < count && i < MAX_PWM_CHANNELS; ++i) {
        if (pwm_handlers == nullptr) {
            break;
        }
        instance.pwms[i] = new PWM(pwm_handlers[i]);
    }
}

void PWMManager::setAllDutyCycles(float duty_ratio) {
    for (size_t i = 0; i < MAX_PWM_CHANNELS; ++i) {
        if (this->pwms[i] != nullptr) {
            this->pwms[i]->setDutyCycle(duty_ratio);
        }
    }
}

void PWMManager::setDutyCycle(size_t index, float duty_ratio) {
    if (index < MAX_PWM_CHANNELS && this->pwms[index] != nullptr) {
        this->pwms[index]->setDutyCycle(duty_ratio);
    }
}

void PWMManager::enableAll(bool on) {
    for (size_t i = 0; i < MAX_PWM_CHANNELS; ++i) {
        if (this->pwms[i] != nullptr) {
            this->pwms[i]->enable(on);
        }
    }
}

void PWMManager::enable(size_t index, bool on) {
    if (index < MAX_PWM_CHANNELS && this->pwms[index] != nullptr) {
        this->pwms[index]->enable(on);
    }
}

} // namespace pwm
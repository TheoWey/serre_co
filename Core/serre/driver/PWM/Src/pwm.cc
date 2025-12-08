#include "../inc/pwm.hh"

namespace driver {
namespace pwm {

PWM::PWM(pwm_handler_t pwm_handler) {
    this->m_handler = pwm_handler;
    this->m_period =
        this->m_handler.htim->Init.Period; // Cache the timer period
}

void PWM::setDutyCycle(uint8_t duty_ratio) {
    if (duty_ratio < 0) {
        duty_ratio = 0;
    } else if (duty_ratio > 100) {
        duty_ratio = 100;
    }
    this->m_duty = duty_ratio;

    uint32_t pulse = static_cast<uint32_t>(static_cast<float>(this->m_period) *
                                           this->m_duty / 100.0f);

    __HAL_TIM_SET_COMPARE(this->m_handler.htim, this->m_handler.channel, pulse);
}

void PWM::enable(bool on) {
    this->m_enabled = on;
    if (on) {
        HAL_TIM_PWM_Start(this->m_handler.htim, this->m_handler.channel);
    } else {
        HAL_TIM_PWM_Stop(this->m_handler.htim, this->m_handler.channel);
    }
}

} // namespace pwm
} // namespace driver

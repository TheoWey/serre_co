#include "../inc/debouncer.hh"

namespace utils {
namespace debouncer {

__WEAK void init_debouncer(void) {
    // Weak implementation for user override
}

void Debouncer::update() {
    GPIO_PinState current_state = HAL_GPIO_ReadPin(this->debouncer_handler.port,
                                                   this->debouncer_handler.pin);
    uint32_t current_tick = HAL_GetTick();
    uint32_t elapsed = current_tick - this->debouncer_handler.last_change;

    if (current_state != this->last_sampled_state_) {
        // State has changed; reset the timer
        this->debouncer_handler.last_change = current_tick;
        this->last_sampled_state_ = current_state;
        this->debouncer_handler.state = Button_Event_t::NONE;
    } else if ((elapsed >= this->debouncer_handler.debounce_time) && 
               (current_state == GPIO_PIN_SET)) {
        // State stable and pressed
        if (elapsed >= this->debouncer_handler.long_press_time) {
            this->debouncer_handler.state = Button_Event_t::LONG_PRESS;
        } else {
            this->debouncer_handler.state = Button_Event_t::SHORT_PRESS;
        }
    } else if (current_state == GPIO_PIN_RESET) {
        this->debouncer_handler.state = Button_Event_t::NONE;
    }
}

void DebouncerManager::initialize(DebouncerHandler *debouncer_handlers,
                                  size_t num_channels) {
    DebouncerManager &instance = getInstance();
    for (size_t i = 0; i < num_channels && i < DEBOUNCER_CHANNELS; ++i) {
        if (debouncer_handlers == nullptr) {
            break;
        }
        instance.debouncers[i] = Debouncer(debouncer_handlers[i]);
    }
}

DebouncerManager &DebouncerManager::getInstance() {
    static DebouncerManager instance;
    return instance;
}

void DebouncerManager::updateDebouncer(size_t index) {
    if (index >= DEBOUNCER_CHANNELS) {
        Error_Handler();
    }
    if (this->debouncers[index].getPort() != nullptr) {
        this->debouncers[index].update();
    }
}

} // namespace debouncer
} // namespace utils

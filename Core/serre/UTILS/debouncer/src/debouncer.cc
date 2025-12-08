#include "../inc/debouncer.hh"

namespace utils {
namespace debouncer {

__WEAK void init_debouncer(void) {
    // Weak implementation for user override
}

void Debouncer::update() {
    GPIO_PinState current_state = HAL_GPIO_ReadPin(this->debouncer_handler.port,
                                                   this->debouncer_handler.pin);
    uint32_t now = HAL_GetTick();
    static bool first = true;
    if (first) {
        this->last_sampled_state_ = current_state;
        first = false;
        return;
    }

    if (current_state != this->last_sampled_state_) {
        if (current_state == pressed) {
            this->debouncer_handler.last_change = now;
            this->debouncer_handler.state = Button_Event_t::NONE;
        } else if (current_state == released) {
            uint32_t press_duration = now - this->debouncer_handler.last_change;

            if (press_duration >= this->debouncer_handler.long_press_time) {
                this->debouncer_handler.state = Button_Event_t::LONG_PRESS;
            } else if (press_duration >=
                       this->debouncer_handler.debounce_time) {
                this->debouncer_handler.state = Button_Event_t::SHORT_PRESS;
            } else {
                this->debouncer_handler.state = Button_Event_t::NONE;
            }
        }
        this->last_sampled_state_ = current_state;
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
        instance.debouncers[i].clearState();
    }
}

DebouncerManager &DebouncerManager::getInstance() {
    static DebouncerManager instance;
    return instance;
}

void DebouncerManager::updateDebouncer(size_t index) {
    if (index == DEBOUNCER_CHANNELS) {
        for (size_t i = 0; i < DEBOUNCER_CHANNELS; ++i) {
            if (this->debouncers[i].getPort() != nullptr) {
                this->debouncers[i].update();
            }
        }
        return;
    } else {
        if (index >= DEBOUNCER_CHANNELS) {
            Error_Handler();
        } else {
            if (this->debouncers[index].getPort() != nullptr) {
                this->debouncers[index].update();
            }
        }
    }
}

} // namespace debouncer
} // namespace utils

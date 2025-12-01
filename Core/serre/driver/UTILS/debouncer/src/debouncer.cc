#include "../inc/debouncer.hh"

namespace utils {
namespace debouncer {

__WEAK void init_debouncer(void) {
    // Weak implementation for user override
}

void Debouncer::update() {
    GPIO_PinState current_state =
        HAL_GPIO_ReadPin(debouncer_handler.port, debouncer_handler.pin);
    uint32_t current_time = HAL_GetTick();

    if (current_state != debouncer_handler.state) {
        if ((current_time - debouncer_handler.last_change) >=
            debouncer_handler.sample_time) {
            debouncer_handler.state = current_state;
            debouncer_handler.last_change = current_time;
        }
    } else {
        debouncer_handler.last_change = current_time;
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

void DebouncerManager::updateAllDebouncers() {
    for (size_t i = 0; i < DEBOUNCER_CHANNELS; ++i) {
        if (this->debouncers[i].getPort() != nullptr) {
            this->debouncers[i].update();
        } else {
            break;
        }
    }
}

} // namespace debouncer
} // namespace utils

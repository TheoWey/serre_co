#include "../inc/menu_controler.hh"

#include "../../../driver/LCD/inc/lcd.hh"
#include "../../debouncer/inc/debouncer.hh"
#include "../../mae_serre/inc/mae_serre.hh"

using namespace utils::ui;

UIControler::UIControler()
    : currentPage_(&this->mainPage_), currentState_(MenuState::MAIN),
      editMode_(EditMode::NAVIGATING),
      controllerState_(ControllerState::NORMAL_MODE),
      lastSwitchTick_(HAL_GetTick()), switchPeriodMs_(5000),
      welcomeDurationMs_(5000) {
}

UIControler &UIControler::getInstance() {
    static UIControler instance;
    return instance;
}

void UIControler::init(bool showWelcomeScreen, uint32_t switchPeriodMs) {
    this->switchPeriodMs_ = switchPeriodMs;

    if (showWelcomeScreen) {
        this->controllerState_ = ControllerState::WELCOME_SCREEN;
        this->showWelcomeScreen();
        this->lastSwitchTick_ = HAL_GetTick();
    } else {
        this->controllerState_ = ControllerState::NORMAL_MODE;
        this->lastSwitchTick_ = HAL_GetTick();
        this->displayCurrentPage_();
    }
}

void UIControler::update() {
    uint32_t currentTick = HAL_GetTick();

    if (this->controllerState_ == ControllerState::WELCOME_SCREEN) {
        if ((currentTick - this->lastSwitchTick_) >= this->welcomeDurationMs_) {
            this->controllerState_ = ControllerState::NORMAL_MODE;
            this->lastSwitchTick_ = currentTick;
            this->displayCurrentPage_();
        }
        return;
    }

    this->pollButtons_();

    if ((currentTick - this->lastSwitchTick_) >= this->switchPeriodMs_) {
        this->lastSwitchTick_ = currentTick;

        if (this->currentState_ == MenuState::MAIN) {
            this->mainPage_.switchPhase();
        }

        this->displayCurrentPage_();
    }
}

void UIControler::pollButtons_() {
    auto &debouncerManager = utils::debouncer::DebouncerManager::getInstance();

    utils::debouncer::Debouncer *buttons[] = {
        debouncerManager.getDebouncer(bebouncer_channel_t::BUTTON_UP),
        debouncerManager.getDebouncer(bebouncer_channel_t::BUTTON_DOWN),
        debouncerManager.getDebouncer(bebouncer_channel_t::BUTTON_SELECT)};

    debouncerManager.updateAllDebouncers();

    if (buttons[bebouncer_channel_t::BUTTON_UP]->getState() ==
        utils::debouncer::Button_Event_t::SHORT_PRESS) {
        this->lastActionTick_ = HAL_GetTick();
        this->handleButtonUp_();
        buttons[bebouncer_channel_t::BUTTON_UP]->clearState();
    }
    if (buttons[bebouncer_channel_t::BUTTON_DOWN]->getState() ==
        utils::debouncer::Button_Event_t::SHORT_PRESS) {
        this->lastActionTick_ = HAL_GetTick();
        this->handleButtonDown_();
        buttons[bebouncer_channel_t::BUTTON_DOWN]->clearState();
    }
    if (buttons[bebouncer_channel_t::BUTTON_SELECT]->getState() ==
        utils::debouncer::Button_Event_t::SHORT_PRESS) {
        this->lastActionTick_ = HAL_GetTick();
        this->handleButtonSelect_();
        buttons[bebouncer_channel_t::BUTTON_SELECT]->clearState();
    }

    if (buttons[bebouncer_channel_t::BUTTON_SELECT]->getState() ==
        utils::debouncer::Button_Event_t::LONG_PRESS) {
        this->lastActionTick_ = HAL_GetTick();
        this->handleLongPressSelect_();
        buttons[bebouncer_channel_t::BUTTON_SELECT]->clearState();
    }
}

void UIControler::handleButtonUp_() {
    if (this->editMode_ == EditMode::NAVIGATING) {
        switch (this->currentState_) {
        case MenuState::SETTINGS:
            this->settingsPage_.onButtonUp();
            break;
        case MenuState::PWM_EDIT: {
            EditPage *editPage = this->getCurrentEditPage_();
            if (editPage)
                editPage->navigateSelection(-1);
            break;
        }
        case MenuState::SENSORS_PARAMETER:
            this->sensorTypeSelectPage_.onButtonUp();
            break;
        case MenuState::SENSOR_NUMBER_SELECT:
            this->sensorNumberSelectPage_.onButtonUp();
            break;
        case MenuState::TEMP_OFFSET_EDIT:
            this->tempOffsetEditPage_.onButtonUp();
            break;
        case MenuState::HUMIDITY_CALIB_EDIT:
            this->humidityCalibEditPage_.onButtonUp();
            break;
        case MenuState::SETPOINT_TYPE_SELECT:
            this->setpointTypeSelectPage_.onButtonUp();
            break;
        case MenuState::SETPOINT_MODE_SELECT:
            this->setpointModeSelectPage_.onButtonUp();
            break;
        case MenuState::SETPOINT_VALUE_EDIT:
            this->setpointValueEditPage_.onButtonUp();
            break;
        default:
            break;
        }
    } else if (this->editMode_ == EditMode::EDITING) {
        EditPage *editPage = this->getCurrentEditPage_();
        if (editPage)
            editPage->adjustValue(this->delta_m);
    }
    this->displayCurrentPage_();
}

void UIControler::handleButtonDown_() {
    if (this->editMode_ == EditMode::NAVIGATING) {
        switch (this->currentState_) {
        case MenuState::SETTINGS:
            this->settingsPage_.onButtonDown();
            break;
        case MenuState::PWM_EDIT: {
            EditPage *editPage = this->getCurrentEditPage_();
            if (editPage)
                editPage->navigateSelection(1);
            break;
        }
        case MenuState::SENSORS_PARAMETER:
            this->sensorTypeSelectPage_.onButtonDown();
            break;
        case MenuState::SENSOR_NUMBER_SELECT:
            this->sensorNumberSelectPage_.onButtonDown();
            break;
        case MenuState::TEMP_OFFSET_EDIT:
            this->tempOffsetEditPage_.onButtonDown();
            break;
        case MenuState::HUMIDITY_CALIB_EDIT:
            this->humidityCalibEditPage_.onButtonDown();
            break;
        case MenuState::SETPOINT_TYPE_SELECT:
            this->setpointTypeSelectPage_.onButtonDown();
            break;
        case MenuState::SETPOINT_MODE_SELECT:
            this->setpointModeSelectPage_.onButtonDown();
            break;
        case MenuState::SETPOINT_VALUE_EDIT:
            this->setpointValueEditPage_.onButtonDown();
            break;
        default:
            break;
        }
    } else if (this->editMode_ == EditMode::EDITING) {
        EditPage *editPage = this->getCurrentEditPage_();
        if (editPage)
            editPage->adjustValue(-(this->delta_m));
    }
    this->displayCurrentPage_();
}

void UIControler::handleButtonSelect_() {
    if (this->editMode_ == EditMode::NAVIGATING) {
        switch (this->currentState_) {
        case MenuState::SETTINGS:
            if (this->settingsPage_.getSelectedParameter() ==
                display_parameter_t::PARAMETER_ACTUATOR) {
                this->switchToPage_(MenuState::PWM_EDIT);
            } else if (this->settingsPage_.getSelectedParameter() ==
                       display_parameter_t::PARAMETER_SENSOR) {
                this->switchToPage_(MenuState::SENSORS_PARAMETER);
            } else if (this->settingsPage_.getSelectedParameter() ==
                       display_parameter_t::PARAMETER_SETPOINT) {
                this->switchToPage_(MenuState::SETPOINT_TYPE_SELECT);
            }
            break;
        case MenuState::PWM_EDIT:
            this->editMode_ = EditMode::EDITING;
            if (EditPage *editPage = this->getCurrentEditPage_())
                editPage->onEnterEditMode();
            this->displayCurrentPage_();
            break;
        case MenuState::SENSORS_PARAMETER:
            this->sensorNumberSelectPage_.setType(
                this->sensorTypeSelectPage_.getSelectedType());
            this->switchToPage_(MenuState::SENSOR_NUMBER_SELECT);
            break;
        case MenuState::SENSOR_NUMBER_SELECT: {
            auto type = this->sensorNumberSelectPage_.getSelectedType();
            auto idx = this->sensorNumberSelectPage_.getSelectedIndex();
            if (type == SensorType::TEMPERATURE) {
                this->tempOffsetEditPage_.setSensor(idx);
                this->switchToPage_(MenuState::TEMP_OFFSET_EDIT);
            } else {
                this->humidityCalibEditPage_.setSensor(idx);
                this->switchToPage_(MenuState::HUMIDITY_CALIB_EDIT);
            }
            break;
        }
        case MenuState::TEMP_OFFSET_EDIT:
            this->editMode_ = EditMode::EDITING;
            this->tempOffsetEditPage_.onEnterEditMode();
            this->displayCurrentPage_();
            break;
        case MenuState::HUMIDITY_CALIB_EDIT:
            this->editMode_ = EditMode::EDITING;
            this->humidityCalibEditPage_.onEnterEditMode();
            this->displayCurrentPage_();
            break;
        case MenuState::SETPOINT_TYPE_SELECT:
            this->setpointModeSelectPage_.setType(
                this->setpointTypeSelectPage_.getSelectedType());
            this->switchToPage_(MenuState::SETPOINT_MODE_SELECT);
            break;
        case MenuState::SETPOINT_MODE_SELECT:
            this->setpointValueEditPage_.setSensor(
                this->setpointModeSelectPage_.getSelectedType(),
                this->setpointModeSelectPage_.getSelectedMode());
            this->switchToPage_(MenuState::SETPOINT_VALUE_EDIT);
            break;
        case MenuState::SETPOINT_VALUE_EDIT:
            this->editMode_ = EditMode::EDITING;
            this->setpointValueEditPage_.onEnterEditMode();
            this->displayCurrentPage_();
            break;
        default:
            break;
        }
    } else if (this->editMode_ == EditMode::EDITING) {
        this->editMode_ = EditMode::NAVIGATING;
        EditPage *editPage = this->getCurrentEditPage_();
        if (editPage)
            editPage->onExitEditMode();
        this->displayCurrentPage_();
    }
}

void UIControler::handleLongPressSelect_() {
    switch (this->currentState_) {
    case MenuState::MAIN:
        this->switchToPage_(MenuState::SETTINGS);
        break;
    case MenuState::SETTINGS:
        this->switchToPage_(MenuState::MAIN);
        break;
    case MenuState::PWM_EDIT:
        if (this->editMode_ == EditMode::EDITING) {
            this->editMode_ = EditMode::NAVIGATING;
            EditPage *editPage = this->getCurrentEditPage_();
            if (editPage)
                editPage->onExitEditMode();
            this->switchToPage_(MenuState::SETTINGS);
        } else {
            this->switchToPage_(MenuState::SETTINGS);
        }
        break;
    case MenuState::SENSORS_PARAMETER:
        this->switchToPage_(MenuState::SETTINGS);
        break;
    case MenuState::SENSOR_NUMBER_SELECT:
        this->switchToPage_(MenuState::SENSORS_PARAMETER);
        break;
    case MenuState::TEMP_OFFSET_EDIT:
        if (this->editMode_ == EditMode::EDITING) {
            this->editMode_ = EditMode::NAVIGATING;
            this->tempOffsetEditPage_.onExitEditMode();
            this->switchToPage_(MenuState::SENSOR_NUMBER_SELECT);
        } else {
            this->switchToPage_(MenuState::SENSOR_NUMBER_SELECT);
        }
        break;
    case MenuState::HUMIDITY_CALIB_EDIT:
        if (this->editMode_ == EditMode::EDITING) {
            this->editMode_ = EditMode::NAVIGATING;
            this->humidityCalibEditPage_.onExitEditMode();
            this->switchToPage_(MenuState::SENSOR_NUMBER_SELECT);
        } else {
            this->switchToPage_(MenuState::SENSOR_NUMBER_SELECT);
        }
        break;
    case MenuState::SETPOINT_TYPE_SELECT:
        this->switchToPage_(MenuState::SETTINGS);
        break;
    case MenuState::SETPOINT_MODE_SELECT:
        this->switchToPage_(MenuState::SETPOINT_TYPE_SELECT);
        break;
    case MenuState::SETPOINT_VALUE_EDIT:
        if (this->editMode_ == EditMode::EDITING) {
            this->editMode_ = EditMode::NAVIGATING;
            this->setpointValueEditPage_.onExitEditMode();
            this->switchToPage_(MenuState::SETPOINT_MODE_SELECT);
        } else {
            this->switchToPage_(MenuState::SETPOINT_MODE_SELECT);
        }
        break;
    default:
        break;
    }
    this->displayCurrentPage_();
}

void UIControler::switchToPage_(MenuState newState) {
    this->currentState_ = newState;
    switch (newState) {
    case MenuState::MAIN:
        this->currentPage_ = &this->mainPage_;
        mae_serre::SerreController::getInstance().setMode(
            mae_serre::SerreMode::NORMAL);
        break;
    case MenuState::SETTINGS:
        this->currentPage_ = &this->settingsPage_;
        mae_serre::SerreController::getInstance().setMode(
            mae_serre::SerreMode::SETTING);
        break;
    case MenuState::PWM_EDIT:
        this->currentPage_ = &this->pwmEditPage_;
        break;
    case MenuState::SENSORS_PARAMETER:
        this->currentPage_ = &this->sensorTypeSelectPage_;
        break;
    case MenuState::SENSOR_NUMBER_SELECT:
        this->currentPage_ = &this->sensorNumberSelectPage_;
        break;
    case MenuState::TEMP_OFFSET_EDIT:
        this->currentPage_ = &this->tempOffsetEditPage_;
        break;
    case MenuState::HUMIDITY_CALIB_EDIT:
        this->currentPage_ = &this->humidityCalibEditPage_;
        break;
    case MenuState::SETPOINT_TYPE_SELECT:
        this->currentPage_ = &this->setpointTypeSelectPage_;
        break;
    case MenuState::SETPOINT_MODE_SELECT:
        this->currentPage_ = &this->setpointModeSelectPage_;
        break;
    case MenuState::SETPOINT_VALUE_EDIT:
        this->currentPage_ = &this->setpointValueEditPage_;
        break;
    default:
        break;
    }
    this->editMode_ = EditMode::NAVIGATING;
    this->displayCurrentPage_();
}

void UIControler::displayCurrentPage_() {
    this->currentPage_->display();
}

void UIControler::showWelcomeScreen() {
    driver::lcd::LCD::getInstance().lcd_clear();
    driver::lcd::LCD::getInstance().lcd_write_str("Welcome \nto SerreCo!");
}

EditPage *UIControler::getCurrentEditPage_() const {
    switch (this->currentState_) {
    case MenuState::PWM_EDIT:
        return const_cast<PWMEditPage *>(&this->pwmEditPage_);
    case MenuState::TEMP_OFFSET_EDIT:
        return const_cast<TempOffsetEditPage *>(&this->tempOffsetEditPage_);
    case MenuState::HUMIDITY_CALIB_EDIT:
        return const_cast<HumidityCalibEditPage *>(
            &this->humidityCalibEditPage_);
    case MenuState::SETPOINT_VALUE_EDIT:
        return const_cast<SetpointValueEditPage *>(
            &this->setpointValueEditPage_);
    default:
        return nullptr;
    }
}

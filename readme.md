<!-- filepath: a:\OneDrive - Association Cesi Viacesi mail\micro\stm32\serre_co\readme.md -->
# Serre Controller Project

## Project Overview
This project manages a greenhouse (serre) using an STM32 microcontroller. It controls actuators (PWM), reads sensors, and provides a menu-driven user interface for configuration and diagnostics.

## Cloning the Project

To get started, clone the repository using git:

```sh
git clone https://github.com/TheoWey/serre_co.git
cd serre_co
```

If you are using SSH:
```sh
git clone git@github.com:TheoWey/serre_co.git
cd serre_co
```

---

## Important Note on STM32 HAL Files
Some STM32 HAL (Hardware Abstraction Layer) files are not tracked in this repository. You must open the project at least once in STM32CubeMX or STM32CubeIDE and generate the code to create all necessary HAL files before building the project. This ensures all required source and configuration files are present for a successful build.

---

## Build Instructions
1. Open the project in STM32CubeIDE or your preferred STM32 development environment.
2. Ensure the correct MCU (STM32G031K8Tx) is selected.
3. Build the project using the provided `makefile` or IDE build system.
4. Flash the binary to the target board.

---

## Documentation

Complete Doxygen documentation is available in the `docs/` folder. Open `docs/html/index.html` in a web browser to browse the full API documentation.

### Generating Documentation
If you need to regenerate the documentation:
1. Install Doxygen on your system
2. Run `doxygen Doxyfile` from the project root
3. Documentation will be generated in the `docs/` folder

---

## Preliminary Usage Tutorial

### Startup Sequence
1. Power on the device.
2. The display shows "Welcome" for 5 seconds.
3. After 5 seconds, the main page is displayed (temperature/humidity or PWM phase).

### Navigation
- Use the UP/DOWN buttons to navigate between menu items or change display phase.
- Press SELECT for short actions (e.g., enter a submenu or confirm a choice).
- Long press SELECT to enter the settings menu.

### Settings Menu
The settings menu provides access to configuration options:
- **Hysteresis Settings**: Configure temperature and humidity setpoints with automatic constraint enforcement
  - Setpoint and Reset Point maintain a minimum 5-unit difference
  - When updating one value, the other is automatically adjusted if needed
- **PWM Configuration**: Adjust actuator PWM values
- **Sensor Parameters**: Configure sensor-specific settings

### Modes
- **NORMAL:** Default mode, sensors are read and actuators controlled.
- **SETTING:** Configuration mode; readings/updates are paused.
- **STANDBY:** Low power mode (to be implemented).
- **ERROR:** Error state (to be implemented).
- **MAINTENANCE:** Diagnostics/logs (to be implemented).

### Returning to NORMAL
- After a period of inactivity in SETTING, the system should automatically return to NORMAL (to be implemented).

---

## Code Formatting and Linting

This project includes `.clang-format` and `.clangd` files at the root. These files ensure consistent code style and provide advanced code completion and diagnostics (IntelliSense).

### How to Use
- **.clang-format**: Automatically formats your C/C++ code. Most IDEs (VSCode, CLion, STM32CubeIDE) can use this file for on-save formatting. You can also run:
  ```sh
  clang-format -i <file(s)>
  ```
- **.clangd**: Used by editors (like VSCode with the clangd extension) to provide code completion, navigation, and diagnostics. 
  
  **Important**: Before using clangd, you must update the `.clangd` configuration file with your own absolute paths. Open the `.clangd` file and replace all absolute paths (typically starting with `a:\OneDrive - Association Cesi Viacesi mail\micro\stm32\serre_co\`) with the absolute path to your local project directory. This ensures clangd can correctly locate include files and provide accurate IntelliSense.

### Coding Rules
This project follows a strict coding standard to ensure code consistency and readability:
- **File naming**: Use snake_case for all files (e.g., `sensor_manager.cc`, `menu_controler.hh`)
- **Class naming**: Use PascalCase for class names (e.g., `SensorManager`, `UIControler`)
- **Function/Method naming**: Use camelCase for functions and methods (e.g., `readSensor()`, `displayMenu()`)
- **Variable naming**: Use camelCase for local variables and snake_case for member variables with trailing underscore (e.g., `temperature_setpoint_`)
- **Constants**: Use UPPER_CASE for constants and macros (e.g., `MAX_TEMPERATURE`)
- **Documentation**: All public classes, methods, and functions must have Doxygen comments
- **Formatting**: The `.clang-format` file enforces the project's formatting rules automatically

For detailed coding standards and best practices, refer to the inline code examples and Doxygen documentation.

### Recommended Workflow
1. Edit your code following the project's coding rules.
2. Format with clang-format (automatically or manually).
3. Use your IDE's code navigation and diagnostics powered by clangd.
4. Commit only code that passes formatting and basic diagnostics.

---

## Features
- Sensor reading (temperature, humidity, soil moisture)
- Actuator control (fan, pump, lighting, etc.) via PWM
- Menu-based UI with navigation and settings
- Hysteresis control with automatic constraint enforcement
- Multiple operating modes: NORMAL, SETTING, STANDBY, ERROR, MAINTENANCE
- Modular code structure for easy extension
- Comprehensive Doxygen documentation

## Architecture

### Driver Layer (`Core/serre/driver/`)
- **ADC_MANAGER**: Analog-to-Digital Converter management
- **I2C**: I2C communication protocol implementation
- **LCD**: 16x2 LCD display driver
- **PWM**: Pulse Width Modulation control for actuators
- **sensors**: Sensor abstraction layer
  - Temperature sensor (DS18B20 or similar)
  - Soil humidity sensor
  - Sensor manager for centralized sensor handling

### Utils Layer (`Core/serre/UTILS/`)
- **circular_buffer**: Ring buffer implementation for data storage
- **debouncer**: Button debouncing logic
- **mae_serre**: Main greenhouse state machine (SerreController)
- **menu_controler**: UI navigation and menu system
  - Main page display
  - Settings pages (hysteresis, PWM, sensors)
  - Menu navigation logic

### HAL Layer (`Core/Inc/` and `Core/Src/`)
- STM32 HAL and hardware abstraction
- MCU initialization and configuration

### Drivers (`Drivers/`)
- CMSIS and STM32 HAL peripheral drivers

## File Structure
- `readme.md`: Project documentation
- `makefile`: Build instructions for command-line
- `Doxyfile`: Doxygen configuration file
- `docs/`: Generated documentation (HTML)
- `.clang-format`: Code formatting rules
- `.clangd`: Language server configuration (requires path customization)
- `Core/`: Main source code and headers
- `Drivers/`: MCU and peripheral drivers
- `Debug/`, `Release/`: Build output

---

## Releases
Pre-built releases and binaries are available at: [https://github.com/TheoWey/serre_co/releases](https://github.com/TheoWey/serre_co/releases)

---

## Implementation Plan

### Recently Implemented
- [x] Hysteresis settings with automatic constraint enforcement
- [x] Adaptive constraint logic based on which setpoint is updated
- [x] Comprehensive Doxygen documentation across all modules
- [x] Code refactoring for improved maintainability
- [x] Interface with UI to return to NORMAL mode after user inactivity
- [x] Pause sensor reading and actuator updates in SETTING mode
- [x] Define UI <-> SerreController interface for mode switching and timeouts

### Missing Features (to implement)
- [ ] Implement all SerreMode states:
    - [ ] STANDBY: minimal power consumption
    - [ ] ERROR: sensor failure, overheating, etc.
    - [ ] MAINTENANCE: diagnostics and logs
- [ ] Create missing UI menus/pages:
    - [ ] STANDBY menu/page
    - [ ] ERROR/ALARM menu/page
    - [ ] MAINTENANCE menu/page
- [ ] Specify triggers for each menu/page

### Integration Points
- Mode switching logic between UI and SerreController
- Timeout and user action handling for automatic return to NORMAL
- Error and maintenance menu accessibility

---

## Tutorials

### How the Project Works
- The main control loop (in `mae_serre`) manages the greenhouse state and mode (NORMAL, SETTING, etc.).
- The UI controller (`menu_controler`) handles user input, menu navigation, and mode switching.
- Sensor readings and actuator commands are updated in NORMAL mode; in SETTING mode, updates are paused for safe configuration.
- The display shows relevant information and allows configuration via buttons.
- Hysteresis control ensures setpoint and reset point maintain a minimum 5-unit difference, with automatic adjustment based on which value was modified.

### Adding a New Sensor
1. Create a new driver or interface for your sensor in `Core/serre/driver/sensors/` (or create a new subfolder).
2. Inherit from the `Sensor` base class defined in `Core/serre/driver/sensors/inc/sensor.hh`.
3. Implement required virtual methods (initialization, reading logic).
4. Register your sensor with the `SensorManager` in `Core/serre/driver/sensors/inc/sensor_manager.hh`.
5. Update the main control loop to call your sensor's read function and store the value.
6. If you want to display the sensor in the UI, update the relevant menu page (e.g., `main_page.hh/.cc`).
7. Add any configuration options to the settings menu if needed.
8. Document your sensor class with Doxygen comments following the project's coding rules.

### Adding a New Actuator
1. Create a new driver or interface for your actuator in `Core/serre/driver/`.
2. If using PWM, utilize the existing `PWM` and `PWMManager` classes.
3. Add initialization and control logic in your new file.
4. Update the control logic in `mae_serre` to command the actuator as needed (e.g., based on sensor values or user input).
5. If you want to control the actuator from the UI, update the relevant menu page or add a new PWM edit page.
6. Document your actuator class with Doxygen comments following the project's coding rules.

### Understanding the Menu System
The menu system is hierarchical and state-driven:
- `MenuPage` is the base class for all menu pages
- Navigation is handled by `UIControler` (singleton pattern)
- Each page implements display and input handling methods
- Settings pages support edit mode for value modification
- The system automatically handles constraint enforcement (e.g., hysteresis settings)

### Tips
- Follow the structure of existing drivers for consistency.
- Adhere to the project's coding rules and naming conventions.
- Update the menu state machine if you add new pages or modes.
- Test new hardware modules independently before integrating into the main project.
- Always add Doxygen comments for new classes and methods.
- Use the existing singleton pattern for manager classes.
- Refer to the generated Doxygen documentation for API details.

For more advanced customization, see the code comments and existing implementations in the `Core/serre/driver/` directory, or consult the Doxygen documentation in `docs/html/`.

---

## Team

This project is developed and maintained by the **GreenControl** team. For detailed information about the team structure and members, see [TEAM.md](TEAM.md).

---

## Contact
For questions or contributions, contact the project maintainer or open an issue on GitHub.
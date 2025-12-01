#ifndef LCD_HH
#define LCD_HH

/**
 * @file lcd.hh
 * @brief LCD Driver Header File
 * @author ThéoWey ThybaltCarratala
 * @date 2024-06-10
 * @version 1.0
 * @details
 * This header file defines the interface for the LCD driver, including
 * initialization and basic operations. It supports control of an HD44780-like
 * character LCD via either an I2C expander or directly via GPIO pins.
 *
 * The API is minimal and designed for embedded use with STM32 HAL types.
 */

#include <stdarg.h>

#include "../../I2C/inc/i2c_manager.hh"

namespace lcd {

/**
 * @brief LCD operating mode.
 */
typedef enum { LCD_MODE_I2C, LCD_MODE_GPIO } lcd_mode_t;

/**
 * @brief Simple GPIO pin descriptor.
 *
 * Describes one GPIO pin by port and pin mask/number.
 */
typedef struct {
    GPIO_TypeDef *data_port; /**< GPIO port (e.g. GPIOA) */
    uint16_t data_pin;       /**< GPIO pin mask (e.g. GPIO_PIN_0) */
} gpio_pin_handler_t;

/**
 * @brief GPIO pin configuration for 4-bit LCD mode.
 *
 * Contains the 4 data pins (D4..D7) and the RS control pin.
 */
typedef struct {
    gpio_pin_handler_t data_pins[4]; /**< Data pins D4..D7 */
    gpio_pin_handler_t rs;           /**< Register Select pin */
} lcd_gpio_config_t;

/**
 * @brief Top-level LCD handler configuration.
 *
 * Combines I2C configuration, selected mode and GPIO pin mapping when using
 * GPIO mode.
 */
typedef struct {
    i2c::I2CManager *i2c_handler; /**< I2C configuration (used in I2C mode) */
    uint8_t i2c_channel;          /**< I2C channel index */
    lcd_mode_t mode;              /**< Selected operating mode */
    lcd_gpio_config_t pin_config; /**< GPIO mapping (used in GPIO mode) */
} lcd_handler_t;

/**
 * @brief Initializes the LCD subsystem. Implemented as a weak function in
 * lcd.cc.
 */
void init_lcd(void);

static void int_to_string(char *buffer, int value, int base);

static void float_to_string(char *buffer, float value, int precision);

static void format_string(char *buffer, const char *format, va_list args);

/**
 * @brief Singleton class providing LCD operations.
 *
 * The class provides initialization and minimal primitives required to drive
 * the LCD in 4-bit mode. It is implemented as a singleton to simplify
 * global access from embedded code.
 */
class LCD {
  public:
    ~LCD() = default;

    /**
     * @brief Initialize the LCD driver with the provided handler.
     *
     * Copies the provided configuration into the driver and performs any
     * required hardware initialization (I2C checks or GPIO setup).
     *
     * @param handler Configuration for the LCD (mode, pins, I2C settings).
     *
     * @note Must be called before using getInstance() or any LCD operations.
     */
    static void initialize(lcd_handler_t handler);

    /**
     * @brief Access the singleton instance of the LCD class.
     *
     * If the instance does not exist it will be created (lazy initialization).
     *
     * @return Reference to the global LCD instance.
     *
     * @note initialize() must be called before first use of getInstance().
     */
    static LCD &getInstance();

    /**
     * @brief Select the configured mode and perform mode-specific setup.
     *
     * Chooses between I2C and GPIO implementations and performs any initial
     * checks (e.g. I2C device ready) or pin initialization required by the
     * selected mode.
     *
     * @note This function should be called after initialize() to activate the
     *       configured communication mode.
     */
    void lcd_select_mode();

    /**
     * @brief Initialize GPIO pins for GPIO mode.
     *
     * Configures the GPIO pins described in lcd_handler.pin_config for output
     * and ensures the LCD is left in a known state.
     *
     * @note Only meaningful when lcd_handler.mode == LCD_MODE_GPIO.
     */
    void lcd_gpio_init();

    /**
     * @brief Send a command byte to the LCD using GPIO.
     *
     * Commands are the control bytes defined by the LCD controller (clear,
     * set cursor, function set, etc.). This function performs the necessary
     * control toggles (RS low, send high nibble, send low nibble).
     *
     * @param cmd Command byte to send.
     */
    void lcd_gpio_command(uint8_t cmd);

    /**
     * @brief Send a 4-bit nibble to the LCD data pins (GPIO mode).
     *
     * The nibble should be right-aligned (bits 0..3) and will be presented on
     * the configured data pins before strobing the enable line.
     *
     * @param nibble Lower 4 bits are sent to D4..D7.
     *
     * @note This is a low-level function typically used by lcd_gpio_command()
     *       and lcd_gpio_write_char() for 4-bit protocol communication.
     */
    void lcd_gpio_send_4bits(uint8_t nibble);

    /**
     * @brief Send character data to the LCD using GPIO.
     *
     * Data bytes represent characters to be displayed on the LCD.
     * This function performs the necessary control toggles (RS high, send high
     * nibble, send low nibble).
     *
     * @param data Data byte (character) to send.
     *
     * @note The cursor automatically advances after each character according
     *       to the LCD's entry mode setting.
     */
    void lcd_write_char_gpio(uint8_t data);

    void lcd_write_char_i2c(uint8_t data);

    void lcd_write_char(uint8_t data);

    /**
     * @brief Send a NUL-terminated string to the LCD using GPIO mode.
     *
     * Sends each character in the provided NUL-terminated buffer to the LCD
     * using lcd_gpio_write_char(). The string is written sequentially starting
     * at the current cursor position.
     *
     * @param str Pointer to a NUL-terminated array of bytes (ASCII/extended
     *            characters). If nullptr the function should do nothing.
     *
     * @note This function is meaningful only when lcd_handler.mode ==
     *       LCD_MODE_GPIO. It does not perform automatic line wrapping beyond
     *       what the controller provides; callers should position the cursor
     *       appropriately (e.g. via lcd_goto_gpio()).
     */
    void lcd_write_str_gpio(const char *str);

    void lcd_write_str_i2c(const char *str);

    void lcd_write_str(const char *str, ...);
    void lcd_clear_gpio();

    void lcd_clear_i2c();

    void lcd_clear();

    /**
     * @brief Position the LCD cursor (GPIO mode).
     *
     * Sets the cursor position on a 16x2 LCD display by computing the DDRAM
     * address and sending the "set DDRAM address" command using
     * lcd_gpio_command().
     *
     * DDRAM address mapping for 16x2 displays:
     *  - row 0 -> 0x80 (line 1, address 0x00-0x0F)
     *  - row 1 -> 0xC0 (line 2, address 0x40-0x4F)
     *
     * @param row Zero-based row index (0 or 1).
     * @param col Zero-based column index (0..15).
     *
     * @note This implementation supports only 2-row displays. Values of row
     *       other than 0 are treated as row 1.
     */
    void lcd_goto_gpio(uint8_t row, uint8_t col);

    void lcd_goto_i2c(uint8_t row, uint8_t col);

    void lcd_goto(uint8_t row, uint8_t col);

  private:
    LCD();
    LCD(const LCD &) = delete;
    LCD &operator=(const LCD &) = delete;

    lcd_handler_t lcd_handler; /**< Active configuration for the LCD */
};
} // namespace lcd

#endif // LCD_HH
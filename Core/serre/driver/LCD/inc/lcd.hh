#ifndef SERRE_DRIVER_LCD_LCD_HH
#define SERRE_DRIVER_LCD_LCD_HH

#include "../../../../Inc/gpio.h"

/**
 * @file lcd.hh
 * @brief LCD driver interface wrapper.
 * @author TheoWey, ThibaltCarratala
 * @date 2023-10-05
 * @version 1.0
 *
 * Small, self-contained header that exposes a lightweight LCD wrapper type.
 * The actual low-level LCD handle is expected to be managed/initialized
 * elsewhere; this class stores the handler and exposes a simple API to
 * initialize, clear, position the cursor and write text.
 */

namespace lcd {

/**
 * @brief HAL-like LCD handle placeholder.
 *
 * The project previously declared a placeholder for the LCD hardware handle.
 * Keep the placeholder definition here so user code can pass a pointer to
 * whatever concrete handle/registers are used.
 */
typedef struct {
    // Placeholder for LCD hardware handle structure
} LCD_HandleTypeDef;

/**
 * @brief Simple LCD handler container.
 *
 * Holds a pointer to the LCD hardware handle. Concrete implementation of
 * the handle and initialization/start must be performed outside this class.
 */
typedef struct {
    LCD_HandleTypeDef *hlcd; ///< Pointer to the LCD hardware handle
} lcd_handler_t;

/**
 * @brief LCD control class.
 *
 * Represents a single LCD display instance. Provides methods to initialize
 * the display, clear it, set the cursor and write text. Backlight/output
 * enable state can be managed through enable()/disable().
 */
class LCD {
  public:
    /**
     * @brief Construct an LCD instance.
     * @param handler Handler describing the LCD hardware handle.
     *
     * The concrete hardware initialization (GPIO, clocks, low-level init)
     * must be done elsewhere; this class caches the handler for higher-level
     * operations.
     */
    LCD(lcd_handler_t handler);

    /**
     * @brief Destroy the LCD instance.
     */
    ~LCD() = default;

    /**
     * @brief Initialize the LCD.
     * @return true on success, false on failure.
     *
     * This function may perform higher-level init steps (clearing display,
     * setting default cursor mode). Low-level peripheral init is out of
     * scope and must be done by the caller.
     */
    bool init() noexcept;

    /**
     * @brief Clear display.
     */
    void clear() noexcept;

    /**
     * @brief Set cursor position.
     * @param row Row index (implementation-defined).
     * @param col Column index (implementation-defined).
     */
    void setCursor(size_t row, size_t col) noexcept;

    /**
     * @brief Write a null-terminated string at the current cursor.
     * @param text Null-terminated C string to write.
     */
    void write(const char *text) noexcept;

    /**
     * @brief Enable or disable the display/backlight/output.
     * @param on true to enable, false to disable.
     */
    void enable(bool on = true);

    /**
     * @brief Disable the display/backlight/output.
     */
    void disable() {
        this->enable(false);
    }

  private:
    lcd_handler_t m_handler; ///< LCD handler structure.
    size_t m_rows = 0; ///< Number of rows (cached / implementation-defined).
    size_t m_cols = 0; ///< Number of columns (cached / implementation-defined).
    bool m_enabled = false; ///< Display enabled state.
};

} // namespace lcd

#endif // SERRE_DRIVER_LCD_LCD_HH
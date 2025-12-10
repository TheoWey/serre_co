#include "../inc/lcd.hh"

namespace driver::lcd {

__WEAK void init_lcd(void) {
    // Weak implementation for user override
}
namespace {
/**
 * @brief Delay for a specified number of microseconds.
 * @param us Number of microseconds to delay.
 * @note This is a busy-wait loop and may not be accurate for long delays.
 * also this implementation is MCU dependent.
 */
constexpr uint32_t CYCLES_PER_US = 16; // Adjust based on MCU clock speed
inline void delay_us(uint32_t us) {
    us *= CYCLES_PER_US;
    while (us--) {
        __NOP();
    }
}
} // namespace
void int_to_string(char *buffer, size_t size, int value, int base) {
    if (size == 0)
        return;

    char *ptr = buffer;
    char *end = buffer + size - 1;

    if (value == 0) {
        *ptr++ = '0';
        *ptr = '\0';
        return;
    }

    bool is_negative = (value < 0 && base == 10);
    if (is_negative)
        value = -value;

    char temp[16];
    int idx = 0;
    while (value != 0 && idx < 15) {
        int digit = value % base;
        temp[idx++] = (digit < 10) ? (digit + '0') : (digit - 10 + 'a');
        value /= base;
    }

    if (is_negative && ptr < end)
        *ptr++ = '-';

    while (idx > 0 && ptr < end) {
        *ptr++ = temp[--idx];
    }
    *ptr = '\0';
}

void float_to_string(char *buffer, size_t size, float value, int precision) {
    if (size == 0)
        return;

    char *ptr = buffer;
    char *end = buffer + size - 1;

    bool is_negative = (value < 0.0f);
    if (is_negative) {
        value = -value;
        if (ptr < end)
            *ptr++ = '-';
    }

    float rounding = 0.5f;
    for (int i = 0; i < precision; i++)
        rounding /= 10.0f;
    value += rounding;

    int int_part = static_cast<int>(value);
    char temp[16];
    int_to_string(temp, sizeof(temp), int_part, 10);

    const char *t = temp;
    while (*t && ptr < end)
        *ptr++ = *t++;

    if (precision > 0 && ptr < end) {
        *ptr++ = '.';

        float frac = value - static_cast<float>(int_part);
        for (int i = 0; i < precision && ptr < end; i++) {
            frac *= 10.0f;
            int digit = static_cast<int>(frac);
            *ptr++ = static_cast<char>('0' + digit);
            frac -= static_cast<float>(digit);
        }
    }

    *ptr = '\0';
}

void format_string(char *buffer, size_t buffer_size, const char *format,
                   rc_t *rc, va_list args) {
    if (buffer_size == 0)
        return;

    char *buf_ptr = buffer;
    char *buf_end = buffer + buffer_size - 1;
    const char *fmt_ptr = format;
    rc->newline = false;
    rc->length = 0;

    while (*fmt_ptr && buf_ptr < buf_end) {
        if (*fmt_ptr == '%') {
            fmt_ptr++;
            switch (*fmt_ptr) {
            case 'd': {
                int val = va_arg(args, int);
                char tmp[16];
                int_to_string(tmp, sizeof(tmp), val, 10);
                for (char *p = tmp; *p && buf_ptr < buf_end; p++) {
                    *buf_ptr++ = *p;
                }
                break;
            }
            case 'u': {
                unsigned int val = va_arg(args, unsigned int);
                char tmp[16];
                int_to_string(tmp, sizeof(tmp), static_cast<int>(val), 10);
                for (char *p = tmp; *p && buf_ptr < buf_end; p++) {
                    *buf_ptr++ = *p;
                }
                break;
            }
            case 'x': {
                unsigned int val = va_arg(args, unsigned int);
                char tmp[16];
                int_to_string(tmp, sizeof(tmp), static_cast<int>(val), 16);
                for (char *p = tmp; *p && buf_ptr < buf_end; p++) {
                    *buf_ptr++ = *p;
                }
                break;
            }
            case 'c': {
                char c = static_cast<char>(va_arg(args, int));
                if (buf_ptr < buf_end)
                    *buf_ptr++ = c;
                break;
            }
            case 'f': {
                double val = va_arg(args, double);
                char tmp[32];
                float_to_string(tmp, sizeof(tmp), static_cast<float>(val), 2);
                for (char *p = tmp; *p && buf_ptr < buf_end; p++) {
                    *buf_ptr++ = *p;
                }
                break;
            }
            case '.': {
                fmt_ptr++;
                int precision = *fmt_ptr - '0';
                fmt_ptr++;
                if (*fmt_ptr == 'f') {
                    double val = va_arg(args, double);
                    char tmp[32];
                    float_to_string(tmp, sizeof(tmp), static_cast<float>(val),
                                    precision);
                    for (char *p = tmp; *p && buf_ptr < buf_end; p++) {
                        *buf_ptr++ = *p;
                    }
                }
                break;
            }
            case 's': {
                const char *str = va_arg(args, const char *);
                while (*str && buf_ptr < buf_end) {
                    *buf_ptr++ = *str++;
                }
                break;
            }
            case '%':
                if (buf_ptr < buf_end)
                    *buf_ptr++ = '%';
                break;
            default:
                if (buf_ptr < buf_end)
                    *buf_ptr++ = '%';
                if (*fmt_ptr && buf_ptr < buf_end)
                    *buf_ptr++ = *fmt_ptr;
                break;
            }
        } else {
            if (*fmt_ptr == '\n') {
                rc->newline = true;
                rc->length = static_cast<uint8_t>(buf_ptr - buffer);
            } else {
                *buf_ptr++ = *fmt_ptr;
            }
        }
        fmt_ptr++;
    }
    *buf_ptr = '\0';
}

LCD::LCD() {
}

void LCD::initialize(lcd_handler_t handler) {
    LCD &instance = getInstance();
    instance.lcd_handler = handler;
    instance.lcd_select_mode();
    instance.lcd_init();
}

LCD &LCD::getInstance() {
    static LCD instance;
    return instance;
}

void LCD::lcd_select_mode() {
    this->lcd_handler.mode = ping((this->lcd_handler.i2c_handler->getI2CHandler(
                                 this->lcd_handler.i2c_channel))) == HAL_OK
                                 ? LCD_MODE_I2C
                                 : LCD_MODE_GPIO;
}

void LCD::lcd_gpio_init() {
    HAL_GPIO_WritePin(this->lcd_handler.pin_config.rs.data_port,
                      this->lcd_handler.pin_config.rs.data_pin, GPIO_PIN_RESET);
    HAL_Delay(20);
    this->lcd_gpio_command(LCD_FUNCTION_SET);
    this->lcd_gpio_command(LCD_DISPLAY_ON);
    this->lcd_gpio_command(LCD_ENTRY_MODE);
    this->lcd_clear_gpio();
    HAL_Delay(2);
}

bool LCD::lcd_i2c_init() {
    HAL_Delay(50);

    if (!this->lcd_i2c_send_nibble(0x03))
        return false;
    HAL_Delay(5);

    if (!this->lcd_i2c_send_nibble(0x03))
        return false;
    HAL_Delay(1);

    if (!this->lcd_i2c_send_nibble(0x03))
        return false;
    HAL_Delay(1);

    if (!this->lcd_i2c_send_nibble(0x02))
        return false;
    HAL_Delay(1);

    if (!this->lcd_i2c_write_byte(LCD_FUNCTION_SET, false))
        return false;
    if (!this->lcd_i2c_write_byte(LCD_DISPLAY_ON, false))
        return false;
    if (!this->lcd_i2c_write_byte(LCD_ENTRY_MODE, false))
        return false;
    if (!this->lcd_i2c_write_byte(LCD_CLEAR_CMD, false))
        return false;
    HAL_Delay(2);

    return true;
}

bool LCD::lcd_init() {
    if (this->lcd_handler.mode == LCD_MODE_GPIO) {
        this->lcd_gpio_init();
        return true;
    } else {
        return this->lcd_i2c_init();
    }
}

void LCD::lcd_gpio_command(uint8_t cmd) {
    HAL_GPIO_WritePin(this->lcd_handler.pin_config.rs.data_port,
                      this->lcd_handler.pin_config.rs.data_pin, GPIO_PIN_RESET);

    this->lcd_gpio_send_4bits(cmd >> 4);
    this->lcd_gpio_send_4bits(cmd & 0x0F);
    HAL_Delay(2);
}

void LCD::lcd_gpio_send_4bits(uint8_t nibble) {
    for (int i = 0; i < 4; i++) {
        HAL_GPIO_WritePin(this->lcd_handler.pin_config.data_pins[i].data_port,
                          this->lcd_handler.pin_config.data_pins[i].data_pin,
                          (nibble & (1 << i)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
    this->lcd_gpio_toggle_enable();
}

bool LCD::lcd_i2c_write_byte(uint8_t data, bool is_data) {
    i2c::i2c_handler_t hi2c = this->lcd_handler.i2c_handler->getI2CHandler(
        this->lcd_handler.i2c_channel);

    uint8_t backlight = LCD_BL_BIT;
    uint8_t rs = is_data ? LCD_RS_BIT : 0;
    uint8_t nibble;

    nibble = (data & 0xF0) | backlight | rs | LCD_EN_BIT;
    if (i2c::i2c_write_helper(hi2c, &nibble, 1) != HAL_OK)
        return false;
    delay_us(1);

    nibble &= ~LCD_EN_BIT;
    if (i2c::i2c_write_helper(hi2c, &nibble, 1) != HAL_OK)
        return false;
    delay_us(50);

    nibble = ((data << 4) & 0xF0) | backlight | rs | LCD_EN_BIT;
    if (i2c::i2c_write_helper(hi2c, &nibble, 1) != HAL_OK)
        return false;
    delay_us(1);

    nibble &= ~LCD_EN_BIT;
    if (i2c::i2c_write_helper(hi2c, &nibble, 1) != HAL_OK)
        return false;
    delay_us(50);

    return true;
}

bool LCD::lcd_i2c_send_nibble(uint8_t nibble) {
    i2c::i2c_handler_t hi2c = this->lcd_handler.i2c_handler->getI2CHandler(
        this->lcd_handler.i2c_channel);

    uint8_t data = (nibble << 4) | LCD_BL_BIT | LCD_EN_BIT;
    if (i2c::i2c_write_helper(hi2c, &data, 1) != HAL_OK)
        return false;
    delay_us(1);

    data &= ~LCD_EN_BIT;
    if (i2c::i2c_write_helper(hi2c, &data, 1) != HAL_OK)
        return false;
    delay_us(50);

    return true;
}

bool LCD::lcd_write_char_gpio(uint8_t data) {
    if (this->lcd_handler.pin_config.rs.data_port == nullptr) {
        return false;
    }

    HAL_GPIO_WritePin(this->lcd_handler.pin_config.rs.data_port,
                      this->lcd_handler.pin_config.rs.data_pin, GPIO_PIN_SET);

    this->lcd_gpio_send_4bits(data >> 4);
    this->lcd_gpio_send_4bits(data & 0x0F);
    HAL_Delay(1);
    return true;
}

bool LCD::lcd_write_char_i2c(uint8_t data) {
    return this->lcd_i2c_write_byte(data, true);
}

bool LCD::lcd_write_char(uint8_t data) {
    if (this->lcd_handler.mode == LCD_MODE_GPIO) {
        return this->lcd_write_char_gpio(data);
    } else {
        return this->lcd_write_char_i2c(data);
    }
}

bool LCD::lcd_write_str(const char *str, ...) {
    char buffer[64];
    rc_t rc;
    va_list args;
    va_start(args, str);
    format_string(buffer, sizeof(buffer), str, &rc, args);
    va_end(args);

    char *p = buffer;
    bool line2_started = false;

    while (*p) {
        if (rc.newline && (p - buffer) == rc.length && !line2_started) {
            if (!this->lcd_goto(1, 0)) {
                return false;
            }
            line2_started = true;
        }

        if (!this->lcd_write_char(static_cast<uint8_t>(*p++))) {
            return false;
        }
    }
    return true;
}

void LCD::lcd_clear_gpio() {
    this->lcd_gpio_command(LCD_CLEAR_CMD);
}

bool LCD::lcd_clear_i2c() {
    bool result = this->lcd_i2c_write_byte(LCD_CLEAR_CMD, false);
    if (result) {
        HAL_Delay(2);
    }
    return result;
}

bool LCD::lcd_clear() {
    if (this->lcd_handler.mode == LCD_MODE_GPIO) {
        this->lcd_clear_gpio();
        return true;
    } else {
        return this->lcd_clear_i2c();
    }
}

void LCD::lcd_goto_gpio(uint8_t row, uint8_t col) {
    uint8_t address = (row == 0) ? LCD_LINE1_ADDR : LCD_LINE2_ADDR;
    address += col;
    this->lcd_gpio_command(address);
}

bool LCD::lcd_goto_i2c(uint8_t row, uint8_t col) {
    if (col >= LCD_MAX_COLS)
        return false;

    uint8_t address = (row == 0) ? LCD_LINE1_ADDR : LCD_LINE2_ADDR;
    address += col;
    return this->lcd_i2c_write_byte(address, false);
}

bool LCD::lcd_goto(uint8_t row, uint8_t col) {
    if (this->lcd_handler.mode == LCD_MODE_GPIO) {
        this->lcd_goto_gpio(row, col);
        return true;
    } else {
        return this->lcd_goto_i2c(row, col);
    }
}

void LCD::lcd_gpio_toggle_enable() {
    HAL_GPIO_WritePin(this->lcd_handler.pin_config.enable.data_port,
                      this->lcd_handler.pin_config.enable.data_pin,
                      GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(this->lcd_handler.pin_config.enable.data_port,
                      this->lcd_handler.pin_config.enable.data_pin,
                      GPIO_PIN_RESET);
    HAL_Delay(1);
}

} // namespace driver::lcd

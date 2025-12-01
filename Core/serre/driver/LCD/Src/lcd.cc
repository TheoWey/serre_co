#include "../inc/lcd.hh"

namespace lcd {

__WEAK void init_lcd(void) {
    // Weak implementation for user override
}

void int_to_string(char *buffer, int value, int base) {
    char *ptr = buffer, *ptr1 = buffer, tmp_char;
    int tmp_value;

    if (value == 0) {
        *ptr++ = '0';
        *ptr = '\0';
        return;
    }

    bool is_negative = false;
    if (value < 0 && base == 10) {
        is_negative = true;
        value = -value;
    }

    while (value != 0) {
        tmp_value = value % base;
        *ptr++ = (tmp_value < 10) ? (tmp_value + '0') : (tmp_value - 10 + 'a');
        value /= base;
    }

    if (is_negative) {
        *ptr++ = '-';
    }

    *ptr-- = '\0';

    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
}

void float_to_string(char *buffer, float value, int precision) {
    int int_part = static_cast<int>(value);
    float frac_part = value - static_cast<float>(int_part);
    if (value < 0 && int_part == 0) {
        *buffer++ = '-';
        frac_part = -frac_part;
    }
    int_to_string(buffer, int_part, 10);
    while (*buffer != '\0') {
        buffer++;
    }
    *buffer++ = '.';
    for (int i = 0; i < precision; i++) {
        frac_part *= 10.0f;
        int digit = static_cast<int>(frac_part);
        *buffer++ = static_cast<char>(digit + '0');
        frac_part -= static_cast<float>(digit);
    }
    *buffer = '\0';
}

void format_string(char *buffer, size_t buffer_size, const char *format,
                   va_list args) {
    if (buffer_size == 0) {
        return;
    }

    char *buf_ptr = buffer;
    char *buf_end = buffer + buffer_size - 1;
    const char *fmt_ptr = format;

    auto put_char = [&](char c) {
        if (buf_ptr < buf_end) {
            *buf_ptr++ = c;
        }
    };

    while (*fmt_ptr) {
        if (*fmt_ptr == '%') {
            fmt_ptr++;
            switch (*fmt_ptr) {
            case 'd': {
                int int_val = va_arg(args, int);
                char int_buffer[12];
                int_to_string(int_buffer, int_val, 10);
                for (char *p = int_buffer; *p && buf_ptr < buf_end; p++) {
                    *buf_ptr++ = *p;
                }
                break;
            }
            case 'f': {
                double float_val = va_arg(args, double);
                char float_buffer[32];
                float_to_string(float_buffer, static_cast<float>(float_val), 2);
                for (char *p = float_buffer; *p && buf_ptr < buf_end; p++) {
                    *buf_ptr++ = *p;
                }
                break;
            }
            case 's': {
                const char *str_val = va_arg(args, const char *);
                while (*str_val && buf_ptr < buf_end) {
                    *buf_ptr++ = *str_val++;
                }
                break;
            }
            default:
                put_char('%');
                if (*fmt_ptr) {
                    put_char(*fmt_ptr);
                }
                break;
            }
        } else {
            put_char(*fmt_ptr);
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
    if (instance.lcd_handler.mode == LCD_MODE_GPIO) {
        instance.lcd_gpio_init();
    }
}

LCD &LCD::getInstance() {
    static LCD instance;
    return instance;
}

void LCD::lcd_select_mode() {
    if (ping((this->lcd_handler.i2c_handler->getI2CHandler(
            this->lcd_handler.i2c_channel))) == HAL_OK) {
        this->lcd_handler.mode = LCD_MODE_I2C;
    } else {
        this->lcd_handler.mode = LCD_MODE_GPIO;
    }
}

void LCD::lcd_gpio_init() {

    HAL_GPIO_WritePin(this->lcd_handler.pin_config.rs.data_port,
                      this->lcd_handler.pin_config.rs.data_pin, GPIO_PIN_RESET);
    this->lcd_gpio_command(0x28); // Mode 4 bits, 2 lines, 5x8
    this->lcd_gpio_command(0x0C); // Display ON, cursor OFF
    this->lcd_gpio_command(0x06); // Increment cursor
    this->lcd_clear_gpio();
}

void LCD::lcd_gpio_command(uint8_t cmd) {
    HAL_GPIO_WritePin(this->lcd_handler.pin_config.rs.data_port,
                      this->lcd_handler.pin_config.rs.data_pin, GPIO_PIN_RESET);

    this->lcd_gpio_send_4bits(cmd >> 4);
    this->lcd_gpio_send_4bits(cmd & 0x0F);
}

void LCD::lcd_gpio_send_4bits(uint8_t nibble) {
    for (int i = 0; i < 4; i++) {
        HAL_GPIO_WritePin(this->lcd_handler.pin_config.data_pins[i].data_port,
                          this->lcd_handler.pin_config.data_pins[i].data_pin,
                          (nibble & (1 << i)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
}

void LCD::lcd_write_char_gpio(uint8_t data) {
    HAL_GPIO_WritePin(this->lcd_handler.pin_config.rs.data_port,
                      this->lcd_handler.pin_config.rs.data_pin, GPIO_PIN_SET);

    this->lcd_gpio_send_4bits(data >> 4);
    this->lcd_gpio_send_4bits(data & 0x0F);
}

void LCD::lcd_write_char_i2c(uint8_t data) {
    // Implementation for I2C character write goes here
    uint8_t data_array[2];
    data_array[0] = 0x40; // Control byte for data
    data_array[1] = data;
    i2c::i2c_write_helper(this->lcd_handler.i2c_handler->getI2CHandler(
                              this->lcd_handler.i2c_channel),
                          data_array, sizeof(data_array));
}

void LCD::lcd_write_char(uint8_t data) {
    if (this->lcd_handler.mode == LCD_MODE_GPIO) {
        this->lcd_write_char_gpio(data);
    } else {
        this->lcd_write_char_i2c(data);
    }
}

void LCD::lcd_write_str_gpio(const char *str) {
    while (*str) {
        this->lcd_write_char_gpio(*str++);
    }
}

void LCD::lcd_write_str_i2c(const char *str) {
    while (*str) {
        this->lcd_write_char_i2c(*str++);
    }
}

void LCD::lcd_write_str(const char *str, ...) {
    char buffer[32];
    va_list args;
    va_start(args, str);
    format_string(buffer, sizeof(buffer), reinterpret_cast<const char *>(str),
                  args);
    va_end(args);

    if (this->lcd_handler.mode == LCD_MODE_GPIO) {
        this->lcd_write_str_gpio(buffer);
    } else {
        this->lcd_write_str_i2c(buffer);
    }
}

void LCD::lcd_clear_gpio() {
    this->lcd_gpio_command(0x01); // Clear display command
}

void LCD::lcd_clear_i2c() {
    uint8_t command_array[2];
    command_array[0] = 0x00; // Control byte for command
    command_array[1] = 0x01; // Clear display command
    i2c::i2c_write_helper(this->lcd_handler.i2c_handler->getI2CHandler(
                              this->lcd_handler.i2c_channel),
                          command_array, sizeof(command_array));
}

void LCD::lcd_clear() {
    if (this->lcd_handler.mode == LCD_MODE_GPIO) {
        this->lcd_clear_gpio();
    } else {
        this->lcd_clear_i2c();
    }
}

void LCD::lcd_goto_gpio(uint8_t row, uint8_t col) {
    uint8_t address = (row == 0) ? 0x80 : 0xC0;
    address += col;
    this->lcd_gpio_command(address);
}

void LCD::lcd_goto_i2c(uint8_t row, uint8_t col) {
    uint8_t address = (row == 0) ? 0x80 : 0xC0;
    address += col;
    uint8_t command_array[2];
    command_array[0] = 0x00; // Control byte for command
    command_array[1] = address;
    i2c::i2c_write_helper(this->lcd_handler.i2c_handler->getI2CHandler(
                              this->lcd_handler.i2c_channel),
                          command_array, sizeof(command_array));
}

void LCD::lcd_goto(uint8_t row, uint8_t col) {
    if (this->lcd_handler.mode == LCD_MODE_GPIO) {
        this->lcd_goto_gpio(row, col);
    } else {
        this->lcd_goto_i2c(row, col);
    }
}

} // namespace lcd

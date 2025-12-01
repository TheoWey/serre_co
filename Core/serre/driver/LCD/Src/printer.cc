#include "../inc/printer.hh"

void print_welcome_message() {
    lcd::LCD &lcd = lcd::LCD::getInstance();
    lcd.lcd_clear();
    lcd.lcd_goto(0, 0);
    lcd.lcd_write_str("Welcome to Serre!");
}

void print_goodbye_message() {
    lcd::LCD &lcd = lcd::LCD::getInstance();
    lcd.lcd_clear();
    lcd.lcd_goto(0, 0);
    lcd.lcd_write_str("Goodbye!");
}

void print_status(float temperature, float soilHumidity) {
    lcd::LCD &lcd = lcd::LCD::getInstance();
    lcd.lcd_clear();
    lcd.lcd_goto(0, 0);
    lcd.lcd_write_str("Temp: %a°C", temperature);
    lcd.lcd_goto(1, 0);
    lcd.lcd_write_str("Soil: %a%", soilHumidity);
}

void print_error(const char *errorMessage) {
    lcd::LCD &lcd = lcd::LCD::getInstance();
    lcd.lcd_clear();
    lcd.lcd_goto(0, 0);
    lcd.lcd_write_str("Error:");
    lcd.lcd_goto(1, 0);
    lcd.lcd_write_str("%s", errorMessage);
}

void print_calibration_message() {
    lcd::LCD &lcd = lcd::LCD::getInstance();
    lcd.lcd_clear();
    lcd.lcd_goto(0, 0);
    lcd.lcd_write_str("Calibrating...");
}

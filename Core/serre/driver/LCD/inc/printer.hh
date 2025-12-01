#ifndef PRINTER_HH
#define PRINTER_HH
/**
 * @file printer.hh
 * @brief Printer Driver Header File
 * @author ThéoWey ThybaltCarratala
 * @date 2024-06-10
 * @version 1.0
 * @details
 * This header file defines the function for printing the system status using
 * lcd.hh.
 */

#include "lcd.hh"

void print_welcome_message();

void print_goodbye_message();

void print_error(const char *errorMessage);

void print_status(float temperature, float soilHumidity);

#endif // PRINTER_HH
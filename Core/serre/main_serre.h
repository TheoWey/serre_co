#ifndef MAIN_SERRE_H
#define MAIN_SERRE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @file main_serre.h
 * @brief Main interface for the Serre application.
 * @author ThéoWey, ThybaltCarratala
 * @date 2024-06-10
 * @version 1.1
 *
 * This header declares functions and types for the main control
 * logic of the Serre application, including initialization and
 * main execution loop.
 */

/**
 * @brief Enumeration for sensor channels.
 *
 * Defines the available sensor channels for the application.
 */
enum sensor_channel_t { HUM_0, TEMP_0 };

/**
 * @brief Main function for the Serre application.
 *
 * This function contains the main execution loop and logic for
 * the Serre application.
 */
void main_serre(void);

/**
 * @brief Initialize the Serre application.
 *
 * This function sets up the necessary components and prepares
 * the application for execution.
 */
void main_serre_init(void);

#ifdef __cplusplus
}
#endif

#endif // MAIN_SERRE_H

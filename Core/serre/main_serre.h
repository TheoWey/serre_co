#ifndef MAIN_SERRE_H
#define MAIN_SERRE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

enum pwm_channel_t { FAN, PUMP };

enum hum_channel_t { SOIL_HUMIDITY_0 };

enum temp_channel_t { TEMPERATURE_0 };

enum i2c_channel_t { I2C_CHANNEL_0 };
// Function prototypes
void main_serre(void);
void main_serre_init(void);

#ifdef __cplusplus
}
#endif

#endif // MAIN_SERRE_H
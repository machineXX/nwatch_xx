#ifndef WEATHER_H_
#define WEATHER_H_
#include "common.h"

#define WEATHER_NUM 5

void weather_type_set(usb_ttl_weather_t *weather);
bool weather_forecast(void);
#endif /* WATCHFACE_H_ */

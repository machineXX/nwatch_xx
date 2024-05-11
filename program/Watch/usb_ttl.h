
#ifndef USB_TTL_H_
#define USB_TTL_H_
#include "common.h"

typedef struct weather_data
{
    char *weather;
    u8 weather_type;
    int temp_low;
    int temp_high;
}usb_ttl_weather_t;

typedef struct usb_ttl_data
{
    u8 cpu;
    u8 member;
    u8 temp;
    int netspeed;
    u8 tdp;
    usb_ttl_weather_t weather[5];
    /* data */
}usb_ttl_data_t;

int  usb_ttl_connected(void);
void usb_ttl_init(void);
void usb_ttl_exit(void);
void usb_ttl_update(void);
int put_cpu(u8 i);
usb_ttl_weather_t get_weather(u8 num);

#endif

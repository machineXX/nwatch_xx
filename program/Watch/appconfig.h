/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#ifndef APPCONFIG_H_
#define APPCONFIG_H_
#include "typedefs.h"

extern appconfig_s appConfig;
u8 flash_init(void);
void appconfig_init(void);
void appconfig_save(void);
void appconfig_reset(void);
void Save_Flash_TIME_Data(void);
unsigned char Read_Flash_Time_Data(void);
unsigned char Read_Flash_Time_Data_test(void);
void watchface_num_set(u8 choice);
#endif /* APPCONFIG_H_ */

/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#ifndef WATCHFACE_H_
#define WATCHFACE_H_

#include "typedefs.h"
void cube_init(void);
void free_OLED_GRAM(void);
void watchfaceOpen(void);
void show_cube(unsigned char way);
void watchface_drawDate(void);
display_t watchface_ticker(void);
#endif /* WATCHFACE_H_ */

/*
   Project: N|Watch
   Author: Zak Kemble, contact@zakkemble.co.uk
   Copyright: (C) 2013 by Zak Kemble
   License: GNU GPL v3 (see License.txt)
   Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
*/

#include "common.h"
#include "sys.h"
#include "led.h"
#include "key.h"

#define BTN_IS_PRESSED	3
#define BTN_NOT_PRESSED	3




/*下面的方式是通过直接操作库函数方式读取IO*/
//#define KEY_UP      GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) 
//#define KEY0        GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) 
//#define KEY1        GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3) 
//#define KEY2        GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) 



/*下面方式是通过位带操作方式读取IO*/
/*
#define KEY0 		PEin(4)   	//PE4
#define KEY1 		PEin(3)		//PE3 
#define KEY2 		PEin(2)		//P32
#define WK_UP 	PAin(0)		//PA0
*/
typedef struct {
  button_f onPress_l_fun;
  byte time_secs;	
}bpress_long;


typedef struct {
  millis_t pressedTime;	// Time of press
  bool processed;			// Time of press has been stored (don't store again until next press)
  byte counter;			// Debounce counter
  bool funcDone;			// Function has been ran (don't run again until next press)
  button_f onPress;		// Function to run when pressed typedef bool (*button_f)(void);
  bpress_long onPress_l;
  const uint* tune;		// Tune to play when pressed
} s_button;

static s_button buttons[BTN_COUNT];
static void processButtons(u8 user_state);
static void processButton(s_button*, BOOL);
static byte bitCount(byte);
void buttons_press_longtime(s_button* button);
//EMPTY_INTERRUPT(PCINT0_vect);

void buttons_init()
{
  buttons_startup();
  memset(buttons, 0 ,sizeof(buttons));
  // Assign tunes
  buttons[BTN_1].tune = tuneBtn1;
  buttons[BTN_2].tune = tuneBtn2;
  buttons[BTN_3].tune = tuneBtn3;

  buttons_setFunc_long(BTN_4,close_screen,2);
}

void buttons_update(u8 user_state)
{
  static millis8_t lastUpdate;
  // Update every 10ms
  millis8_t now = millis();
  if ((millis8_t)(now - lastUpdate) >= 20)
  {
    lastUpdate = now;
    processButtons(user_state);
  }
}

//按键初始化函数
// Sets button pins to INPUT with PULLUP
void buttons_startup()
{
	KEY_Init();
}

// Sets button pins to OUTPUT LOW
// This stops them wasting current through the pull-up resistor when pressed
void buttons_shutdown()
{
	//配合休眠sleep函数开启后关闭按键，不会做
	
//  pinPullup(BTN_1_P,	PU_DIS);
//  pinPullup(BTN_2_P,	PU_DIS);
//  pinPullup(BTN_3_P,	PU_DIS);

//  pinMode(BTN_1_P,	OUTPUT);
//  pinMode(BTN_2_P,	OUTPUT);
//  pinMode(BTN_3_P,	OUTPUT);
}

static void processButtons(u8 user_state)
{
  u8 i,BTN_NUMBER=4;
  // Get button pressed states
  BOOL isPressed[BTN_COUNT];
  isPressed[BTN_1] = KEY1;
  isPressed[BTN_2] = KEY2;
  isPressed[BTN_3] = KEY0;
  isPressed[BTN_4] = KEY3;
	
	// printf("%d,%d,%d,%d\r\n",KEY1,KEY2,KEY0,KEY3);
	
  // Process each button
  // LOOPR(BTN_COUNT, i)
  if(user_state == 0)
  {
    processButton(&buttons[BTN_NUMBER-1], !isPressed[BTN_NUMBER-1]);
    return ;
  }
  for(i=0;i<BTN_NUMBER;i++)
    processButton(&buttons[i], !isPressed[i]);
}

static void processButton(s_button* button, BOOL isPressed)
{
  button->counter <<= 1;
  if (isPressed)
  {
    // Set debounce counter bit
    button->counter |= 1;

    //Are enough bits set to count as pressed?
    if (bitCount(button->counter) >= BTN_IS_PRESSED)
    {
      // Store time of press
      if (!button->processed)
      {
        button->pressedTime = timeDate.time.secs;
        button->processed = true;
      }

      // Run function
      if (!button->funcDone && button->onPress != NULL && button->onPress())
      {
        button->funcDone = true;	
        tune_play(button->tune, VOL_UI, PRIO_UI); //蜂鸣器有关

        led_flash(LED_GREEN, LED_FLASH_FAST, LED_BRIGHTNESS_MAX);
      }
    }

    //press long time
    if(button->onPress_l.onPress_l_fun && button->onPress_l.time_secs)
    {
      buttons_press_longtime(button);
    }
  }
  else // Not pressed
  {
    // Has button been not pressed for long enough?
    if (bitCount(button->counter) <= BTN_NOT_PRESSED)
    {
      button->processed = false;
      button->funcDone = false;
      button->pressedTime = 0;
    }
  }
}

// Count set bits in value
static byte bitCount(byte val)
{
  byte count = 0;
  for (; val; val >>= 1)
    count += val & 1;
  return count;
}

// Set new function to run when button is pressed and return the old function
button_f buttons_setFunc(btn_t btn, button_f func)
{
  button_f old = buttons[btn].onPress;
  buttons[btn].onPress = func;
  return old;
}

// Set functions to run for each button
void buttons_setFuncs(button_f btn1, button_f btn2, button_f btn3)
{
  buttons[BTN_1].onPress = btn1;
  buttons[BTN_2].onPress = btn2;
  buttons[BTN_3].onPress = btn3;
}
void buttons_setFuncs_add4(button_f btn4)
{
  buttons[BTN_4].onPress = btn4;
}

// Set new function to run when button is pressed and return the old function
void buttons_setFunc_long(btn_t btn, button_f func,byte time_s)
{
  buttons[btn].onPress_l.onPress_l_fun = func;
  buttons[btn].onPress_l.time_secs = time_s;
}



  // Get how long a button has been pressed for
millis_t buttons_pressTime(s_button* button) // set max press time to 1 min!!!
{
  // s_button* button = &buttons[btn];
  int secs_dif = 0;
  if(bitCount(button->counter) <= BTN_NOT_PRESSED)
    return 0;
  //printf("b = %d\r\n",bitCount(button->counter) );
  secs_dif = timeDate.time.secs - button->pressedTime;
  if(secs_dif < 0)
  {
    secs_dif += 60; 
  }
  return secs_dif;
  //return 1;
}

void buttons_press_longtime(s_button* button)
{
  millis_t buttons_time = 0; 
	static u8 last_time = 0;

	buttons_time = buttons_pressTime(button);
	if(buttons_time >= button->onPress_l.time_secs && timeDate.time.secs != last_time)
	{
    last_time = timeDate.time.secs;
	  button->onPress_l.onPress_l_fun();
	}
}

// See if a button has been pressed in the past x milliseconds
bool buttons_isActive()
{
//  // If sleep has been disabled then just say that the buttons are always active
  if (!appConfig.sleepTimeout)
    return true;

//  // Get timeout val in ms
  uint timeout = (appConfig.sleepTimeout * 5) * 1000*3;
//  uint timeout =  1000;

  // See if a button has been pressed within that timeout
  LOOPR(BTN_COUNT, i)
  {
    if (millis() - buttons[i].pressedTime < timeout)
      return true;
  }

  return false;
}

// Set button status to pressed, processed etc but don't run their functions
void buttons_wake()
{
  LOOPR(BTN_COUNT, i)
  {
    buttons[i].funcDone		= true;
    buttons[i].processed	= true;
    buttons[i].counter		= BTN_IS_PRESSED;
    buttons[i].pressedTime	= millis();
  }
}

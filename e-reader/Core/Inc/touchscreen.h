#ifndef TOUCHSCREEN_H
#define TOUCHSCREEN_H

#include "cmsis_os.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"
#include "stdlib.h"

typedef struct
{
	int32_t x;
	int32_t y;
	TickType_t t;
} ThumbInfo;

extern ThumbInfo lastThumbState;
extern ThumbInfo actualThumbState;

typedef enum
{
	No_ee,
	ThumbDown,
	Pressing,
	ThumbUp,
} TouchscreenElementalEvent;

typedef enum
{
	No_e,
	Click,
	Move,
	ZoomIn,
	ZoomOut
} TouchscreenEvent;

TouchscreenEvent getTouchscreenEvent(void);
TouchscreenElementalEvent getTouchscreenElementalEvent(void);
uint8_t isClick(ThumbInfo down, ThumbInfo up);
uint8_t isDoubleClick(ThumbInfo first, ThumbInfo second);

#endif // TOUCHSCREEN_H

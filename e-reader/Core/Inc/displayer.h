#ifndef DISPLAYER_H
#define DISPLAYER_H

#include "cmsis_os.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"
#include "touchscreen.h"

typedef enum Perspective_t
{
	None,
	Splash,
	Bookshelf,
	Browse,
	Reader
} Perspective;

extern SemaphoreHandle_t semaphore_ui;
extern void (**uiDisplayHandlers)(void);
extern void (*uiDisplayHandler)(void);
extern void (**uiLogicHandlers)(void);
extern void (*uiLogicHandler)(void);

void initScreen(void);
void displayerHandler(void);

void setActualPerspective(uint8_t perspective_id);

#endif // DISPLAYER_H

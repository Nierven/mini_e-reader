#ifndef DISPLAYER_H
#define DISPLAYER_H

#include "cmsis_os.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"

void initScreen(void);
void displayerHandler(void);

#endif // DISPLAYER_H

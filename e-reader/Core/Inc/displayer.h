#ifndef DISPLAYER_H
#define DISPLAYER_H

#include "cmsis_os.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"

#define BOOK_MARGIN 20

void initScreen(void);
void displayerHandler(void);

#endif // DISPLAYER_H

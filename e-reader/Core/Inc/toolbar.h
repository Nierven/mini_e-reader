#ifndef TOOLBAR_H
#define TOOLBAR_H

#include "cmsis_os.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"
#include "ui_elements.h"
#include <string.h>

#define TITLEBAR_HPADDING 13
#define TITLEBAR_VPADDING 9
#define TITLEBAR_RADIUS 10
#define TITLEBAR_FONT Font16
#define TITLEBAR_SUBFONT Font12

typedef struct Toolbar_t
{
	uint8_t size;
	int32_t margin;
	int32_t padding;
	int32_t radius;

	int32_t buttonWidth;
	int32_t buttonHeight;
	Button **buttons;

	uint8_t isVisible;

	int32_t x;
	int32_t y;
	int32_t w;
	int32_t h;
} Toolbar;

void drawToolbar(Toolbar *toolbar);
void toolbar_OnHover(Toolbar *toolbar, int32_t x, int32_t y);
void toolbar_OnClick(Toolbar *toolbar, int32_t x, int32_t y);

#endif // TOOLBAR_H

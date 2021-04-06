#ifndef TOOLBAR_H
#define TOOLBAR_H

#include "cmsis_os.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"
#include <string.h>

#define TOOLBAR_HOVER_BOX_MARGIN 2

#define TOOLTIP_MARGIN 25
#define TOOLTIP_HPADDING 13
#define TOOLTIP_VPADDING 9
#define TOOLTIP_FONT Font16
#define TOOLTIP_RADIUS 10

#define TITLEBAR_HPADDING 13
#define TITLEBAR_VPADDING 9
#define TITLEBAR_RADIUS 10
#define TITLEBAR_FONT Font16
#define TITLEBAR_SUBFONT Font12

typedef struct ToolbarButton_t
{
	int32_t x;
	int32_t y;

	uint8_t isEnabled;
	uint8_t isHovered;
	char tooltip[25];
	uint8_t *icon;

	void (*onClickCallback)(struct ToolbarButton_t *button);
} ToolbarButton;

typedef struct Toolbar_t
{
	uint8_t size;
	int32_t margin;
	int32_t padding;
	int32_t radius;

	int32_t buttonWidth;
	int32_t buttonHeight;
	ToolbarButton **buttons;

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

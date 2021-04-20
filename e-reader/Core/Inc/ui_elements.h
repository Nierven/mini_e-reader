#ifndef UI_ELEMENTS_H
#define UI_ELEMENTS_H

#include "cmsis_os.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"

#define ICON_MAX_SIZE 4096

#define BUTTON_BOX_MARGIN 2

#define TOOLTIP_MARGIN 25
#define TOOLTIP_HPADDING 13
#define TOOLTIP_VPADDING 9
#define TOOLTIP_FONT Font16
#define TOOLTIP_RADIUS 10

typedef struct Button_t
{
	int32_t x;
	int32_t y;
	int32_t w;
	int32_t h;

	uint8_t isEnabled;
	uint8_t isHovered;
	char tooltip[25];
	uint8_t icon[ICON_MAX_SIZE];

	void (*onClickCallback)(struct Button_t *button);
} Button;

void drawButton(Button *b);
void loadIcon(Button *b, char *filename);

typedef struct ListItem_t
{
	int32_t x;
	int32_t y;
	int32_t w;
	int32_t h;

	uint8_t isHovered;
	uint8_t icon[ICON_MAX_SIZE];

	void (*onClickCallback)(struct ListItem_t *item);
	void (*onIconClickCallback)(struct ListItem_t *item);
} ListItem;

void drawListItem(ListItem *item);
void loadItemIcon(ListItem *item, char *filename);

#endif // UI_ELEMENTS_H

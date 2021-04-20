#include "ui_elements.h"
#include <string.h>
#include "SD.h"

void drawButton(Button *b)
{
	if (b->isHovered)
	{
		BSP_LCD_SetTextColor(0xFF2196F3);
		BSP_LCD_FillRect(b->x - BUTTON_BOX_MARGIN, b->y - BUTTON_BOX_MARGIN,
						 b->w + 2*BUTTON_BOX_MARGIN,
						 b->h + 2*BUTTON_BOX_MARGIN);

		int16_t tooltipWidth = TOOLTIP_FONT.Width * strlen(b->tooltip) + 2*TOOLTIP_HPADDING;
		int16_t tooltipHeight = TOOLTIP_FONT.Height + 2*TOOLTIP_VPADDING;

		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_FillCircle((BSP_LCD_GetXSize() - tooltipWidth) / 2 + TOOLTIP_RADIUS, BSP_LCD_GetYSize() - TOOLTIP_MARGIN - tooltipHeight + TOOLTIP_RADIUS, TOOLTIP_RADIUS);
		BSP_LCD_FillCircle((BSP_LCD_GetXSize() + tooltipWidth) / 2 - TOOLTIP_RADIUS - 1, BSP_LCD_GetYSize() - TOOLTIP_MARGIN - tooltipHeight + TOOLTIP_RADIUS, TOOLTIP_RADIUS);
		BSP_LCD_FillCircle((BSP_LCD_GetXSize() + tooltipWidth) / 2 - TOOLTIP_RADIUS - 1, BSP_LCD_GetYSize() - TOOLTIP_MARGIN - TOOLTIP_RADIUS - 1, TOOLTIP_RADIUS);
		BSP_LCD_FillCircle((BSP_LCD_GetXSize() - tooltipWidth) / 2 + TOOLTIP_RADIUS, BSP_LCD_GetYSize() - TOOLTIP_MARGIN - TOOLTIP_RADIUS - 1, TOOLTIP_RADIUS);
		BSP_LCD_FillRect((BSP_LCD_GetXSize() - tooltipWidth) / 2 + TOOLTIP_RADIUS, BSP_LCD_GetYSize() - TOOLTIP_MARGIN - tooltipHeight, tooltipWidth - 2*TOOLTIP_RADIUS, tooltipHeight);
		BSP_LCD_FillRect((BSP_LCD_GetXSize() - tooltipWidth) / 2, BSP_LCD_GetYSize() - TOOLTIP_MARGIN - tooltipHeight + TOOLTIP_RADIUS, tooltipWidth, tooltipHeight - 2*TOOLTIP_RADIUS);

		BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		BSP_LCD_SetFont(&TOOLTIP_FONT);
		BSP_LCD_DisplayStringAt(BSP_LCD_GetXSize() / 2 - TOOLTIP_FONT.Width * strlen(b->tooltip) / 2,
								BSP_LCD_GetYSize() - TOOLTIP_MARGIN - tooltipHeight / 2 - TOOLTIP_FONT.Height / 2,
								(uint8_t*) b->tooltip, LEFT_MODE);
	}

	BSP_LCD_DrawBitmap(b->x, b->y, !b->isEnabled, 0, b->icon);
}

void loadIcon(Button *b, char *filename)
{
	static uint8_t tmp_buf[ICON_MAX_SIZE];

	UINT bytesRead = 0;
	readFile(filename, tmp_buf, 0, ICON_MAX_SIZE, &bytesRead);
	memcpy(b->icon, tmp_buf, bytesRead);
}

void drawListItem(ListItem *item)
{
	BSP_LCD_SetTextColor(item->isHovered ? 0xFFE6E6E6 : 0xFFF0F0F0);
	BSP_LCD_FillRect(item->x, item->y, item->w, item->h);
	BSP_LCD_SetTextColor(0xFFFFAB00);
	BSP_LCD_DrawHLine(item->x, item->y, item->w);
	BSP_LCD_DrawHLine(item->x, item->y + item->h, item->w);

	uint8_t hasAuthor = strlen(item->info->author) > 0;

	char firstLine[60] = "";
	if (item->info->hasDate && item->info->publicationDate >= 0) sprintf(firstLine, "%s (%d)", item->info->name, item->info->publicationDate);
	else if (item->info->hasDate && item->info->publicationDate < 0) sprintf(firstLine, "%s (%d BC)", item->info->name, abs(item->info->publicationDate));
	else sprintf(firstLine, "%s", item->info->name);

	char secondLine[60] = "";
	if (hasAuthor) sprintf(secondLine, "by: %s", item->info->author);

	BSP_LCD_SetBackColor(item->isHovered ? 0xFFE6E6E6 : 0xFFF0F0F0);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

	BSP_LCD_SetFont((strlen(firstLine) > (item->w - 20) / Font16.Width) ? &Font12 : &Font16);
	BSP_LCD_DisplayStringAt(item->x + 10, item->y + 10, (uint8_t*) firstLine, LEFT_MODE);

	BSP_LCD_SetFont(&Font12);
	BSP_LCD_DisplayStringAt(item->x + 10, item->y + 10 + Font16.Height, (uint8_t*) secondLine, LEFT_MODE);

	if (strlen(item->info->language) != 0)
	{
		BSP_LCD_SetFont(&Font12);
		BSP_LCD_SetTextColor(0xFFE0E0E0);
		BSP_LCD_FillRect(item->x + item->w - ITEM_LANG_MARGIN - ITEM_LANG_HPADDING * 2 - Font12.Width * 2,
				         item->y + item->h - ITEM_LANG_MARGIN - ITEM_LANG_VPADDING * 2 - Font12.Height,
						 Font12.Width * 2 + ITEM_LANG_HPADDING * 2,
						 Font12.Height + ITEM_LANG_VPADDING * 2);

		BSP_LCD_SetBackColor(0xFFE0E0E0);
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_DisplayStringAt(item->x + item->w - ITEM_LANG_MARGIN - ITEM_LANG_HPADDING - Font12.Width * 2,
				                item->y + item->h - ITEM_LANG_MARGIN - ITEM_LANG_VPADDING - Font12.Height,
						        (uint8_t*) item->info->language, LEFT_MODE);
	}

	if (item->icon != NULL)
		BSP_LCD_DrawBitmap(item->x + item->w - ITEM_LANG_MARGIN - 24, item->y + ITEM_LANG_MARGIN, 0, 1, item->icon);
}

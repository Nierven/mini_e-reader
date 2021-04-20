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

	BSP_LCD_DrawBitmap(b->x, b->y, !b->isEnabled, b->icon);
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

}

void loadItemIcon(ListItem *item, char *filename)
{
	static uint8_t tmp_buf[ICON_MAX_SIZE];

	UINT bytesRead = 0;
	readFile(filename, tmp_buf, 0, ICON_MAX_SIZE, &bytesRead);
	memcpy(item->icon, tmp_buf, bytesRead);
}

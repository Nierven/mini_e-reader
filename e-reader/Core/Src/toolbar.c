#include "toolbar.h"

void drawToolbar(Toolbar *toolbar)
{
	if (!toolbar->isVisible)
		return;

	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_FillCircle(toolbar->x + toolbar->radius, toolbar->y + toolbar->radius, toolbar->radius);
	BSP_LCD_FillCircle(toolbar->x + toolbar->w - toolbar->radius - 1, toolbar->y + toolbar->radius, toolbar->radius);
	BSP_LCD_FillCircle(toolbar->x + toolbar->w - toolbar->radius - 1, toolbar->y + toolbar->h - toolbar->radius - 1, toolbar->radius);
	BSP_LCD_FillCircle(toolbar->x + toolbar->radius, toolbar->y + toolbar->h - toolbar->radius, toolbar->radius - 1);
	BSP_LCD_FillRect(toolbar->x + toolbar->radius, toolbar->y, toolbar->w - 2*toolbar->radius, toolbar->h);
	BSP_LCD_FillRect(toolbar->x, toolbar->y + toolbar->radius, toolbar->w, toolbar->h - 2*toolbar->radius);

	for (uint8_t i = 0; i < toolbar->size; i++)
	{
		ToolbarButton *b = toolbar->buttons[i];

		if (b->isHovered)
		{
			BSP_LCD_SetTextColor(0xFF2196F3);
			BSP_LCD_FillRect(b->x - TOOLBAR_HOVER_BOX_MARGIN, b->y - TOOLBAR_HOVER_BOX_MARGIN,
					         toolbar->buttonWidth + 2*TOOLBAR_HOVER_BOX_MARGIN,
							 toolbar->buttonHeight + 2*TOOLBAR_HOVER_BOX_MARGIN);
		}

		if (b->icon != NULL)
		{
			BSP_LCD_DrawBitmap(b->x, b->y, b->icon);
		}
	}
}

void toolbar_OnHover(Toolbar *toolbar, int32_t x, int32_t y)
{
	for (uint8_t i = 0; i < toolbar->size; i++)
	{
		ToolbarButton *b = toolbar->buttons[i];
		b->isHovered = b->isEnabled &&
			           x > b->x && x < b->x + toolbar->buttonWidth &&
			           y > b->y && y < b->y + toolbar->buttonHeight;
	}
}

void toolbar_OnClick(Toolbar *toolbar, int32_t x, int32_t y)
{
	for (uint8_t i = 0; i < toolbar->size; i++)
	{
		ToolbarButton *b = toolbar->buttons[i];
		if (b->isEnabled &&
			x > b->x && x < b->x + toolbar->buttonWidth &&
			y > b->y && y < b->y + toolbar->buttonHeight)
		{
			b->onClickCallback(b);
		}
	}
}

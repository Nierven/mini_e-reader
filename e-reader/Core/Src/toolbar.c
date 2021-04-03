#include "toolbar.h"

void openToolbar(Toolbar *toolbar)
{
	toolbar->isVisible = 1;
}

void drawToolbar(Toolbar *toolbar)
{
	if (!toolbar->isVisible)
		return;

	BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
	BSP_LCD_FillRect(BSP_LCD_GetXSize() - toolbar->margin - 2*toolbar->padding - toolbar->buttonWidth, toolbar->margin,
			         toolbar->buttonWidth + 2*toolbar->padding, toolbar->padding + toolbar->size * (toolbar->buttonHeight + toolbar->padding));

	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	for (int i = 0; i < toolbar->size; i++)
	{
		ToolbarButton *b = &toolbar->buttons[i];
		BSP_LCD_FillRect(b->x, b->y, toolbar->buttonWidth, toolbar->buttonHeight);
	}
}

void closeToolbar(Toolbar *toolbar)
{
	toolbar->isVisible = 0;
}

void toolbar_OnHover(Toolbar *toolbar, int32_t x, int32_t y)
{
	for (int i = 0; i < toolbar->size; i++)
	{
		ToolbarButton *b = &toolbar->buttons[i];
		if (b->isEnabled &&
			x > b->x && x < b->x + toolbar->buttonWidth &&
			y > b->y && y < b->y + toolbar->buttonHeight)
		{
			b->onHoverCallback(b);
		}
	}
}

void toolbar_OnClick(Toolbar *toolbar, int32_t x, int32_t y)
{
	for (int i = 0; i < toolbar->size; i++)
	{
		ToolbarButton *b = &toolbar->buttons[i];
		if (b->isEnabled &&
			x > b->x && x < b->x + toolbar->buttonWidth &&
			y > b->y && y < b->y + toolbar->buttonHeight)
		{
			b->onClickCallback(b);
		}
	}
}

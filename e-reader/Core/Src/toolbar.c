#include "toolbar.h"
#include "book.h"

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
		Button *b = toolbar->buttons[i];
		drawButton(b);
	}

	uint8_t hasAuthor = strlen(loadedBook.info->author) > 0;

	char firstLine[60] = "";
	if (loadedBook.info->hasDate && loadedBook.info->publicationDate >= 0) sprintf(firstLine, "%s (%d)", loadedBook.info->name, loadedBook.info->publicationDate);
	else if (loadedBook.info->hasDate && loadedBook.info->publicationDate < 0) sprintf(firstLine, "%s (%d BC)", loadedBook.info->name, abs(loadedBook.info->publicationDate));
	else sprintf(firstLine, "%s", loadedBook.info->name);

	char secondLine[60] = "";
	if (hasAuthor) sprintf(secondLine, "by: %s", loadedBook.info->author);

	int16_t maxLength = strlen(firstLine) * TITLEBAR_FONT.Width > strlen(secondLine) * TITLEBAR_SUBFONT.Width ?
			strlen(firstLine) * TITLEBAR_FONT.Width :
			strlen(secondLine) * TITLEBAR_SUBFONT.Width;

	int16_t titlebarWidth = maxLength + 2*TITLEBAR_HPADDING;
	int16_t titlebarHeight = TITLEBAR_FONT.Height + (hasAuthor ? TITLEBAR_SUBFONT.Height : 0) + 2*TITLEBAR_VPADDING;

	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_FillCircle(titlebarWidth - TITLEBAR_RADIUS - 1, titlebarHeight - TITLEBAR_RADIUS - 1, TITLEBAR_RADIUS);
	BSP_LCD_FillRect(0, 0, titlebarWidth - TITLEBAR_RADIUS, titlebarHeight);
	BSP_LCD_FillRect(0, 0, titlebarWidth, titlebarHeight - TITLEBAR_RADIUS);

	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

	BSP_LCD_SetFont(&TITLEBAR_FONT);
	BSP_LCD_DisplayStringAt(TITLEBAR_HPADDING, TITLEBAR_VPADDING, (uint8_t*) firstLine, LEFT_MODE);

	BSP_LCD_SetFont(&TITLEBAR_SUBFONT);
	BSP_LCD_DisplayStringAt(TITLEBAR_HPADDING, TITLEBAR_VPADDING + TITLEBAR_FONT.Height, (uint8_t*) secondLine, LEFT_MODE);
}

void toolbar_OnHover(Toolbar *toolbar, int32_t x, int32_t y)
{
	for (uint8_t i = 0; i < toolbar->size; i++)
	{
		Button *b = toolbar->buttons[i];
		b->isHovered = b->isEnabled &&
			           x > b->x && x < b->x + toolbar->buttonWidth &&
			           y > b->y && y < b->y + toolbar->buttonHeight;
	}
}

void toolbar_OnClick(Toolbar *toolbar, int32_t x, int32_t y)
{
	for (uint8_t i = 0; i < toolbar->size; i++)
	{
		Button *b = toolbar->buttons[i];
		if (b->isEnabled &&
			x > b->x && x < b->x + toolbar->buttonWidth &&
			y > b->y && y < b->y + toolbar->buttonHeight)
		{
			b->onClickCallback(b);
		}
	}
}

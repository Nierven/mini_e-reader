#include "ui_bookshelf.h"
#include "ui_elements.h"
#include "ui_reader.h"
#include <string.h>
#include "book.h"
#include "SD.h"

Button browseButton;
static void browseButton_OnClick(Button *button);

void initUIBookshelf(void)
{
	browseButton.onClickCallback = &browseButton_OnClick;
	loadIcon(&browseButton, "icons/search.bmp");
	strcpy(browseButton.tooltip, "Browse books online");

	browseButton.x = BSP_LCD_GetXSize() - 27;
	browseButton.y = 3;
	browseButton.w = 24;
	browseButton.h = 24;
	browseButton.isEnabled = 1;

	initBook();
	initBookInfo(&booksInfo[0]);
	readBookInfo("books/527.txt", &booksInfo[0]);
}

void displayUIBookshelf(void)
{
	BSP_LCD_Clear(LCD_COLOR_WHITE);

	BSP_LCD_SetTextColor(0xFFFFAB00);
	BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 30);

	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_SetBackColor(0xFFFFAB00);
	BSP_LCD_SetFont(&Font16);
	BSP_LCD_DisplayStringAt(7, 7, (uint8_t*) "Bookshelf", LEFT_MODE);

	drawButton(&browseButton);
}

void uiBookshelfLogicHandler(void)
{
	switch (getTouchscreenEvent())
	{
		case Click:
		{
			int32_t x = lastThumbState.x;
			int32_t y = lastThumbState.y;
//			TickType_t t = xTaskGetTickCount();

			if (browseButton.isEnabled &&
				x > browseButton.x && x < browseButton.x + browseButton.w &&
				y > browseButton.y && y < browseButton.y + browseButton.h)
			{
				browseButton.onClickCallback(&browseButton);
			}

			if (y > BSP_LCD_GetYSize() / 2)
			{
				setActualPerspective(Reader);
			}

			break;
		}
		case Move:
		{
			int32_t x = lastThumbState.x;
			int32_t y = lastThumbState.y;
//			int32_t dx = actualThumbState.x - x;
//			int32_t dy = actualThumbState.y - y;
//			TickType_t t = xTaskGetTickCount();

			browseButton.isHovered = browseButton.isEnabled &&
						             x > browseButton.x && x < browseButton.x + browseButton.w &&
						             y > browseButton.y && y < browseButton.y + browseButton.h;

			break;
		}
		default:
		{
			break;
		}
	}
}

void browseButton_OnClick(Button *button)
{
	button->isHovered = 0;
	setActualPerspective(Browse);

	changeBook("books/527.txt", &booksInfo[0]);
}

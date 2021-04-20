#include "ui_browse.h"
#include "ui_elements.h"
#include "ui_reader.h"
#include <string.h>
#include "book.h"
#include "SD.h"

Button bookshelfButton;
static void bookshelfButton_OnClick(Button *button);

void initUIBrowse(void)
{
	bookshelfButton.onClickCallback = &bookshelfButton_OnClick;
	loadIcon(&bookshelfButton, "icons/book_24.bmp");
	strcpy(bookshelfButton.tooltip, "Return to the bookshelf");

	bookshelfButton.x = BSP_LCD_GetXSize() - 27;
	bookshelfButton.y = 3;
	bookshelfButton.w = 24;
	bookshelfButton.h = 24;
	bookshelfButton.isEnabled = 1;
}

void displayUIBrowse(void)
{
	BSP_LCD_Clear(LCD_COLOR_WHITE);

	BSP_LCD_SetTextColor(0xFF3D5AFE);
	BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 30);

	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_SetBackColor(0xFF3D5AFE);
	BSP_LCD_SetFont(&Font16);
	BSP_LCD_DisplayStringAt(7, 7, (uint8_t*) "Browsing", LEFT_MODE);

	drawButton(&bookshelfButton);
}

void uiBrowseLogicHandler(void)
{
	switch (getTouchscreenEvent())
	{
		case Click:
		{
			int32_t x = lastThumbState.x;
			int32_t y = lastThumbState.y;
//			TickType_t t = xTaskGetTickCount();

			if (bookshelfButton.isEnabled &&
				x > bookshelfButton.x && x < bookshelfButton.x + bookshelfButton.w &&
				y > bookshelfButton.y && y < bookshelfButton.y + bookshelfButton.h)
			{
				bookshelfButton.onClickCallback(&bookshelfButton);
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

			bookshelfButton.isHovered = bookshelfButton.isEnabled &&
						                x > bookshelfButton.x && x < bookshelfButton.x + bookshelfButton.w &&
						                y > bookshelfButton.y && y < bookshelfButton.y + bookshelfButton.h;

			break;
		}
		default:
		{
			break;
		}
	}
}

void bookshelfButton_OnClick(Button *button)
{
	button->isHovered = 0;
	setActualPerspective(Bookshelf);
}

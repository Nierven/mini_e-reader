#include "ui_bookshelf.h"

void initUIBookshelf(void)
{

}

void displayUIBookshelf(void)
{
	BSP_LCD_Clear(LCD_COLOR_BLACK);
}

void uiBookshelfLogicHandler(void)
{
	switch (getTouchscreenEvent())
	{
		case Click:
		{
			int32_t x = lastThumbState.x;
			int32_t y = lastThumbState.y;
			TickType_t t = xTaskGetTickCount();

			setActualPerspective(Reader);
			break;
		}
		case Move:
		{
			int32_t x = lastThumbState.x;
			int32_t y = lastThumbState.y;
			int32_t dx = actualThumbState.x - x;
			int32_t dy = actualThumbState.y - y;
			TickType_t t = xTaskGetTickCount();



			break;
		}
		default:
		{
			break;
		}
	}
}

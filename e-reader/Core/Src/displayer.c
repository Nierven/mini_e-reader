#include <string.h>
#include <math.h>

#include "displayer.h"
#include "ethernet.h"
#include "toolbar.h"
#include "book.h"
#include "ui.h"

static void displayBook(void);
static void displayLine(Line *line, int at);
static void displayScrollBar(void);
static uint8_t visibleLayer;

void initScreen(void)
{
	BSP_LCD_Init();
	BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);
	BSP_LCD_LayerDefaultInit(1, LCD_FB_START_ADDRESS + BSP_LCD_GetXSize() * BSP_LCD_GetYSize() * 4);

	BSP_LCD_DisplayOn();
	BSP_LCD_SetLayerVisible(0, DISABLE);
	BSP_LCD_SetLayerVisible(1, ENABLE);

	initUI();

	// Init first layer
	BSP_LCD_SelectLayer(0);
	BSP_LCD_Clear(backColor);
	BSP_LCD_SetBackColor(backColor);
	BSP_LCD_SetTextColor(textColor);
	BSP_LCD_SetFont(textFont);

	// Init second layer
	BSP_LCD_SelectLayer(1);
	BSP_LCD_Clear(backColor);
	BSP_LCD_SetBackColor(backColor);
	BSP_LCD_SetTextColor(textColor);
	BSP_LCD_SetFont(textFont);

	BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
	visibleLayer = 1;
}

void displayerHandler(void)
{
	// Wait for the ui to be available
	if(xSemaphoreTake(semaphore_ui, (TickType_t) 10) == pdTRUE)
	{
		// Screen reset
		BSP_LCD_Clear(backColor);

		displayBook();
		displayScrollBar();
		drawToolbar(&mainToolbar);

		// Swap the layers (double buffering)
		while (!(LTDC->CDSR & LTDC_CDSR_VSYNCS)); // VSYNC
		BSP_LCD_SetLayerVisible(visibleLayer, DISABLE); visibleLayer ^= 1;
		BSP_LCD_SetLayerVisible(visibleLayer, ENABLE);
		BSP_LCD_SelectLayer(1 - visibleLayer);

		xSemaphoreGive(semaphore_ui);
	}

	osDelay(30);
}

void displayBook(void)
{
	BSP_LCD_SetBackColor(backColor);
	BSP_LCD_SetTextColor(textColor);
	BSP_LCD_SetFont(textFont);

	int32_t len = (book.linesSize - bookLineOffset < charMaxHeight) ? book.linesSize - bookLineOffset : charMaxHeight;
	for (int32_t i = 0; i < len; i++)
		displayLine(&book.lines[i + bookLineOffset], i);
}

void displayLine(Line *line, int at)
{
	int16_t current_x = BOOK_MARGIN;
	int8_t pxCounter = 0;

	for (int32_t i = line->index; i < line->index + line->length; i++)
	{
		uint8_t c = book.text[i - book.offset];
		if (c == ' ')
		{
			current_x += line->spaceSize;
			if (pxCounter++ < line->additionalPixelPerFirstSpaces) current_x += 1;
		}
		else
		{
			BSP_LCD_DisplayCharWithoutBackground(current_x, at * textFont->Height, c);
			current_x += textFont->Width;
		}
	}
}

void displayScrollBar(void)
{
	float mult = BSP_LCD_GetYSize() / (float) (book.linesSize + charMaxHeight - 1);
	int16_t y = bookLineOffset * mult;
	int16_t h = charMaxHeight * mult;

	BSP_LCD_SetTextColor(scrollbarColor);
	BSP_LCD_FillRect(0, y, SCROLLBAR_WIDTH, h + 1);
}

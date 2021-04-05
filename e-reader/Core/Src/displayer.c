#include <string.h>
#include <math.h>

#include "displayer.h"
#include "ethernet.h"
#include "toolbar.h"
#include "book.h"
#include "ui.h"

static void displayBook(void);
static void displayLine(Line *line, int at);
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
	for (int32_t i = bookLineOffset; i < bookLineOffset + len; i++)
		displayLine(&book.lines[i], i - bookLineOffset);
}

void displayLine(Line *line, int at)
{
	int16_t current_x = BOOK_MARGIN;
	int32_t currentIndex = line->index;
	int8_t pxCounter = 0;

	char word[50] = "";

	for (int32_t i = line->index; i < line->index + line->length + 1; i++)
	{
		if (i == line->index + line->length || book.text[i - book.offset] == ' ')
		{
			memcpy(word, book.text - book.offset + currentIndex, i - currentIndex); word[i - currentIndex] = '\0';

			BSP_LCD_DisplayStringAt(current_x, at * textFont->Height, (uint8_t*) word, LEFT_MODE);
			current_x += (i - currentIndex) * textFont->Width + line->spaceSize + line->additionalPixelPerSpace;
			if (pxCounter++ < line->additionalPixelPerFirstSpaces) current_x += 1;
			currentIndex = i+1;
		}
	}
}

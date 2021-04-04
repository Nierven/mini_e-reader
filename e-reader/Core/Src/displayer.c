#include <string.h>
#include <math.h>

#include "displayer.h"
#include "ethernet.h"
#include "toolbar.h"
#include "ui.h"

static void displayBook(void);

static uint8_t visibleLayer;

void initScreen(void)
{
	BSP_LCD_Init();
	BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);
	BSP_LCD_LayerDefaultInit(1, LCD_FB_START_ADDRESS + BSP_LCD_GetXSize() * BSP_LCD_GetYSize() * 4);

	BSP_LCD_DisplayOn();
	BSP_LCD_SetLayerVisible(0, DISABLE);
	BSP_LCD_SetLayerVisible(1, ENABLE);

	setFont(12);
	initMainToolbar();

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
	visibleLayer = 0;
}

void displayerHandler(void)
{
//	TickType_t wakeTime = xTaskGetTickCount();

	// Screen reset
	BSP_LCD_Clear(backColor);

	displayBook();
	drawToolbar(&mainToolbar);

	// Swap the layers (double buffering)
	while (!(LTDC->CDSR & LTDC_CDSR_VSYNCS)); // VSYNC
	BSP_LCD_SetLayerVisible(visibleLayer, DISABLE); visibleLayer ^= 1;
	BSP_LCD_SetLayerVisible(visibleLayer, ENABLE);
	BSP_LCD_SelectLayer(1 - visibleLayer);

	osDelay(30);
	//osDelayUntil(&wakeTime, 34); // 30 FPS
}

static void displayLine(int start, int len, int space, int at);
static uint32_t pos = 0;

static void displayBook(void)
{
	BSP_LCD_SetBackColor(backColor);
	BSP_LCD_SetTextColor(textColor);
	BSP_LCD_SetFont(textFont);

	uint8_t currentLine = 0;
	int currentLinePos = pos;

	int lettersCounter = 0;
	int wordLettersCounter = 0;
	int spacesCounter = 0;

	const int minSpace = round(0.7 * textFont->Width);
	const int maxSpace = round(3.0 * textFont->Width);

	for (int i = pos; i < bookSize - pos; i++)
	{
		if (book[i] == ' ')// || book[i] == '\n')
		{
			// Get the optimal space length according to how many letters and space we have
			int actualOptimalSpace;
			if (spacesCounter != 0) actualOptimalSpace = (BSP_LCD_GetXSize() - lettersCounter * textFont->Width) / spacesCounter;
			else actualOptimalSpace = (BSP_LCD_GetXSize() - lettersCounter * textFont->Width);

			if (actualOptimalSpace < minSpace) // If too small, then discard the last word and compute the best length
			{
				lettersCounter -= wordLettersCounter;
				spacesCounter--;

				actualOptimalSpace = (BSP_LCD_GetXSize() - lettersCounter * textFont->Width) / spacesCounter;
				displayLine(currentLinePos, lettersCounter + spacesCounter, actualOptimalSpace, currentLine);

				currentLinePos = currentLinePos + lettersCounter + spacesCounter;
				i = currentLinePos;

				if (book[currentLinePos] == ' ') { currentLinePos++; i++; }
				if (++currentLine == charMaxHeight)
					break;

				lettersCounter = 0;
				wordLettersCounter = 0;
				spacesCounter = 0;
			}
//			else if (book[i] == '\n')
//			{
//				if (actualOptimalSpace > maxSpace)
//					actualOptimalSpace = 1;
//
//				displayLine(currentLinePos, i - currentLinePos, actualOptimalSpace, currentLine);
//
//				currentLinePos = i+1;
//				i = currentLinePos;
//
//				if (++currentLine == charMaxHeight)
//					break;
//
//				lettersCounter = 0;
//				wordLettersCounter = 0;
//				spacesCounter = 0;
//			}
//			else
			{
				spacesCounter++;
				wordLettersCounter = 0;
			}
		}
		else if (book[i] == '\n')
		{
			return;
		}
		else
		{
			wordLettersCounter++;
			lettersCounter++;
		}
	}
}

static void displayLine(int start, int len, int space, int at)
{
	int current_x = 0;
	uint8_t word[50];
	int currentPos = start;

	for (int i = start; i < start + len + 1; i++)
	{
		if (i == start + len || book[i] == ' ')
		{
			memcpy(word, book + currentPos, i - currentPos); word[i - currentPos] = '\0';

			BSP_LCD_DisplayStringAt(current_x, at * textFont->Height, (uint8_t*) word, LEFT_MODE);
			current_x += (i - currentPos) * textFont->Width + space;
			currentPos = i+1;
		}
	}
}

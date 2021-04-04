#include <string.h>
#include "displayer.h"
#include "ethernet.h"
#include "toolbar.h"
#include "ui.h"

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
	TickType_t wakeTime = xTaskGetTickCount();

	// Screen reset
	BSP_LCD_Clear(backColor);

	// ---------- Book display ----------
	BSP_LCD_SetBackColor(backColor);
	BSP_LCD_SetTextColor(textColor);
	BSP_LCD_SetFont(textFont);

	uint8_t currentLine = 0;
	uint16_t currentCharacterIndex = 0;
	char *currentParagraph = book;

	for (int i = 0; i < bookSize; i++)
	{
		if (book[i] == '\0' || currentCharacterIndex == charMaxWidth)
		{
			BSP_LCD_DisplayStringAtLine(currentLine, (uint8_t*) currentParagraph);
			currentParagraph = book + i + 1;
			currentCharacterIndex = 0;

			if (++currentLine == charMaxHeight)
				break;
		}

		currentCharacterIndex++;
	}

	// ---------- Toolbar display ----------
	drawToolbar(&mainToolbar);

	// Swap the layers (double buffering)
	while (!(LTDC->CDSR & LTDC_CDSR_VSYNCS)); // VSYNC
	BSP_LCD_SetLayerVisible(visibleLayer, DISABLE); visibleLayer ^= 1;
	BSP_LCD_SetLayerVisible(visibleLayer, ENABLE);
	BSP_LCD_SelectLayer(1 - visibleLayer);

	osDelay(30);
	//osDelayUntil(&wakeTime, 34); // 30 FPS
}

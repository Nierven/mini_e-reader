#include "ui_splash.h"
#include <string.h>

#define POINT_DT 350
#define SPLASH_DT 3000

static uint8_t pointCount = 0;
static sFONT *splashFont = &Font20;
static TickType_t splashTime = 0;

void initUISplash(void)
{
	splashTime = xTaskGetTickCount();
}

void displayUISplash(void)
{
	// Screen reset
	BSP_LCD_Clear(LCD_COLOR_BLACK);

	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_SetFont(splashFont);

	char text[50] = "Booting...";
	size_t len = strlen(text);
	text[len - (3 - pointCount)] = '\0';

	BSP_LCD_DisplayStringAt((BSP_LCD_GetXSize() - len * splashFont->Width) / 2, (BSP_LCD_GetYSize() - splashFont->Height) / 2, (uint8_t*) text, LEFT_MODE);
}

void uiSplashLogicHandler(void)
{
	static TickType_t lastPointTime = 0;

	TickType_t t = xTaskGetTickCount();
	if ((t - lastPointTime) > POINT_DT)
	{
		if (++pointCount == 4)
			pointCount = 0;

		lastPointTime = t;
	}

	if ((t - splashTime) > SPLASH_DT)
		setActualPerspective(Bookshelf); // Switch to the bookshelf
}

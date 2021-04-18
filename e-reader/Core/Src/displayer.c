#include "displayer.h"

static uint8_t initialized = 0;
static uint8_t visibleLayer;

SemaphoreHandle_t semaphore_ui;
void (**uiDisplayHandlers)(void) = NULL;
void (*uiDisplayHandler)(void) = NULL;
void (**uiLogicHandlers)(void) = NULL;
void (*uiLogicHandler)(void) = NULL;

void initScreen(void)
{
	BSP_LCD_Init();
	BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);
	BSP_LCD_LayerDefaultInit(1, LCD_FB_START_ADDRESS + BSP_LCD_GetXSize() * BSP_LCD_GetYSize() * 4);

	BSP_LCD_DisplayOn();
	BSP_LCD_SetLayerVisible(0, DISABLE);
	BSP_LCD_SetLayerVisible(1, ENABLE);

	// Init first layer
	BSP_LCD_SelectLayer(0);
	BSP_LCD_Clear(LCD_COLOR_BLACK);
	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_SetFont(&Font16);

	// Init second layer
	BSP_LCD_SelectLayer(1);
	BSP_LCD_Clear(LCD_COLOR_BLACK);
	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_SetFont(&Font16);

	BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
	visibleLayer = 1;

	semaphore_ui = xSemaphoreCreateMutex();
	initialized = 1;
}

void displayerHandler(void)
{
	if (initialized == 0)
	{
		osDelay(100);
		return;
	}

	// Wait for the ui to be available
	if(xSemaphoreTake(semaphore_ui, (TickType_t) 10) == pdTRUE)
	{
		if (uiDisplayHandler != NULL)
			uiDisplayHandler();

		// Swap the layers (double buffering)
		while (!(LTDC->CDSR & LTDC_CDSR_VSYNCS)); // VSYNC
		BSP_LCD_SetLayerVisible(visibleLayer, DISABLE); visibleLayer ^= 1;
		BSP_LCD_SetLayerVisible(visibleLayer, ENABLE);
		BSP_LCD_SelectLayer(1 - visibleLayer);

		xSemaphoreGive(semaphore_ui);
	}

	osDelay(30);
}

void setActualPerspective(uint8_t perspective_id)
{
	// Wait for the ui to be available
//	if(xSemaphoreTake(semaphore_ui, portMAX_DELAY) == pdTRUE)
	{
		uiDisplayHandler = uiDisplayHandlers[perspective_id];
		uiLogicHandler = uiLogicHandlers[perspective_id];
//		xSemaphoreGive(semaphore_ui);
	}
}

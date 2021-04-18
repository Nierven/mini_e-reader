#include "logic.h"

static uint8_t initialized = 0;

void initLogic(void)
{
	initialized = 1;
}

void logicHandler(void)
{
	if (initialized == 0)
	{
		osDelay(100);
		return;
	}

	if (uiLogicHandler != NULL)
		uiLogicHandler();

	osDelay(10);
}

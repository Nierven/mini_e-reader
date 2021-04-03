#include "logic.h"
#include "touchscreen.h"
#include "toolbar.h"
#include "global.h"

void initLogic(void)
{
	initMainToolbar();
}

void logicHandler(void)
{
	static TickType_t toolbarLastActivityTime = 0;
	static uint8_t isToolbarOpen = 0;

	switch (getTouchscreenEvent())
	{
		case Click:
		{
			if (!isToolbarOpen)
			{
				openToolbar(&mainToolbar);
				isToolbarOpen = 1;
			}
			else
			{
				toolbar_OnClick(&mainToolbar, lastThumbState.x, lastThumbState.y);
			}

			toolbarLastActivityTime = xTaskGetTickCount();
			break;
		}
		case Move:
		{
			if (isToolbarOpen)
			{
				toolbarLastActivityTime = xTaskGetTickCount();
				toolbar_OnHover(&mainToolbar, lastThumbState.x, lastThumbState.y);
			}

			break;
		}
		default:
		{
			break;
		}
	}

	if (isToolbarOpen && (xTaskGetTickCount() - toolbarLastActivityTime) > TOOLBAR_MAX_INACTIVITY_TIME)
	{
		closeToolbar(&mainToolbar);
		isToolbarOpen = 0;
	}

	osDelay(30);
}

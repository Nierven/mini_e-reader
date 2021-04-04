#include "logic.h"
#include "touchscreen.h"
#include "toolbar.h"
#include "ui.h"

void initLogic(void)
{

}

void logicHandler(void)
{
	static TickType_t toolbarLastActivityTime = 0;

	switch (getTouchscreenEvent())
	{
		case Click:
		{
			if (!mainToolbar.isVisible)
			{
				mainToolbar.isVisible = 1;
				toolbarLastActivityTime = xTaskGetTickCount();
			}
			else
			{
				if (lastThumbState.x > mainToolbar.x && lastThumbState.x < mainToolbar.x + mainToolbar.w &&
					lastThumbState.y > mainToolbar.y && lastThumbState.y < mainToolbar.y + mainToolbar.h)
				{
					toolbarLastActivityTime = xTaskGetTickCount();
					toolbar_OnClick(&mainToolbar, lastThumbState.x, lastThumbState.y);
				}
				else
				{
					mainToolbar.isVisible = 0;
				}
			}

			break;
		}
		case Move:
		{
			if (mainToolbar.isVisible)
			{
				if (lastThumbState.x > mainToolbar.x && lastThumbState.x < mainToolbar.x + mainToolbar.w &&
					lastThumbState.y > mainToolbar.y && lastThumbState.y < mainToolbar.y + mainToolbar.h)
				{
					toolbarLastActivityTime = xTaskGetTickCount();
					toolbar_OnHover(&mainToolbar, lastThumbState.x, lastThumbState.y);
				}
				else
				{
					mainToolbar.isVisible = 0;
				}
			}

			break;
		}
		default:
		{
			break;
		}
	}

	if (mainToolbar.isVisible && (xTaskGetTickCount() - toolbarLastActivityTime) > TOOLBAR_MAX_INACTIVITY_TIME)
		mainToolbar.isVisible = 0;

	osDelay(15);
}

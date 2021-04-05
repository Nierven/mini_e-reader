#include "logic.h"
#include "touchscreen.h"
#include "toolbar.h"
#include "book.h"
#include "ui.h"

static int32_t cumulated_dy = 0;

void initLogic(void)
{

}

void logicHandler(void)
{
	static TickType_t toolbarLastActivityTime = 0;

	// Reset toolbar hovering, which is only visual
	toolbar_OnHover(&mainToolbar, 0, 0);

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
			int32_t dy = actualThumbState.y - lastThumbState.y;
			cumulated_dy += dy;

			if (abs(cumulated_dy) > textFont->Height)
			{
				cumulated_dy %= textFont->Height;
			}

			if (mainToolbar.isVisible)
			{
				if (lastThumbState.x > mainToolbar.x && lastThumbState.x < mainToolbar.x + mainToolbar.w &&
					lastThumbState.y > mainToolbar.y && lastThumbState.y < mainToolbar.y + mainToolbar.h)
				{
					toolbarLastActivityTime = xTaskGetTickCount();
					toolbar_OnHover(&mainToolbar, lastThumbState.x, lastThumbState.y);
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

	osDelay(10);
}

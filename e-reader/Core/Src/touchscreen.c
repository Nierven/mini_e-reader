#include "touchscreen.h"

static TS_StateTypeDef TS_State;
static TS_StateTypeDef last_state;
static TS_StateTypeDef last_filtered_state;
static TS_StateTypeDef actual_state;

static ThumbInfo thumbDownState;
static ThumbInfo thumbUpState;

ThumbInfo lastThumbState;
ThumbInfo actualThumbState;

TouchscreenEvent getTouchscreenEvent(void)
{
	TouchscreenEvent event = No_e;
	BSP_TS_GetState(&TS_State);

	switch (getTouchscreenElementalEvent())
	{
		case ThumbUp:
		{
			if (isClick(thumbDownState, thumbUpState))
				event = Click;

			break;
		}
		case Pressing:
		{
			if (actualThumbState.t - thumbDownState.t > 20)
				event = Move;
			break;
		}
		default:
		{
			break;
		}
	}

	return event;
}

TouchscreenElementalEvent getTouchscreenElementalEvent(void)
{
	static TickType_t lastDebounceTime = 0;
	const TickType_t debounceDelay = 5;

	TouchscreenElementalEvent elementalEvent = No_ee;

	// Checking bounces for the click
	if (TS_State.touchDetected != last_state.touchDetected)
		lastDebounceTime = xTaskGetTickCount();

	// Writing the real filetered state
	if ((xTaskGetTickCount() - lastDebounceTime) >= debounceDelay)
	{
		actual_state.touchX[0] = TS_State.touchX[0];
		actual_state.touchY[0] = TS_State.touchY[0];
		actual_state.touchDetected = TS_State.touchDetected;
	}

	// Checking the actual elemental event
	if (last_filtered_state.touchDetected == 0 && actual_state.touchDetected == 1)
	{
		elementalEvent = ThumbDown;
		thumbDownState.x = actual_state.touchX[0];
		thumbDownState.y = actual_state.touchY[0];
		thumbDownState.t = xTaskGetTickCount();
	}
	else if (last_filtered_state.touchDetected == 1 && actual_state.touchDetected == 1)
	{
		elementalEvent = Pressing;
	}
	else if (last_filtered_state.touchDetected == 1 && actual_state.touchDetected == 0)
	{
		elementalEvent = ThumbUp;
		thumbUpState.x = last_filtered_state.touchX[0];
		thumbUpState.y = last_filtered_state.touchY[0];
		thumbUpState.t = xTaskGetTickCount();
	}

	last_state.touchX[0] = TS_State.touchX[0];
	last_state.touchY[0] = TS_State.touchY[0];
	last_state.touchDetected = TS_State.touchDetected;

	last_filtered_state.touchX[0] = actual_state.touchX[0];
	last_filtered_state.touchY[0] = actual_state.touchY[0];
	last_filtered_state.touchDetected = actual_state.touchDetected;

	lastThumbState.x = actualThumbState.x;
	lastThumbState.y = actualThumbState.y;
	lastThumbState.t = actualThumbState.t;

	actualThumbState.x = actual_state.touchX[0];
	actualThumbState.y = actual_state.touchY[0];
	actualThumbState.t = xTaskGetTickCount();

	return elementalEvent;
}

uint8_t isClick(ThumbInfo down, ThumbInfo up)
{
	return abs(up.x - down.x) < 100 &&
		   abs(up.y - down.y) < 100 &&
		   up.t - down.t > 10;
}

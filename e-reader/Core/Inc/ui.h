#ifndef GLOBAL_H
#define GLOBAL_H

#include "cmsis_os.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"
#include "toolbar.h"

extern uint32_t textColor;
extern uint32_t backColor;
extern uint16_t charMaxWidth;
extern uint16_t charMaxHeight;
extern sFONT *textFont;

int getFont(void);
void setFont(int fontSize);

#define TOOLBAR_MAX_INACTIVITY_TIME 3000
#define TOOLBAR_SIZE 5

#define TOOLBAR_MARGIN 13
#define TOOLBAR_PADDING 7
#define BUTTON_WIDTH 40
#define BUTTON_HEIGHT 40

void initMainToolbar(void);
extern Toolbar mainToolbar;

#endif // GLOBAL_H

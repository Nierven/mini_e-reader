#ifndef UI_H
#define UI_H

#include "cmsis_os.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"
#include "toolbar.h"

#define BOOK_MARGIN 20

#define TOOLBAR_MAX_INACTIVITY_TIME 3000
#define TOOLBAR_SIZE 5

#define TOOLBAR_MARGIN 16
#define TOOLBAR_PADDING 10
#define TOOLBAR_RADIUS 10
#define BUTTON_WIDTH 36
#define BUTTON_HEIGHT 36

extern SemaphoreHandle_t semaphore_ui;

extern uint8_t highContrast;
extern uint32_t textColor;
extern uint32_t backColor;
extern uint16_t charMaxWidth;
extern uint16_t charMaxHeight;
extern sFONT *textFont;

extern int32_t bookLineOffset;

extern Toolbar mainToolbar;

void initUI(void);
int getFont(void);
void setFont(int fontSize);
void initMainToolbar(void);

#endif // UI_H

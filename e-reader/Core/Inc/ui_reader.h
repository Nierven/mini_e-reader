#ifndef UI_READER_H
#define UI_READER_H

#include "displayer.h"
#include "toolbar.h"

#define BOOK_MARGIN 20

#define TOOLBAR_MAX_INACTIVITY_TIME 3000
#define TOOLBAR_SIZE 5

#define TOOLBAR_MARGIN 16
#define TOOLBAR_PADDING 10
#define TOOLBAR_RADIUS 10
#define BUTTON_WIDTH 36
#define BUTTON_HEIGHT 36

#define SCROLLBAR_WIDTH 4

extern uint8_t highContrast;
extern uint32_t textColor;
extern uint32_t backColor;
extern uint32_t scrollbarColor;
extern uint16_t charMaxWidth;
extern uint16_t charMaxHeight;
extern sFONT *textFont;

extern int32_t bookLineOffset;

extern Toolbar mainToolbar;

void initUIReader(void);
void displayUIReader(void);
void uiReaderLogicHandler(void);

int getFont(void);
void setFont(int fontSize);

#endif // UI_READER_H

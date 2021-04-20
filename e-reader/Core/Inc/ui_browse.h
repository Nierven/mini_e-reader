#ifndef UI_BROWSE_H
#define UI_BROWSE_H

#include "displayer.h"
#include "book.h"

void initUIBrowse(void);
void downloadList(void);
void downloadBook(BookInfo *info);
void displayUIBrowse(void);
void uiBrowseLogicHandler(void);

#endif // UI_BROWSE_H

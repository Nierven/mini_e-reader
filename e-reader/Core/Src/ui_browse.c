#include "ui_browse.h"
#include "ui_elements.h"
#include "ui_bookshelf.h"
#include "ui_reader.h"
#include <string.h>
#include "ethernet.h"
#include "SD.h"

Button bookshelfButton;
static void bookshelfButton_OnClick(Button *button);

static int listSize = 0;
static uint8_t downloadIcon[ICON_MAX_SIZE];
static ListItem listItems[MAX_BOOKS_LOADED];
static void listItem_OnClick(ListItem *li);

static SemaphoreHandle_t semaphore_browse;

void initUIBrowse(void)
{
	bookshelfButton.onClickCallback = &bookshelfButton_OnClick;
	loadIcon(&bookshelfButton, "icons/book_24.bmp");
	strcpy(bookshelfButton.tooltip, "Return to the bookshelf");

	bookshelfButton.x = BSP_LCD_GetXSize() - 27;
	bookshelfButton.y = 3;
	bookshelfButton.w = 24;
	bookshelfButton.h = 24;
	bookshelfButton.isEnabled = 1;

	downloadList();

	semaphore_browse = xSemaphoreCreateMutex();
}

void downloadList(void)
{
//	char data[1000]; uint32_t bytesDownloaded = 0;
//	getStructOnline((uint8_t*) data, "/", &bytesDownloaded);

//	for (int bookCpt = 0; bookCpt < MAX_BOOKS_LOADED; bookCpt++)
//	{
//		f_readdir(&dir, &fno);
//		if (fno.fname[0] == 0)
//		{
//			listSize = bookCpt;
//			break;
//		}
//
//		// New book, register it
//		sprintf(path, "books/%s", fno.fname);
//		initBookInfo(&booksInfo[bookCpt]);
//		readBookInfo(path, &booksInfo[bookCpt]);
//
//		// Create an associated list item
//		ListItem *li = &listItems[bookCpt];
//
//		li->x = 20;
//		li->y = 40 + bookCpt*50;
//		li->w = BSP_LCD_GetXSize() - 20 * 2;
//		li->h = 50;
//
//		li->info = &booksInfo[bookCpt];
//		li->icon = NULL;
//
//		li->onClickCallback = &listItem_OnClick;
//		li->onIconClickCallback = NULL;
//		li->isHovered = 0;
//	}

	ListItem *li = &listItems[0];

	li->x = 20;
	li->y = 40;
	li->w = BSP_LCD_GetXSize() - 20 * 2;
	li->h = 50;

	li->info = &onlineBooksInfo[0];
	strcpy(li->info->name, "test");
	strcpy(li->info->author, "me");
	strcpy(li->info->link, "/files/45/45-0.txt");
	strcpy(li->info->filename, "45-0.txt");
	strcpy(li->info->language, "en");
	li->info->publicationDate = 2001;
	li->info->hasDate = 1;

	li->icon = NULL;

	li->onClickCallback = &listItem_OnClick;
	li->onIconClickCallback = NULL;
	li->isHovered = 0;

	listSize = 1;

//	downloadBook(li->info);
}

static void writeBuffer(netbuf_t *buf);

void downloadBook(BookInfo *info)
{
	char newFilename[100]; sprintf(newFilename, "/books/%s", info->filename);
	f_open(&SDFile, newFilename, FA_WRITE | FA_CREATE_ALWAYS);

	getFileOnlineCallback(info->link, &writeBuffer);

	f_close(&SDFile);
	buildBookshelf();
}

void writeBuffer(netbuf_t *buf)
{
	uint8_t data[1024];
	netbuf_copy(buf, data, netbuf_len(buf));

	f_write(&SDFile, data, netbuf_len(buf), NULL);
}

void displayUIBrowse(void)
{
	BSP_LCD_Clear(LCD_COLOR_WHITE);

	// Wait for the bookshelf to be available
	if(xSemaphoreTake(semaphore_browse, (TickType_t) 10) == pdTRUE)
	{
		for (int i = 0; i < listSize; i++)
			drawListItem(&listItems[i]);

		xSemaphoreGive(semaphore_browse);
	}

	BSP_LCD_SetTextColor(0xFF3D5AFE);
	BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 30);

	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_SetBackColor(0xFF3D5AFE);
	BSP_LCD_SetFont(&Font16);
	BSP_LCD_DisplayStringAt(7, 7, (uint8_t*) "Browsing", LEFT_MODE);

	drawButton(&bookshelfButton);
}

void uiBrowseLogicHandler(void)
{
	switch (getTouchscreenEvent())
	{
		case Click:
		{
			int32_t x = lastThumbState.x;
			int32_t y = lastThumbState.y;
//			TickType_t t = xTaskGetTickCount();

			if (bookshelfButton.isEnabled &&
				x > bookshelfButton.x && x < bookshelfButton.x + bookshelfButton.w &&
				y > bookshelfButton.y && y < bookshelfButton.y + bookshelfButton.h)
			{
				bookshelfButton.onClickCallback(&bookshelfButton);
			}

			for (int i = 0; i < listSize; i++)
			{
				ListItem *li = &listItems[i];

				if (x > li->x && x < li->x + li->w &&
						 y > li->y && y < li->y + li->h)
				{
					li->onClickCallback(li);
					break;
				}
			}

			break;
		}
		case Move:
		{
			int32_t x = lastThumbState.x;
			int32_t y = lastThumbState.y;
//			int32_t dx = actualThumbState.x - x;
//			int32_t dy = actualThumbState.y - y;
//			TickType_t t = xTaskGetTickCount();

			bookshelfButton.isHovered = bookshelfButton.isEnabled &&
						                x > bookshelfButton.x && x < bookshelfButton.x + bookshelfButton.w &&
						                y > bookshelfButton.y && y < bookshelfButton.y + bookshelfButton.h;

			for (int i = 0; i < listSize; i++)
			{
				ListItem *li = &listItems[i];
				li->isHovered = x > li->x && x < li->x + li->w &&
								y > li->y && y < li->y + li->h;
			}

			break;
		}
		default:
		{
			break;
		}
	}
}

void bookshelfButton_OnClick(Button *button)
{
	button->isHovered = 0;
	setActualPerspective(Bookshelf);
}

void listItem_OnClick(ListItem *li)
{
	downloadBook(li->info);
	li->icon = downloadIcon;
}

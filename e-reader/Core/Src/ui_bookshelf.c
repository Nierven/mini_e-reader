#include "ui_bookshelf.h"
#include "ui_elements.h"
#include "ui_reader.h"
#include <string.h>
#include "book.h"
#include "SD.h"

static Button browseButton;
static void browseButton_OnClick(Button *button);

static int listSize = 0;
static uint8_t removeIcon[ICON_MAX_SIZE];
static ListItem listItems[MAX_BOOKS_LOADED];
static void listItem_OnClick(ListItem *li);
static void listItem_OnIconClick(ListItem *li);

static SemaphoreHandle_t semaphore_bookshelf;

void initUIBookshelf(void)
{
	browseButton.onClickCallback = &browseButton_OnClick;
	loadIcon(&browseButton, "icons/search.bmp");
	strcpy(browseButton.tooltip, "Browse books online");

	browseButton.x = BSP_LCD_GetXSize() - 27;
	browseButton.y = 3;
	browseButton.w = 24;
	browseButton.h = 24;
	browseButton.isEnabled = 1;

	readFile("icons/remove.bmp", removeIcon, 0, ICON_MAX_SIZE, NULL);
	buildBookshelf();

	semaphore_bookshelf = xSemaphoreCreateMutex();
}

void buildBookshelf(void)
{
	DIR dir;
	FILINFO fno;
	char path[50] = "/books";
	f_opendir(&dir, path);

	for (int bookCpt = 0; bookCpt < MAX_BOOKS_LOADED; bookCpt++)
	{
		f_readdir(&dir, &fno);
		if (fno.fname[0] == 0)
		{
			listSize = bookCpt;
			break;
		}

		// New book, register it
		sprintf(path, "books/%s", fno.fname);
		initBookInfo(&booksInfo[bookCpt]);
		readBookInfo(path, &booksInfo[bookCpt]);

		// Create an associated list item
		ListItem *li = &listItems[bookCpt];

		li->x = 20;
		li->y = 40 + bookCpt*50;
		li->w = BSP_LCD_GetXSize() - 20 * 2;
		li->h = 50;

		li->info = &booksInfo[bookCpt];
		li->icon = removeIcon;

		li->onClickCallback = &listItem_OnClick;
		li->onIconClickCallback = &listItem_OnIconClick;
		li->isHovered = 0;
	}

	f_closedir(&dir);
	initBook();
}

void displayUIBookshelf(void)
{
	BSP_LCD_Clear(LCD_COLOR_WHITE);

	// Wait for the bookshelf to be available
	if(xSemaphoreTake(semaphore_bookshelf, (TickType_t) 10) == pdTRUE)
	{
		for (int i = 0; i < listSize; i++)
			drawListItem(&listItems[i]);

		xSemaphoreGive(semaphore_bookshelf);
	}

	BSP_LCD_SetTextColor(0xFFFFAB00);
	BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 30);

	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_SetBackColor(0xFFFFAB00);
	BSP_LCD_SetFont(&Font16);
	BSP_LCD_DisplayStringAt(7, 7, (uint8_t*) "Bookshelf", LEFT_MODE);

	drawButton(&browseButton);
}

void uiBookshelfLogicHandler(void)
{
	switch (getTouchscreenEvent())
	{
		case Click:
		{
			int32_t x = lastThumbState.x;
			int32_t y = lastThumbState.y;
//			TickType_t t = xTaskGetTickCount();

			if (browseButton.isEnabled &&
				x > browseButton.x && x < browseButton.x + browseButton.w &&
				y > browseButton.y && y < browseButton.y + browseButton.h)
			{
				browseButton.onClickCallback(&browseButton);
			}

			for (int i = 0; i < listSize; i++)
			{
				ListItem *li = &listItems[i];

				int32_t icon_x1 = li->x + li->w - ITEM_LANG_MARGIN - 24;
				int32_t icon_y1 = li->y + ITEM_LANG_MARGIN;
				int32_t icon_x2 = li->x + li->w - ITEM_LANG_MARGIN;
				int32_t icon_y2 = li->x + li->w - ITEM_LANG_MARGIN;

				if (x > icon_x1 && x < icon_x2 &&
				    y > icon_y1 && y < icon_y2)
				{
					li->onIconClickCallback(li);
					break;
				}
				else if (x > li->x && x < li->x + li->w &&
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

			browseButton.isHovered = browseButton.isEnabled &&
						             x > browseButton.x && x < browseButton.x + browseButton.w &&
						             y > browseButton.y && y < browseButton.y + browseButton.h;

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

void browseButton_OnClick(Button *button)
{
	button->isHovered = 0;
	setActualPerspective(Browse);
}

void listItem_OnClick(ListItem *li)
{
	changeBook(li->info);
	setActualPerspective(Reader);
}

void listItem_OnIconClick(ListItem *li)
{
	// Wait for the bookshelf to be available
	if(xSemaphoreTake(semaphore_bookshelf, (TickType_t) 10) == pdTRUE)
	{
		deleteFile(li->info->filename);
		buildBookshelf();

		xSemaphoreGive(semaphore_bookshelf);
	}
}

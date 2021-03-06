#include "ui_reader.h"
#include "SD.h"

uint8_t highContrast = 1;
uint32_t backColor = LCD_COLOR_WHITE;
uint32_t textColor = LCD_COLOR_BLACK;
uint32_t scrollbarColor = 0xFF909090;
uint16_t charMaxHeight;
sFONT *textFont;

int32_t bookLineOffset = 0;
uint16_t bookWidth = 0;

static void displayBook(void);
static void displayLine(Line *line, int at);
static void displayScrollBar(void);
static void initMainToolbar(void);

Toolbar mainToolbar;

static Button *toolbarButtons[TOOLBAR_SIZE];
static Button zoomInButton;
static Button zoomOutButton;
static Button contrastButton;
static Button topButton;
static Button bookListButton;

static void zoomInButton_OnClick(Button *button);
static void zoomOutButton_OnClick(Button *button);
static void contrastButton_OnClick(Button *button);
static void topButton_OnClick(Button *button);
static void bookListButton_OnClick(Button *button);

void initUIReader(void)
{
	setFont(12);
	initMainToolbar();

	bookWidth = BSP_LCD_GetXSize() - BOOK_MARGIN * 2;
}

void changeBook(BookInfo *info)
{
	openBook(info);
	buildBook(bookWidth, textFont->Width);

	bookLineOffset = 0;
	topButton.isEnabled = 0;
}

void displayUIReader(void)
{
	// Screen reset
	BSP_LCD_Clear(backColor);

	displayBook();
	displayScrollBar();
	drawToolbar(&mainToolbar);
}

void displayBook(void)
{
	BSP_LCD_SetBackColor(backColor);
	BSP_LCD_SetTextColor(textColor);
	BSP_LCD_SetFont(textFont);

	int32_t len = (loadedBook.linesSize - bookLineOffset < charMaxHeight) ? loadedBook.linesSize - bookLineOffset : charMaxHeight;
	for (int32_t i = 0; i < len; i++)
		displayLine(&loadedBook.lines[i + bookLineOffset], i);
}

void displayLine(Line *line, int at)
{
	int16_t current_x = BOOK_MARGIN;
	int8_t pxCounter = 0;

	for (int32_t i = line->index; i < line->index + line->length; i++)
	{
		uint8_t c = loadedBook.text[i];
		if (c == ' ')
		{
			current_x += line->spaceSize;
			if (pxCounter++ < line->additionalPixelPerFirstSpaces) current_x += 1;
		}
		else
		{
			BSP_LCD_DisplayCharWithoutBackground(current_x, at * textFont->Height, c);
			current_x += textFont->Width;
		}
	}
}

void displayScrollBar(void)
{
	float mult = BSP_LCD_GetYSize() / (float) (loadedBook.linesSize + charMaxHeight - 1);
	int16_t y = bookLineOffset * mult;
	int16_t h = charMaxHeight * mult;

	BSP_LCD_SetTextColor(scrollbarColor);
	BSP_LCD_FillRect(0, y, SCROLLBAR_WIDTH, h + 1);
}

static int32_t cumulated_dy = 0;

void uiReaderLogicHandler(void)
{
	static TickType_t toolbarLastActivityTime = 0;

	// Reset toolbar hovering, which is only visual
	toolbar_OnHover(&mainToolbar, 0, 0);

	switch (getTouchscreenEvent())
	{
		case Click:
		{
			int32_t x = lastThumbState.x;
			int32_t y = lastThumbState.y;
			TickType_t t = xTaskGetTickCount();

			if (!mainToolbar.isVisible)
			{
				mainToolbar.isVisible = 1;
				toolbarLastActivityTime = t;
			}
			else
			{
				if (x > mainToolbar.x && x < mainToolbar.x + mainToolbar.w &&
					y > mainToolbar.y && y < mainToolbar.y + mainToolbar.h)
				{
					toolbarLastActivityTime = t;
					toolbar_OnClick(&mainToolbar, x, y);
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
			int32_t x = lastThumbState.x;
			int32_t y = lastThumbState.y;
//			int32_t dx = actualThumbState.x - x;
			int32_t dy = actualThumbState.y - y;
			TickType_t t = xTaskGetTickCount();

			if (mainToolbar.isVisible && x > mainToolbar.x && x < mainToolbar.x + mainToolbar.w &&
										 y > mainToolbar.y && y < mainToolbar.y + mainToolbar.h)
			{
				toolbarLastActivityTime = t;
				toolbar_OnHover(&mainToolbar, x, y);
			}
			else
			{
				cumulated_dy += dy;
				int16_t dline = cumulated_dy / textFont->Height;
				cumulated_dy %= textFont->Height;

				if (bookLineOffset - dline > loadedBook.linesSize - 1)
					dline = bookLineOffset - loadedBook.linesSize + 1;
				else if (bookLineOffset - dline < 0)
					dline = bookLineOffset;

				if (dline != 0)
				{
					// Wait for the ui to be available
					if(xSemaphoreTake(semaphore_ui, portMAX_DELAY) == pdTRUE)
					{
						bookLineOffset -= dline;
						topButton.isEnabled = (bookLineOffset != 0);
						xSemaphoreGive(semaphore_ui);
					}
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
}

int getFont(void)
{
	return textFont->Height;
}

void setFont(int fontSize)
{
	sFONT *fonts[5] = { &Font8, &Font12, &Font16, &Font20, &Font24 };
	textFont = fonts[(fontSize - 8) / 4];
	charMaxHeight = BSP_LCD_GetYSize() / textFont->Height;
}

void initMainToolbar(void)
{
	mainToolbar.isVisible = 0;
	mainToolbar.buttonHeight = BUTTON_HEIGHT;
	mainToolbar.buttonWidth = BUTTON_WIDTH;
	mainToolbar.margin = TOOLBAR_MARGIN;
	mainToolbar.padding = TOOLBAR_PADDING;
	mainToolbar.radius = TOOLBAR_RADIUS;
	mainToolbar.size = TOOLBAR_SIZE;

	mainToolbar.x = BSP_LCD_GetXSize() - mainToolbar.margin - 2*mainToolbar.padding - mainToolbar.buttonWidth;
	mainToolbar.y = mainToolbar.margin;
	mainToolbar.w = mainToolbar.buttonWidth + 2*mainToolbar.padding;
	mainToolbar.h = mainToolbar.padding + mainToolbar.size * (mainToolbar.buttonHeight + mainToolbar.padding);

	mainToolbar.buttons = toolbarButtons;

	zoomInButton.onClickCallback = &zoomInButton_OnClick;
	loadIcon(&zoomInButton, "icons/zoom_in.bmp");
	strcpy(zoomInButton.tooltip, "Zoom In");
	toolbarButtons[0] = &zoomInButton;

	zoomOutButton.onClickCallback = &zoomOutButton_OnClick;
	loadIcon(&zoomOutButton, "icons/zoom_out.bmp");
	strcpy(zoomOutButton.tooltip, "Zoom Out");
	toolbarButtons[1] = &zoomOutButton;

	contrastButton.onClickCallback = &contrastButton_OnClick;
	loadIcon(&contrastButton, "icons/contrast.bmp");
	strcpy(contrastButton.tooltip, "Change the contrast mode");
	toolbarButtons[2] = &contrastButton;

	topButton.onClickCallback = &topButton_OnClick;
	loadIcon(&topButton, "icons/top.bmp");
	strcpy(topButton.tooltip, "Return to top");
	toolbarButtons[3] = &topButton;

	bookListButton.onClickCallback = &bookListButton_OnClick;
	loadIcon(&bookListButton, "icons/book_36.bmp");
	strcpy(bookListButton.tooltip, "Back to the Bookshelf");
	toolbarButtons[4] = &bookListButton;

	for (int i = 0; i < mainToolbar.size; i++)
	{
		toolbarButtons[i]->x = mainToolbar.x + mainToolbar.padding;
		toolbarButtons[i]->y = mainToolbar.y + mainToolbar.padding + (mainToolbar.buttonHeight + mainToolbar.padding) * i;
		toolbarButtons[i]->w = BUTTON_WIDTH;
		toolbarButtons[i]->h = BUTTON_HEIGHT;
		toolbarButtons[i]->isEnabled = 1;
	}

	topButton.isEnabled = 0;
}

void zoomInButton_OnClick(Button *button)
{
	// Wait for the ui to be available
	if(xSemaphoreTake(semaphore_ui, portMAX_DELAY) == pdTRUE)
	{
		int32_t actualFirstIndex = loadedBook.lines[bookLineOffset].index;

		int newFontSize = getFont() + 4;
		setFont(newFontSize);

		if (newFontSize == 20)
			button->isEnabled = 0;

		if (!zoomOutButton.isEnabled)
			zoomOutButton.isEnabled = 1;

		buildBook(bookWidth, textFont->Width);

		// Search for the first word shown using the previous zoom level
		for (int32_t i = bookLineOffset; i < loadedBook.linesSize; i++)
		{
			if (loadedBook.lines[i].index <= actualFirstIndex &&
				loadedBook.lines[i].index + loadedBook.lines[i].length > actualFirstIndex)
			{
				bookLineOffset = i;
				break;
			}

			int a = 0;
			a++;
		}

		xSemaphoreGive(semaphore_ui);
	}
}

void zoomOutButton_OnClick(Button *button)
{
	// Wait for the ui to be available
	if(xSemaphoreTake(semaphore_ui, portMAX_DELAY) == pdTRUE)
	{
		int32_t actualFirstIndex = loadedBook.lines[bookLineOffset].index;

		int newFontSize = getFont() - 4;
		setFont(newFontSize);

		if (newFontSize == 8)
			button->isEnabled = 0;

		if (!zoomInButton.isEnabled)
			zoomInButton.isEnabled = 1;

		buildBook(bookWidth, textFont->Width);

		// Search for the first word shown using the previous zoom level
		int32_t start = bookLineOffset < loadedBook.linesSize ? bookLineOffset : loadedBook.linesSize - 1;
		for (int32_t i = start; i >= 0; i--)
		{
			if (loadedBook.lines[i].index <= actualFirstIndex)
			{
				bookLineOffset = i;
				break;
			}
		}

		xSemaphoreGive(semaphore_ui);
	}
}

void contrastButton_OnClick(Button *button)
{
	// Wait for the ui to be available
	if(xSemaphoreTake(semaphore_ui, portMAX_DELAY) == pdTRUE)
	{
		highContrast ^= 1;
		backColor = highContrast ? LCD_COLOR_WHITE : 0xFF303030;
		textColor = highContrast ? LCD_COLOR_BLACK : LCD_COLOR_WHITE;
		scrollbarColor = highContrast ? 0xFF909090 : 0xFF707070;

		xSemaphoreGive(semaphore_ui);
	}
}

void topButton_OnClick(Button *button)
{
	// Wait for the ui to be available
	if(xSemaphoreTake(semaphore_ui, portMAX_DELAY) == pdTRUE)
	{
		bookLineOffset = 0;
		topButton.isEnabled = 0;

		xSemaphoreGive(semaphore_ui);
	}
}

void bookListButton_OnClick(Button *button)
{
	setActualPerspective(Bookshelf);
}

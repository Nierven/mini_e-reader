#include "ui.h"
#include "logos.h"
#include "book.h"

SemaphoreHandle_t semaphore_ui;

uint8_t highContrast = 1;
uint32_t backColor = LCD_COLOR_WHITE;
uint32_t textColor = LCD_COLOR_BLACK;
uint32_t scrollbarColor = 0xFF909090;
uint16_t charMaxWidth;
uint16_t charMaxHeight;
sFONT *textFont;

int32_t bookLineOffset = 0;
static uint16_t bookWidth = 0;

void initUI(void)
{
	setFont(12);
	initMainToolbar();

	initBook();
	bookWidth = BSP_LCD_GetXSize() - BOOK_MARGIN * 2;
	buildBook(bookWidth, textFont->Width);

	semaphore_ui = xSemaphoreCreateMutex();
}

int getFont(void)
{
	return textFont->Height;
}

void setFont(int fontSize)
{
	sFONT *fonts[5] = { &Font8, &Font12, &Font16, &Font20, &Font24 };
	textFont = fonts[(fontSize - 8) / 4];
	charMaxWidth = BSP_LCD_GetXSize() / textFont->Width;
	charMaxHeight = BSP_LCD_GetYSize() / textFont->Height;
}

Toolbar mainToolbar;

static ToolbarButton *toolbarButtons[TOOLBAR_SIZE];
static ToolbarButton zoomInButton;
static ToolbarButton zoomOutButton;
static ToolbarButton contrastButton;
static ToolbarButton orientationButton;
static ToolbarButton bookListButton;

static void zoomInButton_OnClick(ToolbarButton *button);
static void zoomOutButton_OnClick(ToolbarButton *button);
static void contrastButton_OnClick(ToolbarButton *button);
static void orientationButton_OnClick(ToolbarButton *button);
static void bookListButton_OnClick(ToolbarButton *button);

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
	zoomInButton.icon = zoomInBitmap;
	strcpy(zoomInButton.tooltip, "Zoom In");
	toolbarButtons[0] = &zoomInButton;

	zoomOutButton.onClickCallback = &zoomOutButton_OnClick;
	zoomOutButton.icon = zoomOutBitmap;
	strcpy(zoomOutButton.tooltip, "Zoom Out");
	toolbarButtons[1] = &zoomOutButton;

	contrastButton.onClickCallback = &contrastButton_OnClick;
	contrastButton.icon = contrastBitmap;
	strcpy(contrastButton.tooltip, "Change the contrast mode");
	toolbarButtons[2] = &contrastButton;

	orientationButton.onClickCallback = &orientationButton_OnClick;
	orientationButton.icon = orientationBitmap;
	strcpy(orientationButton.tooltip, "Rotate the screen");
	toolbarButtons[3] = &orientationButton;

	bookListButton.onClickCallback = &bookListButton_OnClick;
	bookListButton.icon = bookListBitmap;
	strcpy(bookListButton.tooltip, "Back to the Book List");
	toolbarButtons[4] = &bookListButton;

	for (int i = 0; i < mainToolbar.size; i++)
	{
		toolbarButtons[i]->x = mainToolbar.x + mainToolbar.padding;
		toolbarButtons[i]->y = mainToolbar.y + mainToolbar.padding + (mainToolbar.buttonHeight + mainToolbar.padding) * i;
		toolbarButtons[i]->isEnabled = 1;
	}
}

void zoomInButton_OnClick(ToolbarButton *button)
{
	// Wait for the ui to be available
	if(xSemaphoreTake(semaphore_ui, portMAX_DELAY) == pdTRUE)
	{
		int32_t actualFirstIndex = book.lines[bookLineOffset].index;

		int newFontSize = getFont() + 4;
		setFont(newFontSize);

		if (newFontSize == 24)
			button->isEnabled = 0;

		if (!zoomOutButton.isEnabled)
			zoomOutButton.isEnabled = 1;

		buildBook(bookWidth, textFont->Width);

		// Search for the first word shown using the previous zoom level
		for (int32_t i = bookLineOffset; i < book.linesSize; i++)
		{
			if (book.lines[i].index <= actualFirstIndex &&
				book.lines[i].index + book.lines[i].length > actualFirstIndex)
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

void zoomOutButton_OnClick(ToolbarButton *button)
{
	// Wait for the ui to be available
	if(xSemaphoreTake(semaphore_ui, portMAX_DELAY) == pdTRUE)
	{
		int32_t actualFirstIndex = book.lines[bookLineOffset].index;

		int newFontSize = getFont() - 4;
		setFont(newFontSize);

		if (newFontSize == 8)
			button->isEnabled = 0;

		if (!zoomInButton.isEnabled)
			zoomInButton.isEnabled = 1;

		buildBook(bookWidth, textFont->Width);

		// Search for the first word shown using the previous zoom level
		int32_t start = bookLineOffset < book.linesSize ? bookLineOffset : book.linesSize - 1;
		for (int32_t i = start; i >= 0; i--)
		{
			if (book.lines[i].index <= actualFirstIndex)
			{
				bookLineOffset = i;
				break;
			}
		}

		xSemaphoreGive(semaphore_ui);
	}
}

void contrastButton_OnClick(ToolbarButton *button)
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

void orientationButton_OnClick(ToolbarButton *button)
{

}

void bookListButton_OnClick(ToolbarButton *button)
{

}

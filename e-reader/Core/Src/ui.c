#include "ui.h"
#include "logos.h"

static ToolbarButton *toolbarButtons[TOOLBAR_SIZE];
static ToolbarButton zoomInButton;
static ToolbarButton zoomOutButton;
static ToolbarButton contrastButton;
static ToolbarButton orientationButton;
static ToolbarButton bookListButton;

uint32_t backColor = LCD_COLOR_WHITE;
uint32_t textColor = LCD_COLOR_BLACK;
uint16_t charMaxWidth;
uint16_t charMaxHeight;
sFONT *textFont;

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

static void toolBarButton_OnHover(ToolbarButton *button);
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
		toolbarButtons[i]->onHoverCallback = &toolBarButton_OnHover;
	}
}

void toolBarButton_OnHover(ToolbarButton *button)
{

}

void zoomInButton_OnClick(ToolbarButton *button)
{
	int newFontSize = getFont() + 4;
	setFont(newFontSize);

	if (newFontSize == 24)
		button->isEnabled = 0;

	if (!zoomOutButton.isEnabled)
		zoomOutButton.isEnabled = 1;
}

void zoomOutButton_OnClick(ToolbarButton *button)
{
	int newFontSize = getFont() - 4;
	setFont(newFontSize);

	if (newFontSize == 8)
		button->isEnabled = 0;

	if (!zoomInButton.isEnabled)
		zoomInButton.isEnabled = 1;
}

void contrastButton_OnClick(ToolbarButton *button)
{

}

void orientationButton_OnClick(ToolbarButton *button)
{

}

void bookListButton_OnClick(ToolbarButton *button)
{

}

#include "global.h"
#include "logos.h"

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

	ToolbarButton toolbarButtons[TOOLBAR_SIZE];
	mainToolbar.buttons = toolbarButtons;

	ToolbarButton zoomInButton;
	zoomInButton.onClickCallback = &zoomInButton_OnClick;
	zoomInButton.icon = zoomInBitmap;
	strcpy(zoomInButton.tooltip, "Zoom In");
	toolbarButtons[0] = zoomInButton;

	ToolbarButton zoomOutButton;
	zoomOutButton.onClickCallback = &zoomOutButton_OnClick;
	zoomOutButton.icon = zoomOutBitmap;
	strcpy(zoomOutButton.tooltip, "Zoom Out");
	toolbarButtons[1] = zoomOutButton;

	ToolbarButton contrastButton;
	contrastButton.onClickCallback = &contrastButton_OnClick;
	contrastButton.icon = contrastBitmap;
	strcpy(contrastButton.tooltip, "Change the contrast mode");
	toolbarButtons[2] = contrastButton;

	ToolbarButton orientationButton;
	orientationButton.onClickCallback = &orientationButton_OnClick;
	orientationButton.icon = orientationBitmap;
	strcpy(orientationButton.tooltip, "Rotate the screen");
	toolbarButtons[3] = orientationButton;

	ToolbarButton bookListButton;
	bookListButton.onClickCallback = &bookListButton_OnClick;
	bookListButton.icon = bookListBitmap;
	strcpy(bookListButton.tooltip, "Back to the Book List");
	toolbarButtons[4] = bookListButton;

	for (int i = 0; i < TOOLBAR_SIZE; i++)
	{
		toolbarButtons[i].x = BSP_LCD_GetXSize() - TOOLBAR_MARGIN - TOOLBAR_PADDING - BUTTON_WIDTH;
		toolbarButtons[i].y = TOOLBAR_MARGIN + TOOLBAR_PADDING + (BUTTON_HEIGHT + TOOLBAR_PADDING) * i;

		toolbarButtons[i].onHoverCallback = &toolBarButton_OnHover;
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
}

void zoomOutButton_OnClick(ToolbarButton *button)
{
	int newFontSize = getFont() - 4;
	setFont(newFontSize);

	if (newFontSize == 8)
		button->isEnabled = 0;
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

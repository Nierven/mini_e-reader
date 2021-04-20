#include "global.h"

static void (*localUiDisplayHandlers[5])(void);
static void (*localUiLogicHandlers[5])(void);

void init(void)
{
//	initEthernet();
	initScreen();
	initLogic();
	initSD();

	initUISplash();
	initUIBookshelf();
	initUIBrowse();
	initUIReader();

	localUiDisplayHandlers[0] = NULL;
	localUiDisplayHandlers[1] = &displayUISplash;
	localUiDisplayHandlers[2] = &displayUIBookshelf;
	localUiDisplayHandlers[3] = &displayUIBrowse;
	localUiDisplayHandlers[4] = &displayUIReader;

	localUiLogicHandlers[0] = NULL;
	localUiLogicHandlers[1] = &uiSplashLogicHandler;
	localUiLogicHandlers[2] = &uiBookshelfLogicHandler;
	localUiLogicHandlers[3] = &uiBrowseLogicHandler;
	localUiLogicHandlers[4] = &uiReaderLogicHandler;

	uiDisplayHandlers = localUiDisplayHandlers;
	uiLogicHandlers = localUiLogicHandlers;

	setActualPerspective(Splash);
}

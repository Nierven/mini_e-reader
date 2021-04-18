#include "SD.h"
#include <string.h>

void initSD(void)
{
	uint32_t byteswritten;
	uint8_t wtext[] = "plop";
	uint8_t rtext[_MAX_SS];

	f_mount(&SDFatFS, (TCHAR const*)SDPath, 0);
	f_open(&SDFile, "TEST.TXT", FA_CREATE_ALWAYS | FA_WRITE);
	f_write(&SDFile, wtext, strlen((char *)wtext), (void *)&byteswritten);
	f_close(&SDFile);
//	f_mount(&SDFatFS, (TCHAR const*)NULL, 0);
}

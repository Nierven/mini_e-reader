#include "SD.h"
#include <string.h>

void initSD(void)
{
	f_mount(&SDFatFS, (TCHAR const*)SDPath, 0);
}

void readFile(char *filename, uint8_t *buffer, uint32_t offset, uint32_t length, UINT *bytesRead)
{
	f_open(&SDFile, filename, FA_READ);
	f_lseek(&SDFile, offset);
	f_read(&SDFile, buffer, length, bytesRead);
	f_close(&SDFile);
}

void writeFile(char *filename, uint8_t *buffer, uint32_t offset, uint32_t length, UINT *bytesWritten)
{
	f_open(&SDFile, filename, FA_WRITE | FA_CREATE_ALWAYS);
	f_write(&SDFile, buffer + offset, length, bytesWritten);
	f_close(&SDFile);
}

void deleteFile(char *filename)
{
	f_unlink(filename);
}

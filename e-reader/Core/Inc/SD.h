#ifndef SD_H
#define SD_H

#include "cmsis_os.h"
#include "fatfs.h"

void initSD(void);
void readFile(char *filename, uint8_t *buffer, uint32_t offset, uint32_t length, UINT *bytesRead);
void writeFile(char *filename, uint8_t *buffer, uint32_t offset, uint32_t length, UINT *bytesWritten);

#endif // SD_H

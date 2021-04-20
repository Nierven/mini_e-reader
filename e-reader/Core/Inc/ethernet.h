#ifndef ETHERNET_H
#define ETHERNET_H

#include "cmsis_os.h"
#include "lwip.h"

void getStructOnline(uint8_t *buffer, uint32_t length, char *link, uint32_t *bytesDownloaded);
void getFileOnline(uint8_t *buffer, uint32_t length, char *link, uint32_t *bytesDownloaded);

#endif // ETHERNET_H

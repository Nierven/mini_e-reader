#ifndef ETHERNET_H
#define ETHERNET_H

#include "cmsis_os.h"
#include "lwip.h"
#include "lwip/api.h"

typedef struct netbuf netbuf_t;

void getStructOnline(uint8_t *buffer, char *link, uint32_t *bytesDownloaded);
void getFileOnline(uint8_t *buffer, char *link, uint32_t *bytesDownloaded);
void getFileOnlineCallback(char *link, void (*callback)(netbuf_t*));

#endif // ETHERNET_H

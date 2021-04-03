#ifndef ETHERNET_H
#define ETHERNET_H

#include "cmsis_os.h"

void initEthernet(void);
void ethernetHandler(void);

extern char book[256000];
extern uint32_t bookSize;

#endif // ETHERNET_H

#ifndef ETHERNET_H
#define ETHERNET_H

#include "cmsis_os.h"
#include "global.h"

void initEthernet(void);
void ethernetHandler(void);

extern char book[MAX_BOOK_SIZE];
extern uint32_t bookSize;

#endif // ETHERNET_H

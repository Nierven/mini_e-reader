#include "ethernet.h"
#include "book.h"
#include <string.h>
#include "lwip/api.h"

#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"

static uint8_t initialized = 0;

void initEthernet(void)
{
	initialized = 1;
}

void ethernetHandler(void)
{
	if (initialized == 0)
	{
		osDelay(100);
		return;
	}

	uint32_t a;
	uint8_t addr_dest[4] = { 138, 68, 187, 163 };
	char text[600] = "";

	static struct netconn *conn_emission;
	static struct netbuf *buf;
	static ip_addr_t *addr;
	static unsigned short port_dest = 80;

	sprintf(text, "Connection to %d.%d.%d.%d:%d", addr_dest[0], addr_dest[1], addr_dest[2], addr_dest[3], port_dest);
	BSP_LCD_DisplayStringAtLine(2, (uint8_t*) text);

	addr = &a;
	addr->addr = ((uint32_t) addr_dest[3] << 24) + ((uint32_t) addr_dest[2] << 16) + ((uint32_t) addr_dest[1] << 8) + ((uint32_t) addr_dest[0]);
	conn_emission = netconn_new(NETCONN_TCP);
	buf = netbuf_new();

	netconn_connect(conn_emission, addr, port_dest);
	BSP_LCD_DisplayStringAtLine(3, (uint8_t*) "Connected");

	char data[] = "GET / HTTP/1.1\r\nHost: gutendex.com\r\nConnection: close\r\n\r\n";
	netconn_write(conn_emission, (uint8_t*) data, strlen(data), NETCONN_NOCOPY);

	sprintf(text, "%d bytes written", strlen(data));
	BSP_LCD_DisplayStringAtLine(4, (uint8_t*) text);

	netconn_recv(conn_emission, &buf);

	sprintf(text, "%d bytes received", netbuf_len(buf));
	BSP_LCD_DisplayStringAtLine(6, (uint8_t*) text);

	netbuf_copy(buf, text, netbuf_len(buf));
	text[netbuf_len(buf)] = '\0';
	netbuf_delete(buf);

	BSP_LCD_DisplayStringAtLine(7,  (uint8_t*) text);

	for (;;)
	{
		osDelay(1000);
	}
}

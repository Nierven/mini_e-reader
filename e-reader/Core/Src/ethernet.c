#include "ethernet.h"
#include <string.h>
#include "lwip/api.h"

void getStructOnline(uint8_t *buffer, uint32_t length, char *link, uint32_t *bytesDownloaded)
{
	uint32_t a;
	uint8_t addr_dest[4] = { 138, 68, 187, 163 };

	static struct netconn *conn_emission;
	static struct netbuf *buf;
	static ip_addr_t *addr;
	static unsigned short port_dest = 80;

	addr = &a;
	addr->addr = ((uint32_t) addr_dest[3] << 24) + ((uint32_t) addr_dest[2] << 16) + ((uint32_t) addr_dest[1] << 8) + ((uint32_t) addr_dest[0]);
	conn_emission = netconn_new(NETCONN_TCP);
	buf = netbuf_new();

	netconn_connect(conn_emission, addr, port_dest);

	char data[200];
	sprintf(data, "GET %s HTTP/1.1\r\nHost: gutendex.com\r\nConnection: close\r\n\r\n", link);
	netconn_write(conn_emission, (uint8_t*) data, strlen(data), NETCONN_NOCOPY);
	netconn_recv(conn_emission, &buf);

	*bytesDownloaded = netbuf_len(buf);
	netbuf_copy(buf, buffer, *bytesDownloaded);
	buffer[*bytesDownloaded] = '\0';
	netbuf_delete(buf);
}

void getFileOnline(uint8_t *buffer, uint32_t length, char *link, uint32_t *bytesDownloaded)
{
	uint32_t a;
	uint8_t addr_dest[4] = { 152, 19, 134, 47 };

	static struct netconn *conn_emission;
	static struct netbuf *buf;
	static ip_addr_t *addr;
	static unsigned short port_dest = 80;

	addr = &a;
	addr->addr = ((uint32_t) addr_dest[3] << 24) + ((uint32_t) addr_dest[2] << 16) + ((uint32_t) addr_dest[1] << 8) + ((uint32_t) addr_dest[0]);
	conn_emission = netconn_new(NETCONN_TCP);
	buf = netbuf_new();

	netconn_connect(conn_emission, addr, port_dest);

	char data[200];
	sprintf(data, "GET %s HTTP/1.1\r\nHost: www.gutenberg.org\r\nConnection: close\r\n\r\n", link);
	netconn_write(conn_emission, (uint8_t*) data, strlen(data), NETCONN_NOCOPY);
	netconn_recv(conn_emission, &buf);

	*bytesDownloaded = netbuf_len(buf);
	netbuf_copy(buf, buffer, *bytesDownloaded);
	buffer[*bytesDownloaded] = '\0';
	netbuf_delete(buf);
}

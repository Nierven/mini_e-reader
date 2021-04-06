#include <string.h>

#include "ethernet.h"
#include "book.h"

void initEthernet(void)
{
//	__HAL_RCC_ETHMAC_CLK_ENABLE();
//	__HAL_RCC_ETHMACTX_CLK_ENABLE();
//	__HAL_RCC_ETHMACRX_CLK_ENABLE();
}

void ethernetHandler(void)
{
	uint8_t data[] = { 0x42, 0x4d, 0x00, 0xFF, 0x01 };

	ETH_DMADescTypeDef t;
	t.Buffer1Addr = data;

	HAL_ETH_DMATxDescListInit(&heth, &t);
	HAL_ETH_Start(&heth);
	HAL_ETH_TransmitFrame(&heth, 5);

//	HAL_ETH_DMARxDescListInit();
//	HAL_ETH_GetReceivedFrame();
//	osDelay(1);
}

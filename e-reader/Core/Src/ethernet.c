#include "ethernet.h"
#include <string.h>

char book[256000] = "";
uint32_t bookSize = 0;

void initEthernet(void)
{
	char *test = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed ultrices orci eu magna porta, in feugiat lectus convallis. Fusce id rhoncus mauris. Duis maximus lorem eget est interdum tempus. Aliquam tempus condimentum nulla, sit amet consectetur elit ultricies sit amet. Nulla vel egestas nibh. Vivamus suscipit faucibus porttitor. Nullam non orci id nisl accumsan semper in in mi. Proin congue dolor ac ligula egestas, sed consequat elit aliquet. Ut turpis nulla, consequat eu tortor at, fermentum gravida justo. In ac magna sed mi dictum fermentum eu eget urna.\n\n"
				 "Mauris molestie, ligula eget tincidunt volutpat, ligula nibh viverra nulla, vitae tincidunt turpis mi vel ligula. Donec ac purus vel tortor dapibus bibendum id in lectus. Quisque finibus feugiat justo, ut iaculis lacus posuere in. Proin dignissim quam ut interdum varius. Quisque tempor neque nibh, eu rhoncus odio iaculis at. Vivamus consequat vehicula dolor, sit amet suscipit mi mattis vel. Etiam id augue non risus egestas iaculis at non metus. Curabitur vel orci orci.\n\n";
//				 "Fusce sit amet interdum lorem. Aliquam erat volutpat. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Sed hendrerit non metus quis iaculis. Praesent iaculis venenatis dolor, non convallis sem euismod sed. Sed dictum enim et metus molestie, a iaculis purus cursus. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Donec sit amet orci justo. Sed at vehicula lorem. Etiam lacinia magna at urna finibus accumsan. Suspendisse id risus nulla. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Phasellus tortor ex, bibendum ut mi et, accumsan pellentesque quam. In rhoncus magna venenatis erat eleifend euismod.\n\n"
//				 "Cras non metus finibus, gravida nunc in, cursus purus. Interdum et malesuada fames ac ante ipsum primis in faucibus. Donec semper cursus nisl, et luctus odio eleifend dignissim. Nullam nec lorem tempus, congue eros rutrum, mattis nunc. Morbi semper sodales condimentum. Nulla vulputate neque in sapien dignissim finibus. Quisque venenatis, neque vitae euismod porta, quam mi pharetra erat, quis ultricies ligula erat eu purus. Quisque et tincidunt ligula, a pharetra justo.\n\n"
//				 "Vestibulum non augue non nibh commodo auctor. Pellentesque lobortis viverra urna ac ultricies. Orci varius natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Maecenas magna ante, ultricies vel neque ac, efficitur maximus augue. Proin interdum dui ac eros vulputate, posuere condimentum tortor imperdiet. Morbi eu velit quis enim pellentesque aliquet. Suspendisse luctus congue turpis ut dapibus.\n\n";

	bookSize = strlen(test);
	strcpy(book, test);

//	for (int i = 0; i < bookSize; i++)
//		if (book[i] == '\n')
//			book[i] = '\0';
}

void ethernetHandler(void)
{
	osDelay(1);
}

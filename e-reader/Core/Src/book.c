#include "book.h"
#include <string.h>
#include <math.h>

Book book;

void initBook(void)
{
	book.size = 0;
	book.text[0] = '\0';

	book.offset = 0;
	book.linesSize = 0;

	// Testing zone
	char *test = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed ultrices orci eu magna porta, in feugiat lectus convallis. Fusce id rhoncus mauris. Duis maximus lorem eget est interdum tempus. Aliquam tempus condimentum nulla, sit amet consectetur elit ultricies sit amet. Nulla vel egestas nibh. Vivamus suscipit faucibus porttitor. Nullam non orci id nisl accumsan semper in in mi. Proin congue dolor ac ligula egestas, sed consequat elit aliquet. Ut turpis nulla, consequat eu tortor at, fermentum gravida justo. In ac magna sed mi dictum fermentum eu eget urna.\n\n"
				 "Mauris molestie, ligula eget tincidunt volutpat, ligula nibh viverra nulla, vitae tincidunt turpis mi vel ligula. Donec ac purus vel tortor dapibus bibendum id in lectus. Quisque finibus feugiat justo, ut iaculis lacus posuere in. Proin dignissim quam ut interdum varius. Quisque tempor neque nibh, eu rhoncus odio iaculis at. Vivamus consequat vehicula dolor, sit amet suscipit mi mattis vel. Etiam id augue non risus egestas iaculis at non metus. Curabitur vel orci orci.\n\n"
				 "Fusce sit amet interdum lorem. Aliquam erat volutpat. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Sed hendrerit non metus quis iaculis. Praesent iaculis venenatis dolor, non convallis sem euismod sed. Sed dictum enim et metus molestie, a iaculis purus cursus. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Donec sit amet orci justo. Sed at vehicula lorem. Etiam lacinia magna at urna finibus accumsan. Suspendisse id risus nulla. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Phasellus tortor ex, bibendum ut mi et, accumsan pellentesque quam. In rhoncus magna venenatis erat eleifend euismod.\n\n"
				 "Cras non metus finibus, gravida nunc in, cursus purus. Interdum et malesuada fames ac ante ipsum primis in faucibus. Donec semper cursus nisl, et luctus odio eleifend dignissim. Nullam nec lorem tempus, congue eros rutrum, mattis nunc. Morbi semper sodales condimentum. Nulla vulputate neque in sapien dignissim finibus. Quisque venenatis, neque vitae euismod porta, quam mi pharetra erat, quis ultricies ligula erat eu purus. Quisque et tincidunt ligula, a pharetra justo.\n\n"
				 "Vestibulum non augue non nibh commodo auctor. Pellentesque lobortis viverra urna ac ultricies. Orci varius natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Maecenas magna ante, ultricies vel neque ac, efficitur maximus augue. Proin interdum dui ac eros vulputate, posuere condimentum tortor imperdiet. Morbi eu velit quis enim pellentesque aliquet. Suspendisse luctus congue turpis ut dapibus.\n\n";

	book.size = strlen(test);
	strcpy(book.text, test);
}

void buildBook(uint16_t width, uint16_t charWidth)
{
	int currentLineNb = 0;
	int currentLineIndex = 0;
	int lettersCounter = 0;
	int wordLettersCounter = 0;
	int spacesCounter = 0;

	const int minSpace = round(0.7 * charWidth);
	const int maxSpace = round(2.0 * charWidth);

	for (int i = 0; i < book.size; i++)
	{
		if (book.text[i] == ' ' || book.text[i] == '\n')
		{
			// Get the optimal space length according to how many letters and space we have
			int actualOptimalSpace = width - lettersCounter * charWidth;
			if (spacesCounter != 0) actualOptimalSpace /= spacesCounter;

			// If too small, then discard the last word and compute the best length
			if (actualOptimalSpace < minSpace)
			{
				// Discard the last word
				lettersCounter -= wordLettersCounter;
				spacesCounter--;

				// Compute the best length and the error and its adjustements
				actualOptimalSpace = (width - lettersCounter * charWidth) / spacesCounter;
				int error = 0; int pxPerSpace = 0; int pxPerFirstSpaces = 0;
				if (spacesCounter > 0)
				{
					error = width - lettersCounter * charWidth - spacesCounter * actualOptimalSpace;
					pxPerSpace = error / spacesCounter; pxPerFirstSpaces = error % spacesCounter;
				}

				// Register the new line
			 	book.lines[currentLineNb].index = currentLineIndex;
			 	book.lines[currentLineNb].length = lettersCounter + spacesCounter;
			 	book.lines[currentLineNb].spaceSize = actualOptimalSpace;
				book.lines[currentLineNb].additionalPixelPerSpace = pxPerSpace;
				book.lines[currentLineNb].additionalPixelPerFirstSpaces = pxPerFirstSpaces;
				currentLineNb++;

				// Update the indices
				currentLineIndex = currentLineIndex + lettersCounter + spacesCounter + 1;
				i = currentLineIndex - 1;

				// Reset the counters
				lettersCounter = 0;
				wordLettersCounter = 0;
				spacesCounter = 0;
			}
			else if (book.text[i] == '\n')
			{
				// If too much space left, make the width normal
				if (actualOptimalSpace > maxSpace)
					actualOptimalSpace = charWidth;

				// Register the new line
			 	book.lines[currentLineNb].index = currentLineIndex;
			 	book.lines[currentLineNb].length = lettersCounter + spacesCounter;
			 	book.lines[currentLineNb].spaceSize = actualOptimalSpace;
				book.lines[currentLineNb].additionalPixelPerSpace = 0;
				book.lines[currentLineNb].additionalPixelPerFirstSpaces = 0;
				currentLineNb++;

				// Update the indices
				currentLineIndex = i+1;

				// Reset the counters
				lettersCounter = 0;
				wordLettersCounter = 0;
				spacesCounter = 0;
			}
			else
			{
				spacesCounter++;
				wordLettersCounter = 0;
			}
		}
		else
		{
			wordLettersCounter++;
			lettersCounter++;
		}
	}

	book.linesSize = currentLineNb;
}

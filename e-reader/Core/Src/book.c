#include "book.h"
#include <string.h>
#include <math.h>

Book book;

void initBook(void)
{
	book.name[0] = '\0';
	book.author[0] = '\0';
	book.publicationDate = 0;
	book.hasDate = 0;

	book.size = 0;
	book.text[0] = '\0';

	book.offset = 0;
	book.linesSize = 0;
}

void openBook(char *filename)
{
	UINT bytesRead = 0;
	readFile(filename, (uint8_t*) book.text, 0, MAX_LOADED_BOOK_SIZE, &bytesRead);

	book.size = bytesRead;
	book.text[book.size] = '\0';

//	for (int i = 0; i < book.size; i++)
//	{
//		if (book.text[i] == '\r' && book.text[i+1] == '\n')
//		{
//			book.text[i] = 0x0E; i++;
//			if (book.text[i+1] != '\r' || book.text[i+2] != '\n')
//			{
//				book.text[i] = ' ';
//			}
//			else
//			{
//				book.text[i+1] = 0x0E;
//				i += 2;
//			}
//		}
//	}

//	strcpy(book.name, "Test book");
//	strcpy(book.author, "Quentin Rousset");
//	book.publicationDate = 2021;
//	book.hasDate = 1;
}

void buildBook(uint16_t width, uint16_t charWidth)
{
	int32_t currentLineNb = 0;
	int32_t currentLineIndex = 0;
	int8_t lettersCounter = 0;
	int8_t wordLettersCounter = 0;
	int8_t spacesCounter = 0;

	const int16_t minSpace = round(0.7 * charWidth);
	const int16_t maxSpace = round(2.0 * charWidth);
	const int16_t maxNbOfChars = width / charWidth;

	for (int32_t i = 0; i < book.size + 1; i++)
	{
		if (book.text[i] == ' ' || book.text[i] == '\n' || book.text[i] == '\0' || wordLettersCounter > maxNbOfChars)
		{
			// Get the optimal space length according to how many letters and space we have
			int actualOptimalSpace = width - lettersCounter * charWidth;
			if (spacesCounter != 0) actualOptimalSpace /= spacesCounter;

			// If too small, then discard the last word and compute the best length
			if (wordLettersCounter >= maxNbOfChars)
			{
				// Register the new line
			 	book.lines[currentLineNb].index = currentLineIndex;
			 	book.lines[currentLineNb].length = lettersCounter - 1;
			 	book.lines[currentLineNb].spaceSize = 0;
				book.lines[currentLineNb].additionalPixelPerFirstSpaces = 0;
				currentLineNb++;

				// Update the indices
				if (wordLettersCounter > maxNbOfChars)
					currentLineIndex = i;
				else if (wordLettersCounter == maxNbOfChars)
					currentLineIndex = i+1;

				// Reset the counters
				lettersCounter = 0;
				wordLettersCounter = 0;
				spacesCounter = 0;
			}
			else if (actualOptimalSpace < minSpace)
			{
				// Discard the last word
				lettersCounter -= wordLettersCounter;
				spacesCounter--;

				// Compute the best length and the error and its adjustements
				actualOptimalSpace = (width - lettersCounter * charWidth) / spacesCounter;
				int8_t error = 0; int8_t pxPerFirstSpaces = 0;
				if (spacesCounter > 0)
				{
					error = width - lettersCounter * charWidth - spacesCounter * actualOptimalSpace;
					pxPerFirstSpaces = error % spacesCounter;
				}

				// Register the new line
			 	book.lines[currentLineNb].index = currentLineIndex;
			 	book.lines[currentLineNb].length = lettersCounter + spacesCounter;
			 	book.lines[currentLineNb].spaceSize = actualOptimalSpace;
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
			else if (book.text[i] == '\n' || book.text[i] == '\0')
			{
				// If too much space left, make the width normal
				if (actualOptimalSpace > maxSpace)
					actualOptimalSpace = charWidth;

				// Register the new line
			 	book.lines[currentLineNb].index = currentLineIndex;
			 	book.lines[currentLineNb].length = lettersCounter + spacesCounter;
			 	book.lines[currentLineNb].spaceSize = actualOptimalSpace;
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

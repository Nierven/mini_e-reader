#include "book.h"
#include <string.h>
#include <math.h>

Book loadedBook;
BookInfo booksInfo[50];

void initBook(void)
{
	loadedBook.size = 0;
	loadedBook.linesSize = 0;
}

void initBookInfo(BookInfo *info)
{
	info->name[0] = '\0';
	info->author[0] = '\0';
	info->language[0] = '\0';
	info->publicationDate = 0;
	info->hasDate = 0;

	info->offset = 0;
}

int32_t getInfoLength(char *ptr, int32_t offset)
{
	char *start = ptr + offset;
	int i;
	for (i = 0; i < 50; i++)
		if (start[i] == '\n')
			break;

	return i;
}

void readBookInfo(char *filename, BookInfo *info)
{
	const int32_t header_size = 2048;
	char data[header_size];

	UINT bytesRead = 0;
	readFile(filename, (uint8_t*) data, 0, header_size, &bytesRead);

	if (bytesRead == 0)
		return;

	initBookInfo(info);

	char nameHeader[] = "Title: ";
	char authorHeader[] = "Author: ";
	char dateHeader[] = "Release Date: ";
	char languageHeader[] = "Language: ";
	char startHeader[] = " ***";

	char *namePtr = strstr(data, nameHeader);
	if (namePtr != NULL)
	{
		int32_t nameLength = getInfoLength(namePtr, strlen(nameHeader));
		memcpy(info->name, namePtr + strlen(nameHeader), nameLength); info->name[nameLength] = '\0';
	}
	else
	{
		strcpy(info->name, "Unnamed");
	}

	char *authorPtr = strstr(data, authorHeader);
	if (authorPtr != NULL)
	{
		int32_t authorLength = getInfoLength(authorPtr, strlen(authorHeader));
		memcpy(info->author, authorPtr + strlen(authorHeader), authorLength); info->author[authorLength] = '\0';
	}

	char *datePtr = strstr(data, dateHeader);
	if (datePtr != NULL)
	{
		char *yearPtr = strstr(datePtr, ", ");
		char *yearEndPtr = strstr(datePtr, " [");

		if (yearPtr != NULL && yearEndPtr != NULL)
		{
			char tmp[10] = "";
			memcpy(tmp, yearPtr + 2, yearEndPtr - yearPtr + 2);
			tmp[yearEndPtr - yearPtr + 2] = '\0';
			info->publicationDate = atoi(tmp);
			info->hasDate = 1;
		}
	}

	char *languagePtr = strstr(data, languageHeader);
	if (languagePtr != NULL)
	{
		char tmp[50] = "";
		int32_t languageLength = getInfoLength(languagePtr, strlen(languageHeader));
		memcpy(tmp, languagePtr + strlen(languageHeader), languageLength); tmp[languageLength] = '\0';

		if (strcmp(tmp, "English") == 0)
			sprintf(info->language, "en");
		else if (strcmp(tmp, "French") == 0)
			sprintf(info->language, "fr");
	}

	char *startPtr = strstr(namePtr, startHeader);
	info->offset = startPtr + strlen(startHeader) + 1 - data;
}

void openBook(char *filename, BookInfo *info)
{
	UINT bytesRead = 0;
	readFile(filename, (uint8_t*) loadedBook.text, 0, MAX_LOADED_BOOK_SIZE, &bytesRead);

	if (bytesRead == 0)
		return;

	initBook();

	loadedBook.size = bytesRead;
	loadedBook.text[loadedBook.size] = '\0';
	loadedBook.info = info;

//	for (int i = 0; i < book.size; i++)
//	{
//		if (book.text[i] == '\r' && loadedBook.text[i+1] == '\n')
//		{
//			loadedBook.text[i] = 0x0E; i++;
//			if (loadedBook.text[i+1] != '\r' || loadedBook.text[i+2] != '\n')
//			{
//				loadedBook.text[i] = ' ';
//			}
//			else
//			{
//				loadedBook.text[i+1] = 0x0E;
//				i += 2;
//			}
//		}
//	}
}

void buildBook(uint16_t width, uint16_t charWidth)
{
	int32_t currentLineIndex = loadedBook.info->offset;
	int32_t currentLineNb = 0;
	int8_t lettersCounter = 0;
	int8_t wordLettersCounter = 0;
	int8_t spacesCounter = 0;

	const int16_t minSpace = round(0.7 * charWidth);
	const int16_t maxSpace = round(2.0 * charWidth);
	const int16_t maxNbOfChars = width / charWidth;

	for (int32_t i = currentLineIndex; i < loadedBook.size + 1; i++)
	{
		if (loadedBook.text[i] == ' ' || loadedBook.text[i] == '\n' || loadedBook.text[i] == '\0' || wordLettersCounter > maxNbOfChars)
		{
			// Get the optimal space length according to how many letters and space we have
			int actualOptimalSpace = width - lettersCounter * charWidth;
			if (spacesCounter != 0) actualOptimalSpace /= spacesCounter;

			// If too small, then discard the last word and compute the best length
			if (wordLettersCounter >= maxNbOfChars)
			{
				// Register the new line
			 	loadedBook.lines[currentLineNb].index = currentLineIndex;
			 	loadedBook.lines[currentLineNb].length = lettersCounter - 1;
			 	loadedBook.lines[currentLineNb].spaceSize = 0;
				loadedBook.lines[currentLineNb].additionalPixelPerFirstSpaces = 0;
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
			 	loadedBook.lines[currentLineNb].index = currentLineIndex;
			 	loadedBook.lines[currentLineNb].length = lettersCounter + spacesCounter;
			 	loadedBook.lines[currentLineNb].spaceSize = actualOptimalSpace;
			 	loadedBook.lines[currentLineNb].additionalPixelPerFirstSpaces = pxPerFirstSpaces;
				currentLineNb++;

				// Update the indices
				currentLineIndex = currentLineIndex + lettersCounter + spacesCounter + 1;
				i = currentLineIndex - 1;

				// Reset the counters
				lettersCounter = 0;
				wordLettersCounter = 0;
				spacesCounter = 0;
			}
			else if (loadedBook.text[i] == '\n' || loadedBook.text[i] == '\0')
			{
				// If too much space left, make the width normal
				if (actualOptimalSpace > maxSpace)
					actualOptimalSpace = charWidth;

				// Register the new line
			 	loadedBook.lines[currentLineNb].index = currentLineIndex;
			 	loadedBook.lines[currentLineNb].length = lettersCounter + spacesCounter;
			 	loadedBook.lines[currentLineNb].spaceSize = actualOptimalSpace;
				loadedBook.lines[currentLineNb].additionalPixelPerFirstSpaces = 0;
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

	loadedBook.linesSize = currentLineNb;
}

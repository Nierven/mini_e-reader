#ifndef BOOK_H
#define BOOK_H

#include "SD.h"
#include <stdlib.h>
#include <stdio.h>

#define MAX_LOADED_BOOK_SIZE 32765

typedef struct
{
	int32_t index;
	int16_t length;

	int8_t spaceSize;
	int8_t additionalPixelPerFirstSpaces;
} Line;

typedef struct
{
	char name[50];
	char author[50];
	char language[4];
	int16_t publicationDate;
	uint8_t hasDate;
} BookInfo;

typedef struct
{
	BookInfo info;

	int32_t size;
	char text[MAX_LOADED_BOOK_SIZE + 1];

	int32_t linesSize;
	Line lines[MAX_LOADED_BOOK_SIZE / 8];

	int32_t offset;
} Book;

extern Book book;

void initBook(void);
void openBook(char *filename);
void buildBook(uint16_t width, uint16_t charWidth);

#endif // BOOK_H

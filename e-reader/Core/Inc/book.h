#ifndef BOOK_H
#define BOOK_H

#include <stdlib.h>
#include <stdio.h>

#define MAX_LOADED_BOOK_SIZE 32765

typedef struct
{
	int32_t index;
	int8_t length;

	int8_t spaceSize;
	int8_t additionalPixelPerFirstSpaces;
} Line;

typedef struct
{
	int32_t size;
	char text[MAX_LOADED_BOOK_SIZE];

	int32_t linesSize;
	Line lines[MAX_LOADED_BOOK_SIZE / 8];

	int32_t offset;
} Book;

extern Book book;

void initBook(void);
void buildBook(uint16_t width, uint16_t charWidth);

#endif // BOOK_H

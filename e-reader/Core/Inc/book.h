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

	int32_t offset;
} BookInfo;

typedef struct
{
	BookInfo *info;

	int32_t size;
	char text[MAX_LOADED_BOOK_SIZE + 1];

	int32_t linesSize;
	Line lines[MAX_LOADED_BOOK_SIZE / 8];
} Book;

extern Book loadedBook;
extern BookInfo booksInfo[50];

void initBook(void);
void initBookInfo(BookInfo *info);
void readBookInfo(char *filename, BookInfo *info);
void openBook(char *filename, BookInfo *info);
void buildBook(uint16_t width, uint16_t charWidth);

#endif // BOOK_H

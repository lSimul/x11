#include "pixel.h"

#include <stdio.h>
#include <stdlib.h>

#ifndef _FILE_H_
#define _FILE_H_

// Can be changed to any meaningful
// value, number 10 is a nice testing
// size, ensures that buffer will have
// to be filled multiple times.
#define CHUNK_SIZE 10

typedef struct reader
{
	FILE *file;
	char dangling;
	long pos;
	long size;
	char buffer[CHUNK_SIZE];
	unsigned int bufferSize;
	unsigned int bufferPos;
} READER;

/**
 * @brief
 *
 * (Probably) casual bitmap format. 32 bits, like GIMP likes it from the start
 * 24 bit and the non-fill would be better, but no need, the format is that easy.
 *
 * @param bitmap
 * @param file
 * @return int
 */
int readBitmap(BITMAP *bitmap, const char *file);

/**
 * @brief
 *
 * @param bitmap
 * @param buffer
 * @param length
 * @return int
 */
int parseBitmap(BITMAP *bitmap, unsigned char *buffer, int length);

/**
 * @brief
 *
 * @param reader
 * @param file
 * @return int
 */
int openFile(READER *reader, const char *file);

/**
 * @brief
 *
 * @param reader
 * @return char
 */
char getChar(READER *reader);

/**
 * @brief Return char back to the reader.
 *
 * Finding newline while reading can change the outcome, char
 * has to be stored somewhere, so it can be read again.
 *
 * Right now it is used only for newlines (symbol of the fact that
 * command is ending), later on it can be used for quotes etc.
 *
 * Multiple ungets do not really work, it is not a stack.
 *
 * @param reader
 * @param c
 */
void ungetChar(READER *reader, char c);

#endif

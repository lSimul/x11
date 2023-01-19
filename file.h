#include "pixel.h"

typedef struct bitmap
{
	int size;
	int width;
	int height;
	PIXEL *data;
} BITMAP;

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
int readFile(BITMAP *bitmap, const char *file);

/**
 * @brief
 *
 * @param bitmap
 * @param buffer
 * @param length
 * @return int
 */
int readHeader(BITMAP *bitmap, unsigned char *buffer, int length);

#ifndef _PIXEL_H_
#define _PIXEL_H_

typedef struct pixel
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
} PIXEL;

typedef struct bitmap
{
	int size;
	int width;
	int height;
	PIXEL *data;
} BITMAP;

#endif

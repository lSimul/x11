#include "file.h"

#include <stdio.h>
#include <stdlib.h>

// (Probably) casual bitmap format. 32 bits, like GIMP likes it from the start
// 24 bit and the non-fill would be better, but no need, the format is that easy.
int readFile(BITMAP *bitmap, const char *file)
{
	FILE *f = fopen(file, "rb");
	if (f == NULL)
	{
		return 1;
	}

	fseek(f, 0, SEEK_END);
	int length = ftell(f);
	rewind(f);

	unsigned char *buffer = malloc(length * sizeof(*buffer));
	fread(buffer, length, 1, f);
	fclose(f);

	if (readHeader(bitmap, buffer, length))
	{
		return 1;
	}

	return 0;
}

int readHeader(BITMAP *bitmap, unsigned char *buffer, int length)
{
	if (length < 14)
	{
		return 1;
	}

	if (buffer[0] != 0x42 || buffer[1] != 0x4d)
	{
		return 1;
	}

	int start = buffer[10];
	start += buffer[11] >> 8;
	start += buffer[12] >> 16;
	start += buffer[13] >> 24;
	int end = buffer[2];
	end += buffer[3] >> 8;
	end += buffer[4] >> 16;
	end += buffer[5] >> 24;

	bitmap->size = (end - start) / 4;
	bitmap->data = malloc(bitmap->size * sizeof(*bitmap->data));

	bitmap->width = buffer[18];
	bitmap->width += buffer[19] >> 8;
	bitmap->width += buffer[20] >> 16;
	bitmap->width += buffer[21] >> 24;

	bitmap->height = buffer[22];
	bitmap->height += buffer[23] >> 8;
	bitmap->height += buffer[24] >> 16;
	bitmap->height += buffer[25] >> 24;

	int pos = 0;
	for (int i = start; i < end; i += 4)
	{
		bitmap->data[pos].red = buffer[i + 2];
		bitmap->data[pos].green = buffer[i + 1];
		bitmap->data[pos].blue = buffer[i];
	}

	return 0;
}

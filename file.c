#include "file.h"

// (Probably) casual bitmap format. 32 bits, like GIMP likes it from the start
// 24 bit and the non-fill would be better, but no need, the format is that easy.
int readBitmap(BITMAP *bitmap, const char *file)
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

	if (parseBitmap(bitmap, buffer, length))
	{
		return 1;
	}

	return 0;
}

int parseBitmap(BITMAP *bitmap, unsigned char *buffer, int length)
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
	start += buffer[11] << 8;
	start += buffer[12] << 16;
	start += buffer[13] << 24;
	int end = buffer[2];
	end += buffer[3] << 8;
	end += buffer[4] << 16;
	end += buffer[5] << 24;

	bitmap->size = (end - start) / 4;
	bitmap->data = malloc(bitmap->size * sizeof(*bitmap->data));

	bitmap->width = buffer[18];
	bitmap->width += buffer[19] << 8;
	bitmap->width += buffer[20] << 16;
	bitmap->width += buffer[21] << 24;

	bitmap->height = buffer[22];
	bitmap->height += buffer[23] << 8;
	bitmap->height += buffer[24] << 16;
	bitmap->height += buffer[25] << 24;

	int pos = 0;

	// TODO: Be more effective. I can store it by chunks size of |width|.
	//
	// Lazy way of transforming how the image is stored:
	// bottom left to top right -> top left to bottom right,
	{
		PIXEL *pixels = malloc(bitmap->size * sizeof(*pixels));
		for (int i = start; i < end; i += 4)
		{
			pixels[pos].red = buffer[i + 2];
			pixels[pos].green = buffer[i + 1];
			pixels[pos].blue = buffer[i];
			pos++;
		}
		int row = bitmap->height - 1;
		for (int y = 0; y < bitmap->height; y++)
		{
			for (int x = 0; x < bitmap->width; x++)
			{
				bitmap->data[row * bitmap->width + x] = pixels[y * bitmap->width + x];
			}
			row--;
		}
		free(pixels);
	}

	return 0;
}

int openFile(READER *reader, const char *file)
{
	FILE *f = fopen(file, "r");
	if (f == NULL)
	{
		return 1;
	}

	reader->file = f;
	reader->pos = 0;
	reader->dangling = 0;

	reader->bufferSize = 0;
	reader->bufferPos = 0;

	fseek(f, 0, SEEK_END);
	reader->size = ftell(f);
	rewind(f);

	return 0;
}

char getChar(READER *reader)
{
	if (reader->dangling != 0)
	{
		char c = reader->dangling;
		reader->dangling = 0;
		return c;
	}

	if (reader->bufferPos < reader->bufferSize)
	{
		return reader->buffer[reader->bufferPos++];
	}
	if (reader->pos < reader->size)
	{
		unsigned int readBytes = CHUNK_SIZE;
		if (reader->pos + CHUNK_SIZE > reader->size)
		{
			readBytes = reader->size - reader->pos;
		}

		reader->bufferSize = readBytes;
		reader->bufferPos = 0;
		fread(reader->buffer, readBytes * sizeof(*reader->buffer), sizeof(*reader->buffer), reader->file);
		reader->pos += readBytes;

		return reader->buffer[reader->bufferPos++];
	}

	return EOF;
}

void ungetChar(READER *reader, char c)
{
	reader->dangling = c;
}

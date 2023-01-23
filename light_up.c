#include "common.h"
#include "file.h"
#include "image.h"

#define OPTIONS 7

int main()
{
	const char *tiles[OPTIONS] = {
		"tiles/0.bmp",
		"tiles/1.bmp",
		"tiles/2.bmp",
		"tiles/3.bmp",
		"tiles/4.bmp",

		"tiles/block.bmp",
		"tiles/empty.bmp",
	};

	X_INSTANCE instance = {};
	instance.display = XOpenDisplay(NULL);
	if (instance.display == NULL)
	{
		printf("No display found.\n");
		return 1;
	}
	instance.window = XDefaultRootWindow(instance.display);
	XGetWindowAttributes(instance.display, instance.window, &instance.attrs);

	for (int i = 0; i < OPTIONS; i++)
	{
		BITMAP bitmap = {};
		if (readBitmap(&bitmap, tiles[i]))
		{
			printf("Fail while reading '%s'.\n", tiles[i]);
			return 1;
		}

		int size = 0;
		printf("!!! %s !!!\n", tiles[i]);
		COORDS *coords = findAllImages(&size, &bitmap, &instance);
		if (size == 0)
		{
			printf("Failed to find even a single occurence!\n");
		}
		free(bitmap.data);

		for (int i = 0; i < size; i++)
		{
			printf("X:\t%d\nY:\t%d\n\n", coords[i].x, coords[i].y);
		}
		free(coords);
	}
}

#include "common.h"
#include "file.h"
#include "image.h"

int main()
{
	BITMAP bitmap = {};
	if (readBitmap(&bitmap, "images/multiple_nightly.bmp"))
	{
		printf("Fail while reading '%s'.\n", "images/multiple_nightly.bmp");
		return 1;
	}

	X_INSTANCE instance = {};
	instance.display = XOpenDisplay(NULL);
	if (instance.display == NULL)
	{
		printf("No display found.\n");
		return 1;
	}
	// Use only in C_findImage, but there is no
	// fear of initialiazing it anytime.
	instance.window = XDefaultRootWindow(instance.display);
	XGetWindowAttributes(instance.display, instance.window, &instance.attrs);

	int size = 0;
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

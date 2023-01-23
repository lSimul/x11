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

	COORDS c = {};

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

	findAllImages(&c, &bitmap, &instance);

	free(bitmap.data);
}

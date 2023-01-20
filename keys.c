#include "keys.h"

#include <stdlib.h>
#include <unistd.h>

void pressKey(Display *display, KeySym key)
{
	unsigned int keycode = XKeysymToKeycode(display, key);
	XTestFakeKeyEvent(display, keycode, True, CurrentTime);
	XTestFakeKeyEvent(display, keycode, False, CurrentTime);
	XFlush(display);
	XSync(display, False);
}

EVENT *press(Display *display, EVENT *previous, KeySym key)
{
	unsigned int code = XKeysymToKeycode(display, key);
	XTestFakeKeyEvent(display, code, True, CurrentTime);

	EVENT *e = malloc(sizeof(*e));
	e->display = display;
	e->keycode = code;
	e->previous = previous;

	return e;
}

void release(EVENT *event)
{
	if (event == NULL)
	{
		return;
	}

	XTestFakeKeyEvent(event->display, event->keycode, False, CurrentTime);
	if (event->previous == NULL)
	{
		XFlush(event->display);
		XSync(event->display, False);
	}
	else
	{
		release(event->previous);
	}
	free(event);
}

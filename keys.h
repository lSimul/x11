#ifndef _KEYS_H_
#define _KEYS_H_

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

/**
 * @brief
 *
 * @param display
 */
void pressKey(Display *display, KeySym);

typedef struct event
{
	Display *display;
	KeyCode keycode;
	struct event *previous;
} EVENT;

/**
 * @brief
 *
 * @param display
 * @param previous
 * @param key
 * @return EVENT*
 */
EVENT *press(Display *display, EVENT *previous, KeySym key);

/**
 * @brief
 *
 * @param event
 */
void release(EVENT *event);

/*
int main()
{
	Display *display = XOpenDisplay(NULL);
	pressKey(display, XK_A);

	EVENT *q = press(display, NULL, XK_Control_L);
	q = press(display, q, XK_Alt_L);
	q = press(display, q, XK_T);

	release(q);
}
*/

#endif

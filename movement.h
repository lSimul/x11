#include <X11/Xlib.h>
#include <X11/Xutil.h>

/**
 * @brief Code doing basically absolutely nothing.
 *
 * Represents one of the actions in the grammer, simple _click_.
 * No context, no movement of the mouse.
 *
 * It might be correct approach, moveAndClick <=> mouseMove + click.
 * Atlast in the grammer right now it makes perfect sense.
 *
 * @param display
 * @return int
 */
int click(Display *display);

/**
 * @brief Everything copied from https://stackoverflow.com/questions/20581343/how-to-move-the-mouse-cursor-from-user-code,
 * tweaked pretty heavily.
 *
 * https://tronche.com/gui/x/xlib/input/XWarpPointer.html
 *
 * Another source is:
 *
 * https://github.com/misusi/x11-keyboard-mouse-control/blob/master/xkbmouse.c
 *
 * Code is terrible, but it shows the way, how to use absolute positioning.
 *
 * @param x
 * @param y
 * @return
 */
int mouseMove(Display *display, Window *root, int x, int y);

/**
 * @brief
 *
 * Inspiration for a mouse click comes from
 * https://stackoverflow.com/questions/8767524/how-do-we-simulate-a-mouse-click-with-xlib-c
 *
 * @param display
 * @param root
 * @param x
 * @param y
 * @return int
 */
int moveAndClick(Display *display, Window *root, int x, int y);

/**
 * @brief
 *
 * @param display
 * @param root
 * @param x
 * @param y
 * @return int
 */
int moveAndRightClick(Display *display, Window *root, int x, int y);

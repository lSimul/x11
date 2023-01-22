#include "common.h"
#include "file.h"

// Old comment from nonexisting file, losing relevance every new commit.
//
//
// Main source of screen managing:
//
// https://stackoverflow.com/questions/8249669/how-do-take-a-screenshot-correctly-with-xlib
//
// It goes nicely from top-left to the bottom-right, column-wise.
// Bytes how they are right now fit the top of the
//
// !!! Important note !!!
// It does not start from position 0, 0 for me, but from 1, 1 for some reason.
// No need to be sad or something like that, one pixel is really nothing, but for some
// hardcore evaluation over multiple lines or fixed position it can cause some troubles.
//
// Read screen and based on the sequence of pixels
// click on the position. (Just sequence, context is not checked.)

/**
 * @brief
 *
 * @param result
 * @param needle
 * @param display
 * @param window
 * @return int
 */
int findImage(COORDS *result, BITMAP *needle, X_INSTANCE *instance);

/**
 * @brief
 *
 * @param img
 * @param reference
 * @param read
 * @return int
 */
static inline int comparePixels(XImage *img, PIXEL reference, unsigned long read);

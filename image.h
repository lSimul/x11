#include "common.h"
#include "file.h"

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

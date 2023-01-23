#include "common.h"
#include "file.h"

/**
 * @brief
 *
 * @param result
 * @param needle
 * @param instance
 * @return int
 */
int findAllImages(COORDS *result, BITMAP *needle, X_INSTANCE *instance);

/**
 * @brief
 *
 * Main source of screen managing:
 *
 * https://stackoverflow.com/questions/8249669/how-do-take-a-screenshot-correctly-with-xlib
 *
 * It does not start from position 0, 0 for me, but from 1, 1 for some reason.
 * No need to be sad or something like that, one pixel is really nothing, but for some
 * hardcore evaluation over multiple lines or fixed position it can cause some troubles.
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
 * @param result
 * @param needle
 * @param haystack
 * @return int
 */
int findImageRow(COORDS *result, BITMAP *needle, XImage *haystack);

/**
 * @brief The most naive approach to looking for 2D image
 *
 * It does not use any searching optimizations, after first matching pixel
 * we go full throttle into the findComplexImgeInner.
 *
 * Only one optimization is done, check for image size (needle outside of haystack
 * is forbidden), which is more of a sanity check than an optimization.
 *
 * @todo Avoid more checks. When x is too big, x + 1 is also too big.
 * @todo With complex knowledge of image, I should be able to skip bigger parts of the image
 * (idea of the searching algorithm – movement can be faster, because needle cannot be _here_).
 * @todo What about targeting smaller parts of the haystack?
 *
 * @param result
 * @param needle
 * @param haystack
 * @param instance
 * @return int
 */
int findComplexImage(COORDS *result, BITMAP *needle, XImage *haystack, X_INSTANCE *instance);

/**
 * @brief
 *
 * Uses the same idea as the findImageRow, but the tides are turned, reference is a haystack, not
 * a needle.
 *
 * @param result
 * @param needle
 * @param haystack
 * @param offsetX
 * @param offsetY
 * @return int
 */
int findComplexImageInner(COORDS *result, BITMAP *needle, XImage *haystack, int offsetX, int offsetY);

/**
 * @brief
 *
 * @param img
 * @param reference
 * @param read
 * @return int
 */
static inline int comparePixel(XImage *img, PIXEL reference, unsigned long read);

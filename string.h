#ifndef _STRING_H_
#define _STRING_H_

typedef struct string
{
	int length;
	int capacity;
	char *data;
} STRING;

/**
 * @brief
 *
 * @param s
 * @param c
 */
void appendToString(STRING *s, char c);

/**
 * @brief
 *
 * @param string
 */
void enlargeString(STRING *string);

#endif

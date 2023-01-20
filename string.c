#include "string.h"

#include <stdlib.h>

void appendToString(STRING *s, char c)
{
	s->data[s->length++] = c;

	if (s->capacity == s->length)
	{
		enlargeString(s);
	}
}

void enlargeString(STRING *string)
{
	char *data = malloc(string->capacity * 2 * sizeof(*data));
	for (unsigned int i = 0; i < string->length; i++)
	{
		data[i] = string->data[i];
	}
	string->capacity *= 2;

	string->data = data;
}

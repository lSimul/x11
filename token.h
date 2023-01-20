#include "string.h"

#ifndef _TOKEN_H_
#define _TOKEN_H_

#define TOKEN_STARTING_SIZE 10

typedef enum type
{
	EMPTY,

	TEXT,

	// "PRESS"
	PRESS,

	// "SEQUENCE"
	SEQUENCE,

	// COMMAND is used twice here, as a struct.
	// "COMMAND",
	T_COMMAND,

	// "FIND"
	FIND,

	// "MOVE"
	MOVE,

	// "MOUSE"
	MOUSE,

	// "CLICK"
	CLICK,

	SEPARATOR,
} TYPE;

typedef struct token
{
	TYPE type;

	STRING text;
} TOKEN;

/**
 * @brief
 *
 * @param token
 */
void initToken(TOKEN *token);

/**
 * @brief
 *
 * @param token
 */
void textToType(TOKEN *token);

/**
 * @brief
 *
 * @param token
 */
void printToken(TOKEN *token);

#endif

#include "file.h"

typedef enum type
{
	EMPTY,

	TEXT,

	PRESS,
	SEQUENCE,
	COMMAND,
	FIND,
	MOVE,
	MOUSE,
	CLICK,
} TYPE;

#define STARTING_SIZE 10

typedef struct token
{
	TYPE type;

	int length;
	int capacity;
	char *text;
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
 * @param reader
 * @param token
 * @return int
 */
int toToken(READER *reader, TOKEN *token);

/**
 * @brief
 *
 * @param token
 */
void textToType(TOKEN *token);

int main()
{
	READER r = {};
	openFile(&r, "commands.txt");

	TOKEN t = {};
	initToken(&t);
	while (!toToken(&r, &t))
	{
		printf("%s\n", t.text);
		t.length = 0;
		t.type = EMPTY;
	}
}

void initToken(TOKEN *token)
{
	token->type = EMPTY;

	token->length = 0;
	token->capacity = STARTING_SIZE;
	token->text = malloc(STARTING_SIZE * sizeof(*token->text));
}

int toToken(READER *reader, TOKEN *token)
{
	char c;
	while (1)
	{
		c = getChar(reader);
		if (c == EOF)
		{
			return token->type == EMPTY;
		}

		if (c == ' ' || c == '\n')
		{
			if (token->type == EMPTY)
			{
				continue;
			}
			return 0;
		}

		token->text[token->length++] = c;
		token->type = TEXT;
	}
	return 1;
}

void textToType(TOKEN *token)
{
	token->type = TEXT;
}

#include "file.h"

typedef enum type
{
	EMPTY,

	TEXT,

	PRESS,
	SEQUENCE,
	// COMMAND,
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

typedef struct command
{
	int length;
	int capacity;
	TOKEN *tokens;
} COMMAND;

/**
 * @brief
 *
 * @param command
 */
void enlargeCommands(COMMAND *command);

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

/**
 * @brief
 *
 * @param token
 */
void enlargeToken(TOKEN *token);

int main()
{
	READER r = {};
	openFile(&r, "commands.txt");

	COMMAND c = {};
	c.length = 0;
	c.capacity = STARTING_SIZE;
	c.tokens = malloc(STARTING_SIZE * sizeof(*c.tokens));

	while (1)
	{
		initToken(&(c.tokens[c.length]));
		if (toToken(&r, &(c.tokens[c.length])))
		{
			break;
		}

		if (++c.length == c.capacity)
		{
			enlargeCommands(&c);
		}
	}

	for (int i = 0; i < c.length; i++)
	{
		printf("%s\n", c.tokens[i].text);
	}
}

void enlargeCommands(COMMAND *command)
{
	TOKEN *tokens = malloc(command->capacity * 2 * sizeof(*tokens));
	for (unsigned int i = 0; i < command->length; i++)
	{
		tokens[i] = command->tokens[i];
	}
	command->capacity *= 2;

	command->tokens = tokens;
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

		if (token->capacity == token->length)
		{
			enlargeToken(token);
		}

		token->type = TEXT;
	}
	return 1;
}

void enlargeToken(TOKEN *token)
{
	char *text = malloc(token->capacity * 2 * sizeof(*text));
	for (unsigned int i = 0; i < token->length; i++)
	{
		text[i] = token->text[i];
	}
	token->capacity *= 2;

	token->text = text;
}

void textToType(TOKEN *token)
{
	token->type = TEXT;
}

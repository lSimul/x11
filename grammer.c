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

#define STARTING_SIZE 2

typedef struct string
{
	int length;
	int capacity;
	char *text;
} STRING;

typedef struct token
{
	TYPE type;

	STRING text;
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
		printf("%s\n", c.tokens[i].text.text);
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

	STRING s = {};
	s.length = 0;
	s.capacity = STARTING_SIZE;
	s.text = malloc(STARTING_SIZE * sizeof(*s.text));
	token->text = s;
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

		appendToString(&token->text, c);
		token->type = TEXT;
	}
	return 1;
}

void textToType(TOKEN *token)
{
	token->type = TEXT;
}

void appendToString(STRING *s, char c)
{
	s->text[s->length++] = c;

	if (s->capacity == s->length)
	{
		enlargeString(s);
	}
}

void enlargeString(STRING *string)
{
	char *text = malloc(string->capacity * 2 * sizeof(*text));
	for (unsigned int i = 0; i < string->length; i++)
	{
		text[i] = string->text[i];
	}
	string->capacity *= 2;

	string->text = text;
}

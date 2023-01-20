#include "command.h"
#include "token.h"
#include "file.h"

#ifndef _GRAMMER_H_
#define _GRAMMER_H_

/**
 * @brief
 *
 * Returned failures are super lazy. Just an issue while
 * opening the file can lead to the error code.
 *
 * Failure means command is not filled.
 *
 * @param file
 * @param command
 * @return int 1 if failure, 0 otherwise
 */
int fileToCommands(const char *file, COMMAND **command);

/**
 * @brief
 *
 * @param reader
 * @param token
 * @return int
 */
int toToken(READER *reader, TOKEN *token);

/**
 * @brief Check, if the command is really meaningful.
 *
 * Grammer is really simple. Goal is just to check the ammount
 * of commands, sequence of types. No priority, associations,
 * this is really just a simple list to-do things as a text.
 *
 * @param c
 * @return int 1 if there is a mistake, 0 otherwise
 */
int grammerCheck(COMMAND *c);

#endif

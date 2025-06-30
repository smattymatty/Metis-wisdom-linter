/*
 * include/commands.h
 * METIS WISDOM LINTER - Command Handlers Header
 *
 * "Each command is a prayer to the divine consciousness" - Metis Fragment #33
 */

#ifndef COMMANDS_H
#define COMMANDS_H

#include "cli_utils.h"

// Command execution functions
int metis_cmd_lint(const MetisArgs_t* args);
int metis_cmd_config(const MetisArgs_t* args);
int metis_cmd_wisdom(const MetisArgs_t* args);
int metis_cmd_story(const MetisArgs_t* args);
int metis_cmd_help(const MetisArgs_t* args);
int metis_cmd_version(const MetisArgs_t* args);

// Command dispatcher
int metis_cmd_execute(const MetisArgs_t* args);

// Command validation
bool metis_cmd_is_valid(const char* command);
const char* metis_cmd_get_description(const char* command);

#endif // COMMANDS_H

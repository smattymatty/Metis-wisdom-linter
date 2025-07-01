/*
 * include/cli_utils.h
 * METIS WISDOM LINTER - CLI Utilities Header
 *
 */

#ifndef CLI_UTILS_H
#define CLI_UTILS_H

#include <stdbool.h>
#include <stdio.h>

#define METIS_VERSION "0.0.4"
#define METIS_BUILD_DATE __DATE__
#define METIS_BUILD_TIME __TIME__

typedef struct {
    char* command;              // lint, config, wisdom, help, version, story
    char* target_path;          // File or directory to analyze
    bool recursive;             // Recursive directory analysis
    bool show_fragments;        // Display available fragment types
    bool quiet_mode;           // Suppress wisdom fragments
    bool verbose;              // Extra detailed output
    bool show_stats;           // Show consciousness statistics
    bool enable_colors;        // Enable divine color output
    bool compassion_mode;      // Extra compassionate error messages
    bool story_mode;           // Show story fragments
    char* config_file;         // Custom configuration file path
    char* output_format;       // Output format (text, json, divine)
    char* fragment_filter;     // Filter specific fragment types
    int wisdom_level_filter;   // Minimum wisdom level to display
} MetisArgs_t;

// CLI utility functions
MetisArgs_t* metis_cli_create_default_args(void);
MetisArgs_t* metis_cli_parse_arguments(int argc, char* argv[]);
void metis_cli_free_arguments(MetisArgs_t* args);

// Display functions
void metis_cli_display_greeting(bool quiet);
void metis_cli_display_help(void);
void metis_cli_display_version(void);

// Validation functions
bool metis_cli_validate_args(const MetisArgs_t* args);
const char* metis_cli_get_error_string(int error_code);

// Helper functions
bool metis_cli_is_file(const char* path);
bool metis_cli_is_directory(const char* path);
bool metis_cli_path_exists(const char* path);

const char* metis_cli_get_current_working_directory(void);
#endif // CLI_UTILS_H

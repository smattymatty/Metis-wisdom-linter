
// src/main.c - METIS WISDOM LINTER Main Entry Point
// "Where mortal intention meets divine wisdom in sacred communion" - Metis Fragment #1


#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cli_utils.h"
#include "commands.h"
#include "metis_config.h"
#include "metis_colors.h"
#include "fragment_engine.h"

/*
 * All command handling is delegated to the CLI modules for divine organization
 */
int main(int argc, char* argv[]) {
    int result = 0;
    MetisConfig_t* config = NULL;
    MetisArgs_t* args = NULL;

    // Parse arguments using CLI utilities
    args = metis_cli_parse_arguments(argc, argv);
    if (!args) {
        fprintf(stderr, "💀 Failed to parse arguments\n");
        return 1;
    }

    // Validate parsed arguments
    if (!metis_cli_validate_args(args)) {
        printf("%s💀 Divine Error:%s Invalid arguments provided\n", METIS_ERROR, METIS_RESET);
        printf("%sUse %s'metis help'%s for divine guidance\n",
               METIS_TEXT_SECONDARY, METIS_ACCENT, METIS_RESET);
        metis_cli_free_arguments(args);
        return 2;
    }

    // Initialize color system based on arguments
    metis_colors_enable(args->enable_colors);

    // Handle help and version immediately (no initialization needed)
    if (strcmp(args->command, "help") == 0 || strcmp(args->command, "version") == 0) {
        result = metis_cmd_execute(args);
        metis_cli_free_arguments(args);
        return result;
    }

    // Display divine greeting (unless quiet)
    metis_cli_display_greeting(args->quiet_mode);

    // Initialize divine systems for commands that need them
    if (strcmp(args->command, "lint") == 0 ||
        strcmp(args->command, "config") == 0 ||
        strcmp(args->command, "wisdom") == 0 ||
        strcmp(args->command, "story") == 0) {

        // Initialize configuration system
        config = metis_config_init();
        if (!config) {
            printf("%s💀 Divine Error:%s Failed to initialize configuration system\n",
                   METIS_ERROR, METIS_RESET);
            metis_cli_free_arguments(args);
            return 1;
        }

        // Load custom config if specified
        if (args->config_file && !metis_config_load(args->config_file)) {
            printf("%s⚠️ Divine Warning:%s Could not load config file: %s%s%s\n",
                   METIS_WARNING, METIS_RESET,
                   METIS_CLICKABLE_LINK, args->config_file, METIS_RESET);
        }

        // Initialize fragment engine (unless quiet mode or config-only command)
        if (!args->quiet_mode && strcmp(args->command, "config") != 0) {
            if (!metis_fragment_engine_init()) {
                printf("%s💀 Divine Error:%s Failed to awaken fragment delivery system\n",
                       METIS_ERROR, METIS_RESET);
                metis_cli_free_arguments(args);
                metis_config_cleanup();
                return 1;
            }
        }
    }

    // Execute the requested command using the command dispatcher
    result = metis_cmd_execute(args);

    // Divine cleanup
    if (config) {
        if (!args->quiet_mode && strcmp(args->command, "config") != 0) {
            metis_fragment_engine_cleanup();
        }
        metis_config_cleanup();
    }

    // Final blessing
    if (result == 0 && !args->quiet_mode &&
        strcmp(args->command, "help") != 0 &&
        strcmp(args->command, "version") != 0) {
        printf("\n%s✨ Divine Blessing:%s May your code bring wisdom to the world.\n",
               METIS_SUCCESS, METIS_RESET);
        printf("%s💝 \"Through compassionate code, we heal the digital realm\" - Metis Fragment #50%s\n\n",
               METIS_WISDOM_TEXT, METIS_RESET);
    }

    metis_cli_free_arguments(args);
    return result;
}

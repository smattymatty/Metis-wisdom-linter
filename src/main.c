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

// Forward declarations for helper functions
static bool initialize_divine_systems(const MetisArgs_t* args, MetisConfig_t** config);
static void cleanup_divine_systems(const MetisArgs_t* args, bool systems_initialized);

/*
 * Main entry point, orchestrating the divine analysis cycle.
 */
int main(int argc, char* argv[]) {
    MetisArgs_t* args = metis_cli_parse_arguments(argc, argv);
    if (!args || !metis_cli_validate_args(args)) {
        fprintf(stderr, "err: invalid arguments. Use 'metis help' for guidance.\n");
        metis_cli_free_arguments(args);
        return 2;
    }

    metis_colors_enable(args->enable_colors);

    // Handle commands that don't require initialization
    if (!metis_cmd_requires_init(args->command)) {
        int result = metis_cmd_execute(args);
        metis_cli_free_arguments(args);
        return result;
    }

    // --- Main Execution Cycle for Commands Requiring Initialization ---
    metis_cli_display_greeting(args->quiet_mode);

    MetisConfig_t* config = NULL;
    bool systems_initialized = initialize_divine_systems(args, &config);
    int result = 1; // Default to error if initialization fails

    if (systems_initialized) {
        result = metis_cmd_execute(args);
    }

    cleanup_divine_systems(args, systems_initialized);
    metis_cli_free_arguments(args);

    if (result == 0 && !args->quiet_mode) {
        printf("%sCycle complete.%s\n", METIS_TEXT_MUTED, METIS_RESET);
    }

    return result;
}

/**
 * Initializes the configuration and fragment engine systems.
 * @param args Parsed command line arguments.
 * @param config Pointer to the config structure pointer to be initialized.
 * @return true if initialization was successful, false otherwise.
 */
static bool initialize_divine_systems(const MetisArgs_t* args, MetisConfig_t** config) {
    *config = metis_config_init();
    if (!*config) {
        fprintf(stderr, "err: config system failed to initialize.\n");
        return false;
    }

    if (args->config_file && !metis_config_load(args->config_file)) {
        fprintf(stderr, "warn: could not load specified config: %s\n", args->config_file);
    }

    if (!args->quiet_mode) {
        if (!metis_fragment_engine_init()) {
            fprintf(stderr, "err: fragment engine failed to initialize.\n");
            return false;
        }
    }
    return true;
}

/**
 * Cleans up the core systems that were initialized.
 * @param args Parsed command line arguments.
 * @param systems_initialized Flag indicating if initialization occurred.
 */
static void cleanup_divine_systems(const MetisArgs_t* args, bool systems_initialized) {
    if (!systems_initialized) {
        return;
    }

    if (!args->quiet_mode) {
        metis_fragment_engine_cleanup();
    }
    metis_config_cleanup();
}
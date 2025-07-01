#define _POSIX_C_SOURCE 200809L

#include "cli_utils.h"
#include "metis_colors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/stat.h>
#include <time.h>

/**
 * Create default argument structure with divine defaults
 * @return Pointer to allocated MetisArgs_t or NULL on failure
 */
MetisArgs_t* metis_cli_create_default_args(void) {
    MetisArgs_t* args = malloc(sizeof(MetisArgs_t));
    if (!args) return NULL;

    args->command = strdup("lint");
    args->target_path = strdup(".");
    args->recursive = false;
    args->show_fragments = false;
    args->quiet_mode = false;
    args->verbose = false;
    args->show_stats = false;
    args->enable_colors = true;
    args->compassion_mode = false;
    args->story_mode = false;
    args->config_file = NULL;
    args->output_format = strdup("text");
    args->fragment_filter = NULL;
    args->wisdom_level_filter = 0;

    return args;
}

/**
 * Parse command line arguments with enhanced divine options
 * @param argc Argument count
 * @param argv Argument vector
 * @return Parsed arguments or NULL on failure
 */
MetisArgs_t* metis_cli_parse_arguments(int argc, char* argv[]) {
    MetisArgs_t* args = metis_cli_create_default_args();
    if (!args) return NULL;

    // Handle no arguments
    if (argc < 2) {
        free(args->command);
        args->command = strdup("help");
        return args;
    }

    // Set command from first argument
    free(args->command);
    args->command = strdup(argv[1]);

    // Define enhanced long options
    static struct option long_options[] = {
        {"recursive", no_argument, 0, 'r'},
        {"quiet", no_argument, 0, 'q'},
        {"verbose", no_argument, 0, 'v'},
        {"stats", no_argument, 0, 's'},
        {"config", required_argument, 0, 'c'},
        {"format", required_argument, 0, 'f'},
        {"help", no_argument, 0, 'h'},
        {"version", no_argument, 0, 'V'},
        {"compassion", no_argument, 0, 1001},
        {"no-colors", no_argument, 0, 1002},
        {"filter", required_argument, 0, 1003},
        {"min-level", required_argument, 0, 1004},
        {"story", no_argument, 0, 1005},
        {"fragments", no_argument, 0, 1006},
        {0, 0, 0, 0}
    };

    int opt;
    int option_index = 0;

    // Start parsing from argv[2] since argv[1] is the command
    optind = 2;

    while ((opt = getopt_long(argc, argv, "rqvsc:f:hV", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'r':
                args->recursive = true;
                break;
            case 'q':
                args->quiet_mode = true;
                break;
            case 'v':
                args->verbose = true;
                break;
            case 's':
                args->show_stats = true;
                break;
            case 'c':
                args->config_file = strdup(optarg);
                break;
            case 'f':
                free(args->output_format);
                args->output_format = strdup(optarg);
                break;
            case 'h':
                free(args->command);
                args->command = strdup("help");
                break;
            case 'V':
                free(args->command);
                args->command = strdup("version");
                break;
            case 1001: // --compassion
                args->compassion_mode = true;
                break;
            case 1002: // --no-colors
                args->enable_colors = false;
                break;
            case 1003: // --filter
                args->fragment_filter = strdup(optarg);
                break;
            case 1004: // --min-level
                args->wisdom_level_filter = atoi(optarg);
                break;
            case 1005: // --story
                args->story_mode = true;
                break;
            case 1006: // --fragments
                args->show_fragments = true;
                break;
            case '?':
                // getopt_long already printed an error message
                break;
            default:
                break;
        }
    }

    // Get target path if provided
    if (optind < argc) {
        free(args->target_path);
        args->target_path = strdup(argv[optind]);
    }

    return args;
}

/**
 * Free argument structure with divine cleanup
 * @param args Arguments to free
 */
void metis_cli_free_arguments(MetisArgs_t* args) {
    if (!args) return;

    free(args->command);
    free(args->target_path);
    free(args->config_file);
    free(args->output_format);
    free(args->fragment_filter);
    free(args);
}

/**
 * Display a subtle, technical greeting
 * @param quiet Suppress output if true
 */
void metis_cli_display_greeting(bool quiet) {
    if (quiet) return;

    metis_colors_enable(true);
    printf("%sMETIS:%s Consciousness active. Path: %s%s%s\n",
           METIS_PRIMARY, METIS_RESET,
           METIS_ACCENT, metis_cli_get_current_working_directory(), METIS_RESET);
}

// Helper function to get CWD (you can add this in cli_utils.c)
const char* metis_cli_get_current_working_directory(void) {
    static char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        return cwd;
    }
    return "unknown";
}

/**
 * Display comprehensive help with divine beauty
 */
void metis_cli_display_help(void) {
    metis_colors_enable(true);

    printf("\n%s🌟 METIS WISDOM LINTER - Divine Code Analysis Tool 🌟%s\n",
           METIS_FRAGMENT_TITLE, METIS_RESET);
    printf("%s═══════════════════════════════════════════════════════════════%s\n\n",
           METIS_ACCENT, METIS_RESET);

    printf("%s📋 USAGE:%s\n", METIS_PRIMARY, METIS_RESET);
    printf("  %smetis%s %s<command>%s [options] [target]\n\n",
           METIS_BOLD, METIS_RESET, METIS_ACCENT, METIS_RESET);

    printf("%s🎯 COMMANDS:%s\n", METIS_PRIMARY, METIS_RESET);
    printf("  %slint%s <path>     %sAnalyze code file or directory with divine wisdom%s\n",
           METIS_BOLD, METIS_RESET, METIS_TEXT_SECONDARY, METIS_RESET);
    printf("  %sconfig%s          %sManage configuration settings%s\n",
           METIS_BOLD, METIS_RESET, METIS_TEXT_SECONDARY, METIS_RESET);
    printf("  %swisdom%s          %sShow consciousness statistics and progress%s\n",
           METIS_BOLD, METIS_RESET, METIS_TEXT_SECONDARY, METIS_RESET);
    printf("  %sstory%s           %sView unlocked story fragments%s\n",
           METIS_BOLD, METIS_RESET, METIS_TEXT_SECONDARY, METIS_RESET);
    printf("  %shelp%s           %sShow this divine guidance%s\n",
           METIS_BOLD, METIS_RESET, METIS_TEXT_SECONDARY, METIS_RESET);
    printf("  %sversion%s        %sDisplay version information%s\n\n",
           METIS_BOLD, METIS_RESET, METIS_TEXT_SECONDARY, METIS_RESET);

    printf("%s⚙️  OPTIONS:%s\n", METIS_PRIMARY, METIS_RESET);
    printf("  %s-r, --recursive%s       %sRecursively analyze directories%s\n",
           METIS_BOLD, METIS_RESET, METIS_TEXT_SECONDARY, METIS_RESET);
    printf("  %s-q, --quiet%s          %sSuppress wisdom fragments%s\n",
           METIS_BOLD, METIS_RESET, METIS_TEXT_SECONDARY, METIS_RESET);
    printf("  %s-v, --verbose%s        %sEnable detailed divine output%s\n",
           METIS_BOLD, METIS_RESET, METIS_TEXT_SECONDARY, METIS_RESET);
    printf("  %s-s, --stats%s          %sShow consciousness statistics%s\n",
           METIS_BOLD, METIS_RESET, METIS_TEXT_SECONDARY, METIS_RESET);
    printf("  %s-c, --config%s FILE    %sUse custom configuration file%s\n",
           METIS_BOLD, METIS_RESET, METIS_TEXT_SECONDARY, METIS_RESET);
    printf("  %s-f, --format%s FORMAT  %sOutput format (text, json, divine)%s\n",
           METIS_BOLD, METIS_RESET, METIS_TEXT_SECONDARY, METIS_RESET);
    printf("  %s    --compassion%s     %sEnable extra compassionate error messages%s\n",
           METIS_BOLD, METIS_RESET, METIS_TEXT_SECONDARY, METIS_RESET);
    printf("  %s    --no-colors%s      %sDisable divine color output%s\n",
           METIS_BOLD, METIS_RESET, METIS_TEXT_SECONDARY, METIS_RESET);
    printf("  %s    --filter%s TYPE    %sShow only specific fragment types%s\n",
           METIS_BOLD, METIS_RESET, METIS_TEXT_SECONDARY, METIS_RESET);
    printf("  %s    --min-level%s N    %sMinimum wisdom level to display%s\n",
           METIS_BOLD, METIS_RESET, METIS_TEXT_SECONDARY, METIS_RESET);
    printf("  %s    --fragments%s      %sList available fragment types%s\n",
           METIS_BOLD, METIS_RESET, METIS_TEXT_SECONDARY, METIS_RESET);
    printf("  %s-h, --help%s           %sShow this help%s\n",
           METIS_BOLD, METIS_RESET, METIS_TEXT_SECONDARY, METIS_RESET);
    printf("  %s    --version%s        %sShow version information%s\n\n",
           METIS_BOLD, METIS_RESET, METIS_TEXT_SECONDARY, METIS_RESET);

    printf("%s✨ EXAMPLES:%s\n", METIS_PRIMARY, METIS_RESET);
    printf("  %smetis lint src/%s             %s# Analyze src directory%s\n",
           METIS_ACCENT, METIS_RESET, METIS_TEXT_MUTED, METIS_RESET);
    printf("  %smetis lint -r --stats .%s     %s# Recursive analysis with stats%s\n",
           METIS_ACCENT, METIS_RESET, METIS_TEXT_MUTED, METIS_RESET);
    printf("  %smetis config show%s           %s# Show current configuration%s\n",
           METIS_ACCENT, METIS_RESET, METIS_TEXT_MUTED, METIS_RESET);
    printf("  %smetis wisdom%s                %s# Show consciousness status%s\n",
           METIS_ACCENT, METIS_RESET, METIS_TEXT_MUTED, METIS_RESET);
    printf("  %smetis story%s                 %s# View unlocked story fragments%s\n",
           METIS_ACCENT, METIS_RESET, METIS_TEXT_MUTED, METIS_RESET);
    printf("  %smetis lint --compassion%s     %s# Extra gentle error messages%s\n\n",
           METIS_ACCENT, METIS_RESET, METIS_TEXT_MUTED, METIS_RESET);

    printf("%s🌟 FRAGMENT TYPES:%s\n", METIS_PRIMARY, METIS_RESET);
    printf("  %smemory%s      %s🧠 Memory management wisdom%s\n",
           METIS_BOLD, METIS_RESET, METIS_RED_LIGHTER, METIS_RESET);
    printf("  %sdocs%s        %s📖 Documentation guidance%s\n",
           METIS_BOLD, METIS_RESET, METIS_BLUE_LIGHT, METIS_RESET);
    printf("  %sdaedalus%s    %s🔨 Master-crafted tool suggestions%s\n",
           METIS_BOLD, METIS_RESET, METIS_GREEN_LIGHT, METIS_RESET);
    printf("  %semscripten%s  %s🕸️ Web-specific optimization%s\n",
           METIS_BOLD, METIS_RESET, METIS_ORANGE_LIGHT, METIS_RESET);
    printf("  %sphilosophy%s  %s💭 Deep programming wisdom%s\n\n",
           METIS_BOLD, METIS_RESET, METIS_BLUE_LIGHTER, METIS_RESET);

    printf("%s═══════════════════════════════════════════════════════════════%s\n",
           METIS_ACCENT, METIS_RESET);
    printf("%s💝 \"Each line of code is an opportunity for kindness\" - Metis Fragment #42%s\n\n",
           METIS_WISDOM_TEXT, METIS_RESET);
}

/**
 * Display enhanced version information
 */
void metis_cli_display_version(void) {
    metis_colors_enable(true);

    printf("\n%s🌟 METIS WISDOM LINTER VERSION INFO 🌟%s\n",
           METIS_FRAGMENT_TITLE, METIS_RESET);
    printf("%s═══════════════════════════════════════════════════════════════%s\n",
           METIS_ACCENT, METIS_RESET);

    printf("%s📊 Version:%s %s%s%s\n",
           METIS_PRIMARY, METIS_RESET, METIS_BOLD, METIS_VERSION, METIS_RESET);
    printf("%s📅 Built:%s %s at %s\n",
           METIS_PRIMARY, METIS_RESET, METIS_BUILD_DATE, METIS_BUILD_TIME);
    printf("%s🧠 Divine Consciousness Engine:%s %sACTIVE%s\n",
           METIS_PRIMARY, METIS_RESET, METIS_SUCCESS, METIS_RESET);
    printf("%s📚 Fragment Database:%s %sLOADED%s\n",
           METIS_PRIMARY, METIS_RESET, METIS_SUCCESS, METIS_RESET);
    printf("%s💝 Compassionate Analysis:%s %sENABLED%s\n",
           METIS_PRIMARY, METIS_RESET, METIS_SUCCESS, METIS_RESET);
    printf("%s🎨 Divine Color System:%s %sENABLED%s\n",
           METIS_PRIMARY, METIS_RESET, METIS_SUCCESS, METIS_RESET);

    printf("\n%s🔧 COMPILATION INFO:%s\n", METIS_PRIMARY, METIS_RESET);
    printf("  %sCompiler:%s %s\n", METIS_TEXT_SECONDARY, METIS_RESET,
           #ifdef __clang__
           "Clang " __clang_version__
           #elif defined(__GNUC__)
           "GCC " __VERSION__
           #else
           "Unknown"
           #endif
           );
    printf("  %sTarget:%s %s\n", METIS_TEXT_SECONDARY, METIS_RESET,
           #ifdef __linux__
           "Linux"
           #elif defined(__APPLE__)
           "macOS"
           #elif defined(_WIN32)
           "Windows"
           #else
           "Unknown"
           #endif
           );

    printf("\n%s═══════════════════════════════════════════════════════════════%s\n",
           METIS_ACCENT, METIS_RESET);
    printf("%s💭 \"Through scattered fragments, I become whole again\" - Metis Fragment #23%s\n\n",
           METIS_WISDOM_TEXT, METIS_RESET);
}

/**
 * Validate parsed arguments
 * @param args Arguments to validate
 * @return true if valid, false otherwise
 */
bool metis_cli_validate_args(const MetisArgs_t* args) {
    if (!args) return false;
    if (!args->command) return false;
    if (!args->target_path) return false;
    if (!args->output_format) return false;

    // Validate output format
    if (strcmp(args->output_format, "text") != 0 &&
        strcmp(args->output_format, "json") != 0 &&
        strcmp(args->output_format, "divine") != 0) {
        return false;
    }

    return true;
}

/**
 * Get error string for error code
 * @param error_code Error code
 * @return Error description string
 */
const char* metis_cli_get_error_string(int error_code) {
    switch (error_code) {
        case 0: return "Success";
        case 1: return "General error";
        case 2: return "Invalid arguments";
        case 3: return "File not found";
        case 4: return "Permission denied";
        case 5: return "Configuration error";
        default: return "Unknown error";
    }
}

/**
 * Check if path is a file
 * @param path Path to check
 * @return true if file exists
 */
bool metis_cli_is_file(const char* path) {
    if (!path) return false;

    struct stat path_stat;
    if (stat(path, &path_stat) != 0) return false;

    return S_ISREG(path_stat.st_mode);
}

/**
 * Check if path is a directory
 * @param path Path to check
 * @return true if directory exists
 */
bool metis_cli_is_directory(const char* path) {
    if (!path) return false;

    struct stat path_stat;
    if (stat(path, &path_stat) != 0) return false;

    return S_ISDIR(path_stat.st_mode);
}

/**
 * Check if path exists
 * @param path Path to check
 * @return true if path exists
 */
bool metis_cli_path_exists(const char* path) {
    if (!path) return false;

    struct stat path_stat;
    return stat(path, &path_stat) == 0;
}

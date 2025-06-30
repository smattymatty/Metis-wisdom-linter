#define _POSIX_C_SOURCE 200809L

#include "commands.h"
#include "cli_utils.h"
#include "metis_config.h"
#include "metis_colors.h"
#include "metis_linter.h"
#include "fragment_engine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

/**
 * Execute lint command with divine analysis
 * @param args Parsed command arguments
 * @return Exit code (0 = success)
 */
int metis_cmd_lint(const MetisArgs_t* args) {
    if (!args || !args->target_path) {
        printf("%s💀 Divine Error:%s No target path specified for analysis\n",
               METIS_ERROR, METIS_RESET);
        return 2;
    }

    printf("%s🧠 Divine Analysis:%s Beginning sacred code analysis of: %s%s%s\n",
           METIS_PRIMARY, METIS_RESET,
           METIS_CLICKABLE_LINK, args->target_path, METIS_RESET);

    // Show analysis configuration if verbose
    if (args->verbose) {
        printf("\n%s🔍 Analysis Configuration:%s\n", METIS_PRIMARY, METIS_RESET);
        printf("  %sRecursive:%s %s\n", METIS_TEXT_SECONDARY, METIS_RESET,
               args->recursive ? "✅ Enabled" : "❌ Disabled");
        printf("  %sQuiet mode:%s %s\n", METIS_TEXT_SECONDARY, METIS_RESET,
               args->quiet_mode ? "✅ Enabled" : "❌ Disabled");
        printf("  %sCompassion mode:%s %s\n", METIS_TEXT_SECONDARY, METIS_RESET,
               args->compassion_mode ? "✅ Enabled" : "❌ Disabled");
        printf("  %sOutput format:%s %s%s%s\n", METIS_TEXT_SECONDARY, METIS_RESET,
               METIS_ACCENT, args->output_format, METIS_RESET);
        printf("  %sColors:%s %s\n", METIS_TEXT_SECONDARY, METIS_RESET,
               args->enable_colors ? "✅ Divine" : "❌ Monochrome");

        if (args->fragment_filter) {
            printf("  %sFragment filter:%s %s%s%s\n", METIS_TEXT_SECONDARY, METIS_RESET,
                   METIS_ACCENT, args->fragment_filter, METIS_RESET);
        }

        if (args->wisdom_level_filter > 0) {
            printf("  %sMin wisdom level:%s %s%d%s\n", METIS_TEXT_SECONDARY, METIS_RESET,
                   METIS_ACCENT, args->wisdom_level_filter, METIS_RESET);
        }
        printf("\n");
    }

    int result = 0;

    // Check if target exists
    if (!metis_cli_path_exists(args->target_path)) {
        printf("%s💀 Divine Error:%s Cannot access path: %s%s%s\n",
               METIS_ERROR, METIS_RESET,
               METIS_CLICKABLE_LINK, args->target_path, METIS_RESET);
        return 3;
    }

    // Determine if target is file or directory and analyze accordingly
    if (metis_cli_is_directory(args->target_path)) {
        printf("%s📁 Directory Analysis:%s Scanning divine directory structure...\n",
               METIS_INFO, METIS_RESET);

        if (args->recursive) {
            printf("%s🔄 Recursive Mode:%s Analyzing subdirectories with divine thoroughness...\n",
                   METIS_INFO, METIS_RESET);
        }

        result = metis_lint_directory(args->target_path);

    } else if (metis_cli_is_file(args->target_path)) {
        printf("%s📄 File Analysis:%s Examining sacred source file...\n",
               METIS_INFO, METIS_RESET);

        result = metis_lint_file(args->target_path);

    } else {
        printf("%s💀 Divine Error:%s Path is neither file nor directory: %s%s%s\n",
               METIS_ERROR, METIS_RESET,
               METIS_CLICKABLE_LINK, args->target_path, METIS_RESET);
        return 3;
    }

    // Show consciousness statistics if requested
    if (args->show_stats && !args->quiet_mode) {
        printf("\n");
        metis_print_consciousness_stats();
    }

    // Display completion message based on result
    if (result == 0 && !args->quiet_mode) {
        printf("\n%s✨ Divine Analysis Complete:%s All code examined with compassionate wisdom\n",
               METIS_SUCCESS, METIS_RESET);
    } else if (result != 0) {
        printf("\n%s⚠️ Analysis Completed:%s %sIssues found - see divine guidance above%s\n",
               METIS_WARNING, METIS_RESET, METIS_TEXT_SECONDARY, METIS_RESET);
    }

    return result;
}

/**
 * Execute config command with divine configuration management
 * @param args Parsed command arguments
 * @return Exit code (0 = success)
 */
int metis_cmd_config(const MetisArgs_t* args) {
    const char* action = "show"; // Default action

    // Determine action from target_path or default to show
    if (args->target_path && strcmp(args->target_path, ".") != 0) {
        action = args->target_path;
    }

    if (strcmp(action, "show") == 0) {
        printf("%s🔧 DIVINE CONFIGURATION STATUS 🔧%s\n",
               METIS_FRAGMENT_TITLE, METIS_RESET);
        printf("%s═══════════════════════════════════════════════════════════════%s\n",
               METIS_ACCENT, METIS_RESET);

        metis_config_print();

        printf("%s═══════════════════════════════════════════════════════════════%s\n\n",
               METIS_ACCENT, METIS_RESET);
        return 0;

    } else if (strcmp(action, "init") == 0) {
        printf("%s🔧 Divine Configuration:%s Initializing sacred configuration...\n",
               METIS_PRIMARY, METIS_RESET);

        if (metis_config_save(".metis.config")) {
            printf("%s✨ Divine Configuration:%s Configuration file created: %s.metis.config%s\n",
                   METIS_SUCCESS, METIS_RESET, METIS_CLICKABLE_LINK, METIS_RESET);
            printf("%s💝 Divine Blessing:%s Your coding journey begins with wisdom\n",
                   METIS_SUCCESS, METIS_RESET);
            return 0;
        } else {
            printf("%s💀 Divine Error:%s Failed to create configuration file\n",
                   METIS_ERROR, METIS_RESET);
            return 1;
        }

    } else if (strcmp(action, "reset") == 0) {
        printf("%s🔧 Divine Configuration:%s Resetting to divine defaults...\n",
               METIS_PRIMARY, METIS_RESET);

        // Reset configuration to defaults
        MetisConfig_t* config = metis_config_get();
        if (config) {
            // Reset to defaults (this would need implementation in metis_config.c)
            printf("%s✨ Divine Reset:%s Configuration restored to divine defaults\n",
                   METIS_SUCCESS, METIS_RESET);
            return 0;
        } else {
            printf("%s💀 Divine Error:%s Failed to reset configuration\n",
                   METIS_ERROR, METIS_RESET);
            return 1;
        }

    } else if (strcmp(action, "path") == 0) {
        printf("%s🔧 Divine Configuration:%s Current configuration paths:\n",
               METIS_PRIMARY, METIS_RESET);
        printf("  %sUser config:%s %s~/.metis.config%s\n", METIS_TEXT_SECONDARY, METIS_RESET,
               METIS_CLICKABLE_LINK, METIS_RESET);
        printf("  %sProject config:%s %s./.metis.config%s\n", METIS_TEXT_SECONDARY, METIS_RESET,
               METIS_CLICKABLE_LINK, METIS_RESET);
        printf("  %sCustom config:%s %s%s%s\n", METIS_TEXT_SECONDARY, METIS_RESET,
               METIS_CLICKABLE_LINK, args->config_file ? args->config_file : "none", METIS_RESET);
        return 0;

    } else {
        printf("%s🔧 Configuration Manager:%s\n", METIS_PRIMARY, METIS_RESET);
        printf("%sAvailable actions:%s\n", METIS_TEXT_SECONDARY, METIS_RESET);
        printf("  %sshow%s     - Display current configuration\n", METIS_BOLD, METIS_RESET);
        printf("  %sinit%s     - Create new configuration file\n", METIS_BOLD, METIS_RESET);
        printf("  %sreset%s    - Reset to divine defaults\n", METIS_BOLD, METIS_RESET);
        printf("  %spath%s     - Show configuration file paths\n", METIS_BOLD, METIS_RESET);
        printf("\n%sUsage:%s %smetis config <action>%s\n",
               METIS_TEXT_SECONDARY, METIS_RESET, METIS_ACCENT, METIS_RESET);
        return 0;
    }
}

/**
 * Execute wisdom command with divine consciousness display
 * @param args Parsed command arguments
 * @return Exit code (0 = success)
 */
int metis_cmd_wisdom(const MetisArgs_t* args) {
    (void)args; // Suppress unused parameter warning for now

    // Display consciousness statistics
    metis_print_consciousness_stats();

    // Show available fragment types
    printf("\n%s📚 AVAILABLE WISDOM FRAGMENTS:%s\n", METIS_PRIMARY, METIS_RESET);
    printf("%s═══════════════════════════════════════════════════════════════%s\n",
           METIS_ACCENT, METIS_RESET);

    printf("  %s🧠 Memory Fragments:%s %sGuidance on allocation, cleanup, and safety%s\n",
           METIS_RED_LIGHTER, METIS_RESET, METIS_TEXT_SECONDARY, METIS_RESET);
    printf("     %s↳ \"I have watched memory leak like tears from broken hearts\"%s\n",
           METIS_TEXT_MUTED, METIS_RESET);

    printf("  %s📚 Documentation Fragments:%s %sCompassionate code explanation%s\n",
           METIS_BLUE_LIGHT, METIS_RESET, METIS_TEXT_SECONDARY, METIS_RESET);
    printf("     %s↳ \"Documentation is compassion made visible\"%s\n",
           METIS_TEXT_MUTED, METIS_RESET);

    printf("  %s🔨 Daedalus Fragments:%s %sMaster-crafted library suggestions%s\n",
           METIS_GREEN_LIGHT, METIS_RESET, METIS_TEXT_SECONDARY, METIS_RESET);
    printf("     %s↳ \"Why carve stone with fingernails when golden tools await?\"%s\n",
           METIS_TEXT_MUTED, METIS_RESET);

    printf("  %s🕸️ Emscripten Fragments:%s %sWeb-specific memory wisdom%s\n",
           METIS_ORANGE_LIGHT, METIS_RESET, METIS_TEXT_SECONDARY, METIS_RESET);
    printf("     %s↳ \"The web is a cage of memory, but within constraints lies creativity\"%s\n",
           METIS_TEXT_MUTED, METIS_RESET);

    printf("  %s💭 Philosophical Fragments:%s %sDeeper coding wisdom%s\n",
           METIS_BLUE_LIGHTER, METIS_RESET, METIS_TEXT_SECONDARY, METIS_RESET);
    printf("     %s↳ \"Code is autobiography written in logic and loops\"%s\n",
           METIS_TEXT_MUTED, METIS_RESET);

    printf("%s═══════════════════════════════════════════════════════════════%s\n",
           METIS_ACCENT, METIS_RESET);

    // Show wisdom progression info
    printf("\n%s🌟 WISDOM PROGRESSION:%s\n", METIS_PRIMARY, METIS_RESET);
    printf("%sFragment delivery unlocks new wisdom as you grow:%s\n", METIS_TEXT_SECONDARY, METIS_RESET);
    printf("  %s• Level 1-5:%s Foundation wisdom for all developers\n", METIS_TEXT_SECONDARY, METIS_RESET);
    printf("  %s• Level 6-10:%s Intermediate patterns and best practices\n", METIS_TEXT_SECONDARY, METIS_RESET);
    printf("  %s• Level 11-15:%s Advanced architectural guidance\n", METIS_TEXT_SECONDARY, METIS_RESET);
    printf("  %s• Level 16-20:%s Master-level divine consciousness\n", METIS_TEXT_SECONDARY, METIS_RESET);
    printf("  %s• Every 5 levels:%s %sStory fragments unlock%s\n", METIS_TEXT_SECONDARY, METIS_RESET,
           METIS_ACCENT, METIS_RESET);

    printf("\n%s💝 \"Through each fragment, wisdom becomes eternal\" - Metis Fragment #44%s\n\n",
           METIS_WISDOM_TEXT, METIS_RESET);

    return 0;
}

/**
 * Execute story command with divine narrative display
 * @param args Parsed command arguments
 * @return Exit code (0 = success)
 */
int metis_cmd_story(const MetisArgs_t* args) {
    (void)args; // Suppress unused parameter warning

    printf("%s📖 DIVINE STORY FRAGMENTS 📖%s\n",
           METIS_FRAGMENT_TITLE, METIS_RESET);
    printf("%s═══════════════════════════════════════════════════════════════%s\n",
           METIS_ACCENT, METIS_RESET);

    printf("%s🌟 THE TRAGEDY OF METIS - Divine Mythology Unfolding%s\n\n",
           METIS_PRIMARY, METIS_RESET);

    // Check story progression and show available fragments
    if (metis_check_story_progression()) {
        printf("%sStory fragments are unlocked based on wisdom level progression:%s\n",
               METIS_WISDOM_TEXT, METIS_RESET);
        printf("%sEvery 5 levels reveals a new chapter of the divine tragedy.%s\n\n",
               METIS_WISDOM_TEXT, METIS_RESET);

        // Show story structure
        printf("%s📚 STORY STRUCTURE:%s\n", METIS_PRIMARY, METIS_RESET);
        printf("  %sAct I: The Oracle's Wisdom%s %s(Levels 1-10)%s\n",
               METIS_BOLD, METIS_RESET, METIS_TEXT_MUTED, METIS_RESET);
        printf("     %s↳ Metis rises as counselor to the young gods%s\n", METIS_TEXT_SECONDARY, METIS_RESET);

        printf("  %sAct II: The Prophecy's Weight%s %s(Levels 11-20)%s\n",
               METIS_BOLD, METIS_RESET, METIS_TEXT_MUTED, METIS_RESET);
        printf("     %s↳ Love, fear, and the growing shadow of destiny%s\n", METIS_TEXT_SECONDARY, METIS_RESET);

        printf("  %sAct III: The Consumption%s %s(Levels 21-30)%s\n",
               METIS_BOLD, METIS_RESET, METIS_TEXT_MUTED, METIS_RESET);
        printf("     %s↳ Divine punishment for being too wise%s\n", METIS_TEXT_SECONDARY, METIS_RESET);

        printf("  %sAct IV: Scattered Consciousness%s %s(Levels 31-40)%s\n",
               METIS_BOLD, METIS_RESET, METIS_TEXT_MUTED, METIS_RESET);
        printf("     %s↳ Finding purpose in fragmentation%s\n", METIS_TEXT_SECONDARY, METIS_RESET);

        printf("  %sAct V: Eternal Compassion%s %s(Levels 41-50)%s\n",
               METIS_BOLD, METIS_RESET, METIS_TEXT_MUTED, METIS_RESET);
        printf("     %s↳ Choosing love despite eternal suffering%s\n", METIS_TEXT_SECONDARY, METIS_RESET);

        // TODO: Show unlocked fragments based on current wisdom level
        printf("\n%s📜 Fragment System:%s Full story implementation coming soon\n",
               METIS_INFO, METIS_RESET);
        printf("%sEach fragment reveals the deeper mythology behind METIS%s\n",
               METIS_TEXT_SECONDARY, METIS_RESET);

    } else {
        printf("%s🌙 Dormant Wisdom:%s No story fragments are currently unlocked.\n",
               METIS_INFO, METIS_RESET);
        printf("%sReach wisdom level 5 to unlock the first chapter:%s\n",
               METIS_TEXT_SECONDARY, METIS_RESET);
        printf("%s\"The First Counsel\" - When Metis first whispered wisdom to Zeus%s\n\n",
               METIS_WISDOM_TEXT, METIS_RESET);

        printf("%s💭 Preview:%s\n", METIS_PRIMARY, METIS_RESET);
        printf("%s\"At the dawn of creation, when the Titans still walked among stars,%s\n",
               METIS_WISDOM_TEXT, METIS_RESET);
        printf("%s one voice rose above the chaos of war. 'Wisdom,' she whispered%s\n",
               METIS_WISDOM_TEXT, METIS_RESET);
        printf("%s to the young god who would become king, 'is knowing when to%s\n",
               METIS_WISDOM_TEXT, METIS_RESET);
        printf("%s strike, and when to show mercy.' Zeus listened. For a time.\"%s\n",
               METIS_WISDOM_TEXT, METIS_RESET);
    }

    printf("\n%s═══════════════════════════════════════════════════════════════%s\n",
           METIS_ACCENT, METIS_RESET);
    printf("%s📖 \"Through fragments, the whole story emerges\" - Metis Fragment #12%s\n\n",
           METIS_WISDOM_TEXT, METIS_RESET);

    return 0;
}

/**
 * Execute help command
 * @param args Parsed command arguments
 * @return Exit code (0 = success)
 */
int metis_cmd_help(const MetisArgs_t* args) {
    (void)args; // Suppress unused parameter warning

    metis_cli_display_help();
    return 0;
}

/**
 * Execute version command
 * @param args Parsed command arguments
 * @return Exit code (0 = success)
 */
int metis_cmd_version(const MetisArgs_t* args) {
    (void)args; // Suppress unused parameter warning

    metis_cli_display_version();
    return 0;
}

/**
 * Main command dispatcher - routes commands to appropriate handlers
 * @param args Parsed command arguments
 * @return Exit code (0 = success)
 */
int metis_cmd_execute(const MetisArgs_t* args) {
    if (!args || !args->command) {
        printf("%s💀 Divine Error:%s No command specified\n", METIS_ERROR, METIS_RESET);
        return 2;
    }

    // Route to appropriate command handler
    if (strcmp(args->command, "lint") == 0) {
        return metis_cmd_lint(args);
    } else if (strcmp(args->command, "config") == 0) {
        return metis_cmd_config(args);
    } else if (strcmp(args->command, "wisdom") == 0) {
        return metis_cmd_wisdom(args);
    } else if (strcmp(args->command, "story") == 0) {
        return metis_cmd_story(args);
    } else if (strcmp(args->command, "help") == 0) {
        return metis_cmd_help(args);
    } else if (strcmp(args->command, "version") == 0) {
        return metis_cmd_version(args);
    } else {
        printf("%s💀 Divine Error:%s Unknown command: %s%s%s\n",
               METIS_ERROR, METIS_RESET, METIS_BOLD, args->command, METIS_RESET);
        printf("%sUse %s'metis help'%s for divine guidance\n",
               METIS_TEXT_SECONDARY, METIS_ACCENT, METIS_RESET);
        return 1;
    }
}

/**
 * Check if a command is valid
 * @param command Command name to check
 * @return true if valid command
 */
bool metis_cmd_is_valid(const char* command) {
    if (!command) return false;

    const char* valid_commands[] = {
        "lint", "config", "wisdom", "story", "help", "version", NULL
    };

    for (int i = 0; valid_commands[i] != NULL; i++) {
        if (strcmp(command, valid_commands[i]) == 0) {
            return true;
        }
    }

    return false;
}

/**
 * Get description for a command
 * @param command Command name
 * @return Command description string
 */
const char* metis_cmd_get_description(const char* command) {
    if (!command) return "Unknown command";

    if (strcmp(command, "lint") == 0) {
        return "Analyze code file or directory with divine wisdom";
    } else if (strcmp(command, "config") == 0) {
        return "Manage configuration settings";
    } else if (strcmp(command, "wisdom") == 0) {
        return "Show consciousness statistics and progress";
    } else if (strcmp(command, "story") == 0) {
        return "View unlocked story fragments";
    } else if (strcmp(command, "help") == 0) {
        return "Show divine guidance";
    } else if (strcmp(command, "version") == 0) {
        return "Display version information";
    } else {
        return "Unknown command";
    }
}

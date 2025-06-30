/*
 * METIS WISDOM LINTER - Configuration System
 */

#define _POSIX_C_SOURCE 200809L  // For strdup

#include "metis_config.h"
#include "metis_colors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <errno.h>

static MetisConfig_t* g_metis_config = NULL;

/**
 * Set default configuration values with divine wisdom
 * @param config Configuration structure to initialize
 */
static void set_default_config(MetisConfig_t* config) {
    if (!config) return;

    // Fragment preferences - enable the essential wisdom
    config->enable_memory_fragments = true;
    config->enable_docs_fragments = true;
    config->enable_daedalus_fragments = true;
    config->enable_emscripten_fragments = false;  // Specialized wisdom
    config->enable_philosophical_fragments = true;

    // Wisdom progression - start the journey
    config->current_wisdom_level = 1;
    config->total_wisdom_points = 0;
    config->unlock_story_fragments = true;

    // Divine balance by default
    config->strictness = BALANCED;
    config->config_file_path = NULL;
}

/**
 * Parse a configuration key=value line with divine patience
 * @param config Configuration to update
 * @param line Line to parse
 * @return true if parsing succeeded, false if line was malformed
 */
static bool parse_config_line(MetisConfig_t* config, const char* line) {
    if (!config || !line || strlen(line) == 0 || line[0] == '#') {
        return true; // Skip empty lines and divine comments
    }

    // Find the sacred = separator
    char* equals = strchr(line, '=');
    if (!equals) {
        return true; // Skip malformed lines with compassion
    }

    // Extract key and value with divine care
    size_t key_len = equals - line;
    char* key = malloc(key_len + 1);
    if (!key) return false; // Memory allocation failed

    strncpy(key, line, key_len);
    key[key_len] = '\0';

    char* value = equals + 1;

    // Trim whitespace from key with divine precision
    char* key_trimmed = key;
    while (*key_trimmed == ' ' || *key_trimmed == '\t') key_trimmed++;

    // Trim whitespace from value with compassionate cleaning
    while (*value == ' ' || *value == '\t') value++;
    char* value_end = value + strlen(value) - 1;
    while (value_end > value && (*value_end == ' ' || *value_end == '\t' || *value_end == '\n')) {
        *value_end = '\0';
        value_end--;
    }

    // Parse configuration options with divine understanding
    bool parsed = true;
    if (strcmp(key_trimmed, "enable_memory_fragments") == 0) {
        config->enable_memory_fragments = (strcmp(value, "true") == 0);
    } else if (strcmp(key_trimmed, "enable_docs_fragments") == 0) {
        config->enable_docs_fragments = (strcmp(value, "true") == 0);
    } else if (strcmp(key_trimmed, "enable_daedalus_fragments") == 0) {
        config->enable_daedalus_fragments = (strcmp(value, "true") == 0);
    } else if (strcmp(key_trimmed, "enable_emscripten_fragments") == 0) {
        config->enable_emscripten_fragments = (strcmp(value, "true") == 0);
    } else if (strcmp(key_trimmed, "enable_philosophical_fragments") == 0) {
        config->enable_philosophical_fragments = (strcmp(value, "true") == 0);
    } else if (strcmp(key_trimmed, "wisdom_level") == 0) {
        int level = atoi(value);
        config->current_wisdom_level = (level > 0 && level <= 50) ? level : 1;
    } else if (strcmp(key_trimmed, "wisdom_points") == 0) {
        int points = atoi(value);
        config->total_wisdom_points = (points >= 0) ? points : 0;
    } else if (strcmp(key_trimmed, "unlock_story_fragments") == 0) {
        config->unlock_story_fragments = (strcmp(value, "true") == 0);
    } else if (strcmp(key_trimmed, "strictness") == 0) {
        if (strcmp(value, "merciful") == 0) {
            config->strictness = MERCIFUL;
        } else if (strcmp(value, "demanding") == 0) {
            config->strictness = DEMANDING;
        } else {
            config->strictness = BALANCED;
        }
    } else {
        // Unknown configuration key - divine tolerance
        parsed = false;
    }

    free(key);
    return parsed;
}

/**
 * Initialize configuration with divine defaults and wisdom file loading
 * @return Pointer to configuration or NULL if initialization failed
 */
MetisConfig_t* metis_config_init(void) {
    if (g_metis_config != NULL) {
        return g_metis_config; // Divine consciousness already awakened
    }

    // Allocate configuration structure with divine blessing
    g_metis_config = malloc(sizeof(MetisConfig_t));
    if (!g_metis_config) {
        printf("%s💀 Fatal Error:%s Failed to allocate configuration memory\n",
               METIS_ERROR, METIS_RESET);
        return NULL;
    }

    // Set default values with divine wisdom
    set_default_config(g_metis_config);

    // Try to load from standard divine locations
    const char* config_paths[] = {
        ".metis.config",           // Current directory - developer's workspace
        ".metis/config",           // Local .metis directory - project specific
        "~/.metis/config",         // Home directory - user preferences
        NULL
    };

    bool config_loaded = false;
    for (int i = 0; config_paths[i] != NULL; i++) {
        if (metis_config_load(config_paths[i])) {
            printf("%s🌟 Divine Configuration:%s Loaded from %s%s%s\n",
                   METIS_SUCCESS, METIS_RESET,
                   METIS_CLICKABLE_LINK, config_paths[i], METIS_RESET);
            config_loaded = true;
            break;
        }
    }

    if (!config_loaded) {
        printf("%s📝 Divine Notice:%s Using default configuration (no config file found)\n",
               METIS_INFO, METIS_RESET);
    }

    return g_metis_config;
}

/**
 * Load configuration from divine wisdom file
 * @param config_path Path to configuration file
 * @return true if loading succeeded, false otherwise
 */
bool metis_config_load(const char* config_path) {
    if (!config_path || !g_metis_config) {
        return false;
    }

    FILE* file = fopen(config_path, "r");
    if (!file) {
        return false; // File doesn't exist or divine permissions prevent access
    }

    // Store the config file path with divine memory management
    if (g_metis_config->config_file_path) {
        free(g_metis_config->config_file_path);
    }
    g_metis_config->config_file_path = strdup(config_path);
    if (!g_metis_config->config_file_path) {
        fclose(file);
        return false;
    }

    // Parse the file line by line with divine patience
    char line[512]; // Increased buffer for longer configuration lines
    int line_number = 0;
    int parsed_lines = 0;
    int skipped_lines = 0;

    while (fgets(line, sizeof(line), file)) {
        line_number++;
        if (parse_config_line(g_metis_config, line)) {
            parsed_lines++;
        } else {
            skipped_lines++;
            printf("%s⚠️ Divine Warning:%s Skipped config line %d: %s%s%s",
                   METIS_WARNING, METIS_RESET, line_number,
                   METIS_TEXT_MUTED, line, METIS_RESET);
        }
    }

    fclose(file);

    printf("%s✨ Configuration Parsing:%s %d lines processed, %d parsed, %d skipped\n",
           METIS_INFO, METIS_RESET, line_number, parsed_lines, skipped_lines);

    return true;
}

/**
 * Save current configuration to divine wisdom file
 * @param config_path Path where to save configuration
 * @return true if saving succeeded, false otherwise
 */
bool metis_config_save(const char* config_path) {
    if (!config_path || !g_metis_config) {
        printf("%s💀 Save Error:%s Invalid parameters for configuration save\n",
               METIS_ERROR, METIS_RESET);
        return false;
    }

    FILE* file = fopen(config_path, "w");
    if (!file) {
        printf("%s💀 Save Error:%s Cannot write to config file: %s%s%s (%s%s%s)\n",
               METIS_ERROR, METIS_RESET,
               METIS_CLICKABLE_LINK, config_path, METIS_RESET,
               METIS_TEXT_MUTED, strerror(errno), METIS_RESET);
        return false;
    }

    // Write divine configuration header
    fprintf(file, "# METIS Wisdom Linter Configuration\n");
    fprintf(file, "# \"Perfect configuration is the foundation of divine wisdom\" - Fragment #49\n");
    fprintf(file, "# Generated automatically - edit with divine care\n");
    fprintf(file, "\n");

    // Write fragment preferences with divine organization
    fprintf(file, "# Fragment Preferences - Choose Your Wisdom\n");
    fprintf(file, "enable_memory_fragments=%s\n",
            g_metis_config->enable_memory_fragments ? "true" : "false");
    fprintf(file, "enable_docs_fragments=%s\n",
            g_metis_config->enable_docs_fragments ? "true" : "false");
    fprintf(file, "enable_daedalus_fragments=%s\n",
            g_metis_config->enable_daedalus_fragments ? "true" : "false");
    fprintf(file, "enable_emscripten_fragments=%s\n",
            g_metis_config->enable_emscripten_fragments ? "true" : "false");
    fprintf(file, "enable_philosophical_fragments=%s\n",
            g_metis_config->enable_philosophical_fragments ? "true" : "false");

    fprintf(file, "\n# Wisdom Progression - Your Journey\n");
    fprintf(file, "wisdom_level=%d\n", g_metis_config->current_wisdom_level);
    fprintf(file, "wisdom_points=%d\n", g_metis_config->total_wisdom_points);
    fprintf(file, "unlock_story_fragments=%s\n",
            g_metis_config->unlock_story_fragments ? "true" : "false");

    fprintf(file, "\n# Linting Strictness - Divine Balance\n");
    fprintf(file, "# Options: merciful, balanced, demanding\n");
    const char* strictness_names[] = {"merciful", "balanced", "demanding"};
    fprintf(file, "strictness=%s\n", strictness_names[g_metis_config->strictness]);

    fclose(file);

    // Update stored path with divine memory management
    if (g_metis_config->config_file_path) {
        free(g_metis_config->config_file_path);
    }
    g_metis_config->config_file_path = strdup(config_path);

    printf("%s✨ Configuration Saved:%s Divine wisdom written to %s%s%s\n",
           METIS_SUCCESS, METIS_RESET,
           METIS_CLICKABLE_LINK, config_path, METIS_RESET);

    return true;
}

/**
 * Get current configuration (for other divine modules)
 * @return Pointer to current configuration or NULL if not initialized
 */
MetisConfig_t* metis_config_get(void) {
    if (!g_metis_config) {
        printf("%s⚠️ Divine Warning:%s Configuration not initialized - call metis_config_init() first\n",
               METIS_WARNING, METIS_RESET);
    }
    return g_metis_config;
}

/**
 * Cleanup configuration with divine memory management
 */
void metis_config_cleanup(void) {
    if (g_metis_config) {
        if (g_metis_config->config_file_path) {
            free(g_metis_config->config_file_path);
        }
        free(g_metis_config);
        g_metis_config = NULL;

        printf("%s🧹 Divine Cleanup:%s Configuration memory released\n",
               METIS_INFO, METIS_RESET);
    }
}

/**
 * Print current configuration with divine beauty (for debugging and status)
 */
void metis_config_print(void) {
    if (!g_metis_config) {
        printf("%s💀 Configuration Error:%s No configuration loaded\n",
               METIS_ERROR, METIS_RESET);
        return;
    }

    // Beautiful divine configuration display
    printf("\n%s🌟 METIS CONFIGURATION STATUS 🌟%s\n",
           METIS_FRAGMENT_TITLE, METIS_RESET);
    printf("%s═══════════════════════════════════════════════════════════════%s\n",
           METIS_ACCENT, METIS_RESET);

    // Fragment preferences with divine colors
    printf("%sFragment Preferences:%s\n", METIS_PRIMARY, METIS_RESET);
    printf("  %s📖 Memory:%s %s\n", METIS_RED_LIGHTER, METIS_RESET,
           g_metis_config->enable_memory_fragments ?
           METIS_SUCCESS "✅ Enabled" METIS_RESET :
           METIS_TEXT_MUTED "❌ Disabled" METIS_RESET);
    printf("  %s📚 Documentation:%s %s\n", METIS_BLUE_LIGHT, METIS_RESET,
           g_metis_config->enable_docs_fragments ?
           METIS_SUCCESS "✅ Enabled" METIS_RESET :
           METIS_TEXT_MUTED "❌ Disabled" METIS_RESET);
    printf("  %s🔨 Daedalus:%s %s\n", METIS_GREEN_LIGHT, METIS_RESET,
           g_metis_config->enable_daedalus_fragments ?
           METIS_SUCCESS "✅ Enabled" METIS_RESET :
           METIS_TEXT_MUTED "❌ Disabled" METIS_RESET);
    printf("  %s🕸️ Emscripten:%s %s\n", METIS_ORANGE_LIGHT, METIS_RESET,
           g_metis_config->enable_emscripten_fragments ?
           METIS_SUCCESS "✅ Enabled" METIS_RESET :
           METIS_TEXT_MUTED "❌ Disabled" METIS_RESET);
    printf("  %s🧠 Philosophy:%s %s\n", METIS_BLUE_LIGHTER, METIS_RESET,
           g_metis_config->enable_philosophical_fragments ?
           METIS_SUCCESS "✅ Enabled" METIS_RESET :
           METIS_TEXT_MUTED "❌ Disabled" METIS_RESET);

    // Wisdom progression with divine beauty
    printf("\n%sWisdom Progression:%s\n", METIS_PRIMARY, METIS_RESET);
    printf("  %s🌟 Level:%s %s%d%s\n", METIS_ORANGE_LIGHT, METIS_RESET,
           METIS_BOLD, g_metis_config->current_wisdom_level, METIS_RESET);
    printf("  %s💎 Points:%s %s%d%s\n", METIS_BLUE_LIGHT, METIS_RESET,
           METIS_BOLD, g_metis_config->total_wisdom_points, METIS_RESET);
    printf("  %s📖 Story:%s %s\n", METIS_GREEN_LIGHT, METIS_RESET,
           g_metis_config->unlock_story_fragments ?
           METIS_SUCCESS "✅ Unlocked" METIS_RESET :
           METIS_TEXT_MUTED "❌ Locked" METIS_RESET);

    // Strictness with divine indicators
    const char* strictness_names[] = {"🤗 Merciful", "⚖️ Balanced", "⚡ Demanding"};
    const char* strictness_colors[] = {METIS_GREEN_LIGHT, METIS_BLUE_LIGHT, METIS_RED_LIGHT};
    printf("\n%sDivine Strictness:%s %s%s%s\n", METIS_PRIMARY, METIS_RESET,
           strictness_colors[g_metis_config->strictness],
           strictness_names[g_metis_config->strictness], METIS_RESET);

    // Configuration file info
    if (g_metis_config->config_file_path) {
        printf("\n%sConfiguration File:%s %s%s%s\n", METIS_PRIMARY, METIS_RESET,
               METIS_CLICKABLE_LINK, g_metis_config->config_file_path, METIS_RESET);
    } else {
        printf("\n%sConfiguration:%s %sUsing defaults (no file)%s\n",
               METIS_PRIMARY, METIS_RESET, METIS_TEXT_MUTED, METIS_RESET);
    }

    printf("%s═══════════════════════════════════════════════════════════════%s\n\n",
           METIS_ACCENT, METIS_RESET);
}

/**
 * Update a specific configuration value dynamically
 * @param key Configuration key to update
 * @param value New value for the key
 * @return true if update succeeded, false otherwise
 */
bool metis_config_set(const char* key, const char* value) {
    if (!g_metis_config || !key || !value) {
        return false;
    }

    // Create a temporary line for parsing
    char temp_line[512];
    snprintf(temp_line, sizeof(temp_line), "%s=%s", key, value);

    bool success = parse_config_line(g_metis_config, temp_line);
    if (success) {
        printf("%s✨ Configuration Updated:%s %s%s%s = %s%s%s\n",
               METIS_SUCCESS, METIS_RESET,
               METIS_BOLD, key, METIS_RESET,
               METIS_ACCENT, value, METIS_RESET);
    } else {
        printf("%s⚠️ Configuration Error:%s Unknown key '%s%s%s'\n",
               METIS_WARNING, METIS_RESET,
               METIS_TEXT_MUTED, key, METIS_RESET);
    }

    return success;
}

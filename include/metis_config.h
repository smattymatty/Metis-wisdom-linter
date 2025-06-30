/*
 * METIS WISDOM LINTER - Configuration System Header
 * DRAFT 3 - Minimum Working Consciousness
 *
 * "Even divine wisdom must start with simple truths" - Metis Fragment #12
 */

#ifndef METIS_CONFIG_H
#define METIS_CONFIG_H

#include <stdbool.h>

// Wisdom strictness levels - MUST be defined before the struct that uses it
enum WisdomStrictness {
    MERCIFUL = 0,      // Gentle guidance
    BALANCED = 1,      // Standard wisdom
    DEMANDING = 2      // Divine perfection expected
};

// Configuration structure
typedef struct {
    // Fragment preferences - which wisdom to receive
    bool enable_memory_fragments;
    bool enable_docs_fragments;
    bool enable_daedalus_fragments;
    bool enable_emscripten_fragments;
    bool enable_philosophical_fragments;

    // Wisdom progression settings
    int current_wisdom_level;
    int total_wisdom_points;
    bool unlock_story_fragments;

    // Linting strictness - now the enum is properly defined above
    enum WisdomStrictness strictness;

    // Configuration file path
    char* config_file_path;
} MetisConfig_t;

// Configuration management functions
MetisConfig_t* metis_config_init(void);
bool metis_config_load(const char* config_path);
bool metis_config_save(const char* config_path);
MetisConfig_t* metis_config_get(void);
void metis_config_cleanup(void);
void metis_config_print(void);

#endif // METIS_CONFIG_H

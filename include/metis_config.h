// src/config/metis_config.h - Configuration file for the Metis linter
// INSERT WISDOM HERE

#ifndef METIS_CONFIG_H
#define METIS_CONFIG_H

#include <stdbool.h> // For bool type

// Wisdom strictness levels - MUST be defined before the struct that uses it
// This enum defines the different levels of strictness Metis can apply
// during linting and wisdom delivery.
enum WisdomStrictness {
    MERCIFUL = 0,      // Gentle guidance, forgiving of minor imperfections.
    BALANCED = 1,      // Standard wisdom, applying balanced rules and recommendations.
    DEMANDING = 2      // Divine perfection expected, enforcing strict adherence to wisdom.
};

// Configuration structure
// This structure holds all configurable parameters for the Metis Wisdom Linter.
typedef struct {
    // Fragment preferences - which wisdom to receive (true to enable, false to disable)
    bool enable_memory_fragments;      // Enable wisdom fragments related to memory management.
    bool enable_docs_fragments;        // Enable wisdom fragments related to documentation.
    bool enable_daedalus_fragments;    // Enable wisdom fragments related to Daedalus's technical guidance.
    bool enable_emscripten_fragments;  // Enable wisdom fragments specific to Emscripten/web development.
    bool enable_philosophical_fragments; // Enable wisdom fragments related to coding philosophy.

    // Wisdom progression settings
    int current_wisdom_level;          // The current divine wisdom level of the user.
    int total_wisdom_points;           // Accumulated wisdom points that determine the user's level.
    bool unlock_story_fragments;       // Flag to enable/disable the delivery of story-driven fragments.

    // Linting strictness - now the enum is properly defined above
    enum WisdomStrictness strictness;  // The overall strictness level for linting and recommendations.

    // Configuration file path
    char* config_file_path;            // Dynamically allocated string holding the path to the loaded config file.
} MetisConfig_t;

// --- Configuration Management Functions ---

/**
 * @brief Initializes the Metis configuration system.
 *
 * Allocates memory for the global configuration structure and sets default values.
 * It then attempts to load configuration from predefined file paths (e.g., .metis.config,
 * ~/.metis/config). If no configuration file is found, default values are used.
 *
 * @return A pointer to the initialized MetisConfig_t structure, or NULL if
 * initialization fails (e.g., memory allocation error).
 */
MetisConfig_t* metis_config_init(void);

/**
 * @brief Loads configuration settings from a specified file path.
 *
 * Reads key-value pairs from the given configuration file and updates
 * the global MetisConfig_t structure. Lines starting with '#' are ignored.
 * If the file cannot be opened, false is returned.
 *
 * @param config_path The path to the configuration file to load.
 * @return true if the configuration was successfully loaded, false otherwise.
 */
bool metis_config_load(const char* config_path);

/**
 * @brief Saves the current configuration settings to a specified file path.
 *
 * Writes the current state of the global MetisConfig_t structure to the
 * given file in a key=value format, including comments.
 *
 * @param config_path The path to the file where the configuration should be saved.
 * @return true if the configuration was successfully saved, false otherwise.
 */
bool metis_config_save(const char* config_path);

/**
 * @brief Retrieves a pointer to the current global Metis configuration.
 *
 * This function provides access to the active configuration settings for other
 * modules in the Metis system. It's important to call `metis_config_init()`
 * before calling this function.
 *
 * @return A pointer to the global MetisConfig_t structure, or NULL if the
 * configuration has not been initialized.
 */
MetisConfig_t* metis_config_get(void);

/**
 * @brief Cleans up and frees memory associated with the Metis configuration.
 *
 * Releases memory allocated for the global configuration structure, including
 * the `config_file_path` string. Should be called at program exit to prevent
 * memory leaks.
 */
void metis_config_cleanup(void);

/**
 * @brief Prints the current Metis configuration status to the console.
 *
 * Displays all current configuration settings in a human-readable,
 * color-coded format, useful for debugging and status checks.
 */
void metis_config_print(void);

/**
 * @brief Dynamically updates a specific configuration setting by key and value.
 *
 * Parses a key-value pair string (e.g., "strictness=demanding") and applies
 * the change to the in-memory global configuration. This allows for runtime
 * modification of settings without directly editing the config file.
 *
 * @param key The configuration key (e.g., "enable_docs_fragments", "strictness").
 * @param value The new value for the configuration key (e.g., "true", "balanced").
 * @return true if the key was found and updated successfully, false if the key
 * is unknown or parameters are invalid.
 */
bool metis_config_set(const char* key, const char* value);

#endif // METIS_CONFIG_H

/**
 * @file metis_types.h
 * @brief Type definitions for the METIS Wisdom Linter
 *
 * This header defines the core types used throughout the METIS Wisdom Linter system,
 * including configuration structures, result codes, and other essential data types.
 *
 * @copyright Copyright (c) 2024 METIS Team
 * @license MIT License
 */

#ifndef METIS_TYPES_H
#define METIS_TYPES_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Result codes for METIS operations
 */
typedef enum {
    METIS_SUCCESS = 0,                  /**< Operation completed successfully */
    METIS_ERROR_INVALID_ARGUMENT = -1,  /**< Invalid argument provided */
    METIS_ERROR_FILE_NOT_FOUND = -2,    /**< Specified file could not be found */
    METIS_ERROR_PARSE_FAILED = -3,      /**< Failed to parse source code */
    METIS_ERROR_CONFIG_INVALID = -4,    /**< Invalid configuration */
    METIS_ERROR_MEMORY = -5,            /**< Memory allocation failure */
    METIS_ERROR_IO = -6,                /**< Input/output error */
    METIS_ERROR_INTERNAL = -7           /**< Internal error */
} MetisResult;

/**
 * @brief Version information structure
 */
typedef struct {
    uint32_t major;     /**< Major version number */
    uint32_t minor;     /**< Minor version number */
    uint32_t patch;     /**< Patch version number */
    const char* build;  /**< Build identifier string */
} MetisVersion;

/**
 * @brief Configuration options for METIS
 */
typedef struct {
    const char* config_file;     /**< Path to configuration file */
    const char* style_template;  /**< Name of style template to use */
    int indent_size;            /**< Number of spaces for indentation */
    int max_line_length;        /**< Maximum line length */
    bool enable_colors;         /**< Enable colored output */
    bool verbose;               /**< Enable verbose output */
} MetisConfig;

/**
 * @brief Linting severity levels
 */
typedef enum {
    METIS_SEVERITY_ERROR,     /**< Critical issues that must be fixed */
    METIS_SEVERITY_WARNING,   /**< Potential problems that should be reviewed */
    METIS_SEVERITY_INFO,      /**< Informational messages */
    METIS_SEVERITY_HINT       /**< Suggestions for improvement */
} MetisSeverity;

/**
 * @brief Source code location
 */
typedef struct {
    const char* file;  /**< Source file path */
    size_t line;       /**< Line number (1-based) */
    size_t column;     /**< Column number (1-based) */
} MetisLocation;

/**
 * @brief Diagnostic message from linting
 */
typedef struct {
    MetisSeverity severity;    /**< Severity level */
    MetisLocation location;    /**< Source location */
    const char* message;       /**< Diagnostic message */
    const char* rule_id;       /**< Identifier of the rule that triggered */
    const char* suggestion;    /**< Suggested fix (if available) */
} MetisDiagnostic;

/**
 * @brief Linting options
 */
typedef struct {
    bool fix;                  /**< Attempt to fix issues automatically */
    bool ignore_warnings;      /**< Only report errors */
    const char* ruleset;       /**< Name of ruleset to use */
    const char** ignore_rules; /**< Array of rule IDs to ignore */
    size_t ignore_count;       /**< Number of ignored rules */
} MetisLintOptions;

/**
 * @brief Formatting options
 */
typedef struct {
    bool dry_run;             /**< Show changes without applying them */
    bool in_place;            /**< Modify files in place */
    const char* style;        /**< Style name to apply */
} MetisFormatOptions;

/**
 * @brief Achievement status
 */
typedef enum {
    METIS_ACHIEVEMENT_LOCKED,      /**< Achievement not yet earned */
    METIS_ACHIEVEMENT_UNLOCKED,    /**< Achievement earned */
    METIS_ACHIEVEMENT_IN_PROGRESS  /**< Achievement partially completed */
} MetisAchievementStatus;

/**
 * @brief Achievement information
 */
typedef struct {
    const char* id;           /**< Unique achievement identifier */
    const char* title;        /**< Achievement title */
    const char* description;  /**< Achievement description */
    float progress;          /**< Progress toward completion (0.0 - 1.0) */
    MetisAchievementStatus status; /**< Current achievement status */
} MetisAchievement;

/**
 * @brief Wisdom level and progress
 */
typedef struct {
    uint32_t level;          /**< Current wisdom level */
    uint32_t experience;     /**< Current experience points */
    uint32_t next_level;     /**< Experience needed for next level */
    size_t fragments_found;  /**< Number of wisdom fragments discovered */
    size_t total_fragments;  /**< Total number of wisdom fragments */
} MetisWisdomProgress;

/**
 * @brief Placeholder for Daedalus String Type
 * This is a simplified representation for linting purposes.
 * The actual definition would be in a Daedalus library header.
 */
typedef struct dString_t {
    char* str;      /**< Null-terminated string buffer */
    size_t length;  /**< Current length of the string */
    size_t capacity; /**< Allocated capacity of the buffer */
} dString_t;

#ifdef __cplusplus
}
#endif

#endif /* METIS_TYPES_H */
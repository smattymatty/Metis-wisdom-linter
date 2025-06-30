/**
 * @file metis.h
 * @brief Main public header for the METIS Wisdom Linter
 *
 * This header provides the primary public interface for the METIS Wisdom Linter,
 * a tool that combines code analysis with programming wisdom delivery through
 * narrative elements.
 *
 * @copyright Copyright (c) 2024 METIS Team
 * @license MIT License
 */

#ifndef METIS_H
#define METIS_H

#include "metis_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize METIS with configuration options
 *
 * @param config Pointer to configuration structure
 * @return MetisResult Status code indicating success or failure
 */
MetisResult metis_init(const MetisConfig* config);

/**
 * @brief Analyze source files for linting issues
 *
 * @param files Array of file paths to analyze
 * @param file_count Number of files in the array
 * @param options Linting options
 * @return MetisResult Status code indicating success or failure
 */
MetisResult metis_lint(const char** files, size_t file_count, const MetisLintOptions* options);

/**
 * @brief Format source files according to style rules
 *
 * @param files Array of file paths to format
 * @param file_count Number of files in the array
 * @param options Formatting options
 * @return MetisResult Status code indicating success or failure
 */
MetisResult metis_format(const char** files, size_t file_count, const MetisFormatOptions* options);

/**
 * @brief Get current wisdom progress
 *
 * @param progress Pointer to structure to receive progress data
 * @return MetisResult Status code indicating success or failure
 */
MetisResult metis_get_wisdom_progress(MetisWisdomProgress* progress);

/**
 * @brief Get available achievements
 *
 * @param achievements Pointer to array to receive achievement data
 * @param count Pointer to receive number of achievements
 * @return MetisResult Status code indicating success or failure
 */
MetisResult metis_get_achievements(MetisAchievement** achievements, size_t* count);

/**
 * @brief Get the last error message
 *
 * @return const char* Error message string or NULL if no error
 */
const char* metis_get_last_error(void);

/**
 * @brief Clean up METIS resources
 *
 * Call this function when you're done using METIS to clean up any allocated resources.
 */
void metis_cleanup(void);

/**
 * @brief Get METIS version information
 *
 * @param version Pointer to structure to receive version data
 */
void metis_get_version(MetisVersion* version);

#ifdef __cplusplus
}
#endif

#endif /* METIS_H */
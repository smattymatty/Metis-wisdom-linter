/*
 * METIS WISDOM LINTER - Linter Engine Header
 * DRAFT 3 - Working Divine Analysis
 *
 * "I see patterns in chaos, wisdom in struggle, hope in repetition" - Metis Fragment #15
 */

#ifndef METIS_LINTER_H
#define METIS_LINTER_H

#include <stdbool.h>

// Core linting functions
int metis_lint_file(const char* file_path);
int metis_lint_directory(const char* dir_path);

// Initialization and cleanup
bool metis_linter_init(void);
void metis_linter_cleanup(void);

#endif // METIS_LINTER_H

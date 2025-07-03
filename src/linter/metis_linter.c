/* metis_linter.c - Core linting engine for divine documentation wisdom */
// INSERT WISDOM HERE

#define _POSIX_C_SOURCE 200809L  // For strdup

#include "metis_linter.h"
#include "metis_config.h"
#include "fragment_engine.h"
#include "metis_colors.h"
#include "c_parser.h"
#include "cross_reference.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

// =============================================================================
// DIVINE VIOLATION TRACKING
// =============================================================================

/*
 * Types of violations that Metis detects in her wisdom domain
 */
typedef enum {
    DOCS_VIOLATION,         // Missing or improper documentation
    DAEDALUS_SUGGESTION,    // Dangerous functions that Daedalus can replace
    PHILOSOPHICAL_VIOLATION, // Code quality and wisdom issues
    HEADER_VIOLATION        // Missing file headers (filename/wisdom)
} ViolationType_t;

/*
 * Severity levels for divine guidance
 */
typedef enum {
    SEVERITY_INFO,
    SEVERITY_WARNING,
    SEVERITY_ERROR
} Severity_t;

/*
 * Individual violation with precise positioning for divine guidance
 */
typedef struct {
    char* file_path;
    int line_number;
    int column;
    char* violation_message;
    char* suggestion;
    ViolationType_t type;
    Severity_t severity;
} LintViolation_t;

/*
 * Collection of violations with divine organization
 */
typedef struct ViolationList {
    LintViolation_t* violations;
    int count;
    int capacity;
} ViolationList_t;

// =============================================================================
// VIOLATION MANAGEMENT
// =============================================================================

/*
 * Initialize violation list with divine blessing
 */
static ViolationList_t* create_violation_list(void) {
    ViolationList_t* list = malloc(sizeof(ViolationList_t));
    if (!list) return NULL;

    list->violations = malloc(sizeof(LintViolation_t) * 100);
    if (!list->violations) {
        free(list);
        return NULL;
    }

    list->count = 0;
    list->capacity = 100;
    return list;
}

/*
 * Add a violation with precise location tracking
 */
static void add_violation(ViolationList_t* list, const char* file_path, int line_number,
                         int column, const char* message, const char* suggestion,
                         ViolationType_t type, Severity_t severity) {
    if (!list || !file_path || !message) return;

    // Expand capacity if needed (divine growth)
    if (list->count >= list->capacity) {
        list->capacity *= 2;
        list->violations = realloc(list->violations,
                                 sizeof(LintViolation_t) * list->capacity);
        if (!list->violations) return;
    }

    LintViolation_t* v = &list->violations[list->count];
    v->file_path = strdup(file_path);
    v->line_number = line_number;
    v->column = column;
    v->violation_message = strdup(message);
    v->suggestion = suggestion ? strdup(suggestion) : NULL;
    v->type = type;
    v->severity = severity;
    list->count++;
}

/*
 * Free violation list with compassionate cleanup
 */
static void free_violation_list(ViolationList_t* list) {
    if (!list) return;

    for (int i = 0; i < list->count; i++) {
        free(list->violations[i].file_path);
        free(list->violations[i].violation_message);
        if (list->violations[i].suggestion) {
            free(list->violations[i].suggestion);
        }
    }
    free(list->violations);
    free(list);
}

// =============================================================================
// DIVINE DOCUMENTATION ANALYSIS
// =============================================================================

/*
 * Check file headers using divine parser wisdom
 */
static int check_file_headers_with_parser(ParsedFile_t* parsed, ViolationList_t* violations) {
    if (!parsed || !violations) return 0;

    int issues_found = 0;
    const char* file_path = parsed->file_path;
    const char* filename = c_parser_extract_filename(file_path);

    // Check for proper filename header (first line)
    if (!c_parser_has_proper_filename_header(parsed, filename)) {
        char message[256];
        snprintf(message, sizeof(message),
                "File missing proper filename comment on first line");

        char suggestion[512];
        snprintf(suggestion, sizeof(suggestion),
                "Add: /* %s - brief description */", filename);

        add_violation(violations, file_path, 1, 1, message, suggestion,
                     HEADER_VIOLATION, SEVERITY_WARNING);
        issues_found++;
    }

    // Check for proper purpose line (second line) - FIXED LOGIC
    if (!c_parser_has_proper_purpose_line(parsed)) {
        char message[256];
        snprintf(message, sizeof(message),
                "File missing meaningful purpose line comment on second line");

        add_violation(violations, file_path, 2, 1, message,
                     "Add a meaningful comment describing the file's purpose, any other files it integrates with, and any other relevant information",
                     HEADER_VIOLATION, SEVERITY_WARNING);
        issues_found++;
    }

    return issues_found;
}
/*
 * Check function documentation with parser precision
 */
static int check_function_docs_with_parser(ParsedFile_t* parsed, ViolationList_t* violations) {
    if (!parsed || !violations) return 0;

    int issues_found = 0;
    const char* file_path = parsed->file_path;

    // Check each function for documentation
    for (int i = 0; i < parsed->function_count; i++) {
        FunctionInfo_t* func = &parsed->functions[i];

        // Skip main function and static functions (they might be internal)
        if (strcmp(func->name, "main") == 0 || func->is_static) {
            continue;
        }

        if (!c_parser_has_documentation_for_function(parsed, func->name)) {
            char message[128];
            snprintf(message, sizeof(message),
                    "Function '%s' lacks documentation", func->name);

            add_violation(violations, file_path, func->line_number, func->column,
                        message,
                        "Add comment block explaining purpose, parameters, and return value",
                        DOCS_VIOLATION, SEVERITY_INFO);
            issues_found++;
        }
    }

    return issues_found;
}

// =============================================================================
// DAEDALUS OPPORTUNITY DETECTION
// =============================================================================

/*
 * Check for dangerous functions with token-level precision
 */
static int check_daedalus_with_parser(ParsedFile_t* parsed, ViolationList_t* violations) {
    if (!parsed || !violations) return 0;

    int issues_found = 0;
    const char* file_path = parsed->file_path;

    // Check for dangerous function usage in tokens
    for (int i = 0; i < parsed->token_count; i++) {
        Token_t* token = &parsed->tokens[i];

        if (token->type == TOKEN_IDENTIFIER &&
            c_parser_is_dangerous_function(token->value)) {

            // Make sure it's a function call (next token should be '(')
            if (i + 1 < parsed->token_count &&
                parsed->tokens[i + 1].type == TOKEN_PUNCTUATION &&
                strcmp(parsed->tokens[i + 1].value, "(") == 0) {

                char message[128];
                snprintf(message, sizeof(message),
                        "Unsafe function '%s()' detected", token->value);

                const char* suggestion = NULL;

                // Provide specific Daedalus suggestions with divine precision
                if (strcmp(token->value, "malloc") == 0) {
                    suggestion = "Use d_InitArray() for dynamic growth or d_InitStaticArray() for fixed capacity";
                } else if (strcmp(token->value, "realloc") == 0) {
                    suggestion = "Use d_ResizeArray() or d_GrowArray() for safe memory expansion";
                } else if (strcmp(token->value, "free") == 0) {
                    suggestion = "Use d_DestroyArray() or d_DestroyStaticArray() for automatic cleanup";
                } else if (strcmp(token->value, "calloc") == 0) {
                    suggestion = "Use d_InitArray() which zero-initializes elements automatically";
                    
                // String Function Replacements
                } else if (strcmp(token->value, "strcpy") == 0) {
                    suggestion = "Use d_SetString() or d_AppendString() for safe string assignment";
                } else if (strcmp(token->value, "strncpy") == 0) {
                    suggestion = "Use d_SetString() or d_AppendStringN() for bounded string copying";
                } else if (strcmp(token->value, "strcat") == 0) {
                    suggestion = "Use d_AppendString() for safe string concatenation";
                } else if (strcmp(token->value, "strncat") == 0) {
                    suggestion = "Use d_AppendStringN() for bounded string concatenation";
                } else if (strcmp(token->value, "strcmp") == 0) {
                    suggestion = "Use d_CompareStrings() or d_CompareStringToCString() for dString_t objects";
                } else if (strcmp(token->value, "strncmp") == 0) {
                    suggestion = "Use d_CompareStrings() with d_SliceString() for bounded comparison";
                } else if (strcmp(token->value, "strlen") == 0) {
                    suggestion = "Use d_GetStringLength() for dString_t objects";
                } else if (strcmp(token->value, "strdup") == 0) {
                    suggestion = "Create new dString_t with d_InitString() and d_SetString()";

                // Printf Family Replacements  
                } else if (strcmp(token->value, "printf") == 0) {
                    suggestion = "Use d_LogInfoF() for structured, filterable output";
                } else if (strcmp(token->value, "fprintf") == 0) {
                    suggestion = "Use d_LogInfoF() with file handlers or d_FormatString() to dString_t";
                } else if (strcmp(token->value, "sprintf") == 0) {
                    suggestion = "Use d_FormatString() for safe string formatting";
                } else if (strcmp(token->value, "snprintf") == 0) {
                    suggestion = "Use d_FormatString() which automatically manages buffer size";
                } else if (strcmp(token->value, "vprintf") == 0) {
                    suggestion = "Use Daedalus logging system with d_LogF() variants";
                } else if (strcmp(token->value, "vsprintf") == 0) {
                    suggestion = "Use d_FormatString() which handles variadic arguments safely";

                // Input Function Replacements
                } else if (strcmp(token->value, "gets") == 0) {
                    suggestion = "Use d_AppendString() with safe input validation";
                } else if (strcmp(token->value, "fgets") == 0) {
                    suggestion = "Use d_CreateStringFromFile() or d_AppendString() with bounds checking";
                } else if (strcmp(token->value, "scanf") == 0) {
                    suggestion = "Use d_LogDebugF() for debugging and proper input validation";
                } else if (strcmp(token->value, "sscanf") == 0) {
                    suggestion = "Use d_SplitString() and d_CompareStringToCString() for parsing";

                // File Operations
                } else if (strcmp(token->value, "fopen") == 0) {
                    suggestion = "Consider d_CreateStringFromFile() for simple file reading";
                } else if (strcmp(token->value, "tmpnam") == 0 || strcmp(token->value, "tempnam") == 0) {
                    suggestion = "Use secure temporary file creation with proper cleanup";

                // Memory Functions
                } else if (strcmp(token->value, "memcpy") == 0) {
                    suggestion = "Use d_AppendString() for string data or verify bounds manually";
                } else if (strcmp(token->value, "memmove") == 0) {
                    suggestion = "Use d_SliceString() and d_AppendString() for string manipulation";
                } else if (strcmp(token->value, "memset") == 0) {
                    suggestion = "Use d_ClearString() for string data or d_InitArray() for zero-initialization";

                // Array/Buffer Operations
                } else if (strcmp(token->value, "qsort") == 0) {
                    suggestion = "Use d_SortArray() or d_SortStaticArray() with built-in comparison functions";
                } else if (strcmp(token->value, "bsearch") == 0) {
                    suggestion = "Use d_FindInArray() or d_FindInStaticArray() with sorted arrays";

                // Character Classification (less critical but worth mentioning)
                } else if (strstr(token->value, "to") == token->value && strlen(token->value) > 2) {
                    // toupper, tolower, etc.
                    suggestion = "Consider using d_FormatString() with format specifiers for case conversion";

                } else {
                    suggestion = "Consider using Daedalus library alternatives for safety";
                }

                add_violation(violations, file_path, token->line, token->column,
                            message, suggestion, DAEDALUS_SUGGESTION, SEVERITY_INFO);
                issues_found++;
            }
        }
    }

    return issues_found;
}

// =============================================================================
// PHILOSOPHICAL WISDOM ANALYSIS
// =============================================================================

/*
 * Check for philosophical violations in comments
 */
static int check_philosophy_with_parser(ParsedFile_t* parsed, ViolationList_t* violations) {
    if (!parsed || !violations) return 0;

    int issues_found = 0;
    const char* file_path = parsed->file_path;

    // Check comment tokens for philosophical markers
    for (int i = 0; i < parsed->token_count; i++) {
        Token_t* token = &parsed->tokens[i];

        if (token->type == TOKEN_COMMENT_LINE || token->type == TOKEN_COMMENT_BLOCK) {
            struct WisdomPattern {
                const char* pattern;
                const char* message;
                const char* suggestion;
            } patterns[] = {
                {"TODO", "TODO comment found", "Consider creating a proper issue or fixing immediately"},
                {"FIXME", "FIXME comment found", "This indicates known broken code - prioritize fixing"},
                {"HACK", "HACK comment found", "Replace this hack with a proper solution"},
                {"XXX", "XXX marker found", "This usually indicates problematic code"},
                {NULL, NULL, NULL}
            };

            for (int j = 0; patterns[j].pattern; j++) {
                if (strstr(token->value, patterns[j].pattern)) {
                    add_violation(violations, file_path, token->line, token->column,
                                patterns[j].message, patterns[j].suggestion,
                                PHILOSOPHICAL_VIOLATION, SEVERITY_INFO);
                    issues_found++;
                    break; // Only report one pattern per comment
                }
            }
        }
    }

    return issues_found;
}

/*
 * Check function complexity for philosophical guidance
 */
static int check_complexity_wisdom(ParsedFile_t* parsed, ViolationList_t* violations) {
    if (!parsed || !violations) return 0;

    int issues_found = 0;
    const char* file_path = parsed->file_path;

    // Analyze each function for complexity
    for (int i = 0; i < parsed->function_count; i++) {
        FunctionInfo_t* func = &parsed->functions[i];

        ComplexityAnalysis_t analysis = c_parser_analyze_function_complexity(parsed, func->name);

        // Check for overly complex functions
        if (analysis.complexity_score > 10) {
            char message[128];
            snprintf(message, sizeof(message),
                    "Function '%s' has high complexity (score: %d)",
                    func->name, analysis.complexity_score);

            add_violation(violations, file_path, func->line_number, func->column,
                        message,
                        "Consider breaking this function into smaller, more focused functions",
                        PHILOSOPHICAL_VIOLATION, SEVERITY_WARNING);
            issues_found++;
        }

        // Check for deeply nested functions
        if (analysis.has_deep_nesting) {
            char message[128];
            snprintf(message, sizeof(message),
                    "Function '%s' has deep nesting (depth: %d)",
                    func->name, analysis.nesting_depth);

            add_violation(violations, file_path, func->line_number, func->column,
                        message,
                        "Consider extracting nested logic into separate functions for clarity",
                        PHILOSOPHICAL_VIOLATION, SEVERITY_INFO);
            issues_found++;
        }

        // Check for very long functions
        if (analysis.function_length > 50) {
            char message[128];
            snprintf(message, sizeof(message),
                    "Function '%s' is very long (%d lines)",
                    func->name, analysis.function_length);

            add_violation(violations, file_path, func->line_number, func->column,
                        message,
                        "Consider breaking this function into smaller, more focused functions",
                        PHILOSOPHICAL_VIOLATION, SEVERITY_INFO);
            issues_found++;
        }
    }

    return issues_found;
}

// =============================================================================
// CONTENT ANALYSIS ENGINE
// =============================================================================
/*
 * Check header documentation format violations
 */
static int check_header_doc_format_with_parser(ParsedFile_t* parsed, ViolationList_t* violations) {
    if (!parsed || !violations) return 0;

    int issues_found = 0;
    const char* file_path = parsed->file_path;

    // Check all files for proper documentation format
    for (int i = 0; i < parsed->function_count; i++) {
        FunctionInfo_t* func = &parsed->functions[i];

        // --- ADD THIS EXCEPTION ---
        // The 'main' function is a special case. Also skip static functions.
        if (strcmp(func->name, "main") == 0 || func->is_static) {
            continue;
        }

        if (c_parser_has_documentation_for_function(parsed, func->name)) {
            if (!c_parser_has_proper_header_doc_format(parsed, func->name)) {
                char message[256];
                snprintf(message, sizeof(message),
                        "Function '%s' documentation violates one-line format",
                        func->name);

                add_violation(violations, file_path, func->line_number, func->column,
                            message,
                            "Documentation must have: one-line description, blank line, then parameters/details",
                            DOCS_VIOLATION, SEVERITY_WARNING);
                issues_found++;
            }
        }
    }

    return issues_found;
}
/*
 * Find corresponding header file for a C source file
 */
static char* find_header_file(const char* c_file_path) {
    if (!c_file_path) return NULL;

    // Extract directory and base name
    char* last_slash = strrchr(c_file_path, '/');
    if (!last_slash) last_slash = strrchr(c_file_path, '\\');

    const char* base_name = last_slash ? last_slash + 1 : c_file_path;

    // Remove .c extension
    char base_no_ext[256];
    strncpy(base_no_ext, base_name, sizeof(base_no_ext) - 1);
    base_no_ext[sizeof(base_no_ext) - 1] = '\0';

    char* dot = strrchr(base_no_ext, '.');
    if (dot) *dot = '\0';

    // Try include/ directory first
    char include_path[512];
    if (last_slash) {
        // Extract directory part
        int dir_len = last_slash - c_file_path + 1;
        strncpy(include_path, c_file_path, dir_len);
        include_path[dir_len] = '\0';

        // Go up one level and try include/
        char* parent_slash = include_path + dir_len - 2; // Back up before the trailing slash
        while (parent_slash > include_path && *parent_slash != '/' && *parent_slash != '\\') {
            parent_slash--;
        }
        if (parent_slash > include_path) {
            parent_slash[1] = '\0'; // Keep the slash
            strcat(include_path, "include/");
            strcat(include_path, base_no_ext);
            strcat(include_path, ".h");

            // Check if this file exists
            FILE* test = fopen(include_path, "r");
            if (test) {
                fclose(test);
                return strdup(include_path);
            }
        }
    }

    // Try same directory as source file
    char same_dir_path[512];
    if (last_slash) {
        int dir_len = last_slash - c_file_path + 1;
        strncpy(same_dir_path, c_file_path, dir_len);
        same_dir_path[dir_len] = '\0';
        strcat(same_dir_path, base_no_ext);
        strcat(same_dir_path, ".h");
    } else {
        snprintf(same_dir_path, sizeof(same_dir_path), "%s.h", base_no_ext);
    }

    // Check if this file exists
    FILE* test = fopen(same_dir_path, "r");
    if (test) {
        fclose(test);
        return strdup(same_dir_path);
    }

    return NULL; // No header file found
}

/*
 * Check header file documentation format when analyzing implementation
 */
static int check_corresponding_header_format(const char* c_file_path, ViolationList_t* violations) {
    if (!c_file_path || !violations) return 0;

    // Only check .c files
    const char* ext = strrchr(c_file_path, '.');
    if (!ext || strcmp(ext, ".c") != 0) {
        return 0;
    }

    // Find corresponding header file
    char* header_path = find_header_file(c_file_path);
    if (!header_path) {
        return 0; // No header file found, that's okay
    }

    // Parse the header file
    ParsedFile_t* header_parsed = c_parser_parse_file(header_path);
    if (!header_parsed) {
        free(header_path);
        return 0;
    }

    int issues_found = 0;

    // Check header documentation format for all functions
    for (int i = 0; i < header_parsed->function_count; i++) {
        FunctionInfo_t* func = &header_parsed->functions[i];

        // Skip static functions in headers (shouldn't be there anyway)
        if (func->is_static) continue;

        if (c_parser_has_documentation_for_function(header_parsed, func->name)) {
            if (!c_parser_has_proper_header_doc_format(header_parsed, func->name)) {
                char message[256];
                snprintf(message, sizeof(message),
                        "Function '%s' header documentation violates one-line format (in %s)",
                        func->name, header_path);

                add_violation(violations, header_path, func->line_number, func->column,
                            message,
                            "Header docs must have: one-line description, blank line, then parameters/details",
                            DOCS_VIOLATION, SEVERITY_WARNING);
                issues_found++;
            }
        }
    }

    c_parser_free_parsed_file(header_parsed);
    free(header_path);
    return issues_found;
}

/*
 * Analyze file content using divine parser wisdom
 */
static int analyze_file_content(const char* file_path, const char* content, ViolationList_t* violations) {
    if (!file_path || !content || !violations) return 0;

    // Parse the content using the divine C parser
    ParsedFile_t* parsed = c_parser_parse_content(content, file_path);
    if (!parsed) {
        // If parsing fails, we can't do much analysis
        char message[256];
        snprintf(message, sizeof(message), "Failed to parse file - analysis limited");
        add_violation(violations, file_path, 1, 1, message,
                     "Check for syntax errors or encoding issues",
                     DOCS_VIOLATION, SEVERITY_WARNING);
        return 1;
    }

    int issues_found = 0;

    // Divine file header analysis
    issues_found += check_file_headers_with_parser(parsed, violations);

    // Enhanced function documentation checking (for all files)
    issues_found += check_function_docs_with_parser(parsed, violations);

    // THE FIX: Only apply the strict header documentation format check to .h files
    const char* ext = strrchr(file_path, '.');
    if (ext && strcmp(ext, ".h") == 0) {
        issues_found += check_header_doc_format_with_parser(parsed, violations);
    }

    // If this is a .c file, also check its corresponding header file's format
    // (This check remains as it correctly targets .c files looking for their .h counterparts)
    issues_found += check_corresponding_header_format(file_path, violations);

    // Enhanced Daedalus opportunities detection
    issues_found += check_daedalus_with_parser(parsed, violations);

    // New: Check for unsafe strcmp(dString_t->str, ...) usage with enhanced contextual fragments
    UnsafeStrcmpUsage_t* unsafe_strcmp_usages = NULL;
    int unsafe_strcmp_count = 0;
    if (c_parser_detect_unsafe_strcmp_dstring_usage(parsed, &unsafe_strcmp_usages, &unsafe_strcmp_count)) {
        for (int i = 0; i < unsafe_strcmp_count; i++) {
            char message[256];
            char suggestion[512];
            const char* violation_type = "unsafe_strcmp_generic";
            
            if (unsafe_strcmp_usages[i].is_dstring_vs_cstring) {
                snprintf(message, sizeof(message), "Unsafe `strcmp` with `dString_t->str` and C-string detected");
                snprintf(suggestion, sizeof(suggestion), "Replace `strcmp(%s, %s)` with `d_CompareStringToCString(%s, %s)`", 
                        unsafe_strcmp_usages[i].variable1, unsafe_strcmp_usages[i].variable2,
                        unsafe_strcmp_usages[i].variable1, unsafe_strcmp_usages[i].variable2);
                violation_type = "unsafe_strcmp_dstring_cstring";
            } else if (unsafe_strcmp_usages[i].is_dstring_vs_dstring) {
                snprintf(message, sizeof(message), "Unsafe `strcmp` between two `dString_t` objects detected");
                snprintf(suggestion, sizeof(suggestion), "Replace `strcmp(%s, %s)` with `d_CompareStrings(%s, %s)`", 
                        unsafe_strcmp_usages[i].variable1, unsafe_strcmp_usages[i].variable2,
                        unsafe_strcmp_usages[i].variable1, unsafe_strcmp_usages[i].variable2);
                violation_type = "unsafe_strcmp_dstring_dstring";
            } else {
                snprintf(message, sizeof(message), "Unsafe `strcmp` usage with `dString_t->str` detected");
                snprintf(suggestion, sizeof(suggestion), "Consider using `d_CompareStrings()` or `d_CompareStringToCString()`");
            }
            
            add_violation(violations, file_path, unsafe_strcmp_usages[i].line, unsafe_strcmp_usages[i].column,
                          message, suggestion, DAEDALUS_SUGGESTION, SEVERITY_WARNING);
            issues_found++;
            
            // Deliver enhanced contextual fragment for this specific unsafe strcmp pattern
            FragmentContext_t fragment_context = {
                .variable1 = unsafe_strcmp_usages[i].variable1,
                .variable2 = unsafe_strcmp_usages[i].variable2,
                .function_name = unsafe_strcmp_usages[i].function_name,
                .file_name = file_path,
                .violation_type = violation_type
            };
            metis_deliver_contextual_fragment(&fragment_context);
        }
        free(unsafe_strcmp_usages);
    }

    // Enhanced philosophical wisdom analysis
    issues_found += check_philosophy_with_parser(parsed, violations);

    // Complexity wisdom analysis
    issues_found += check_complexity_wisdom(parsed, violations);

    // Cross-reference analysis for .c files (check against their headers)  
    if (ext && strcmp(ext, ".c") == 0) {
        issues_found += cross_reference_analyze_file(file_path, violations);
    }

    c_parser_free_parsed_file(parsed);
    return issues_found;
}

/*
 * Read file content with compassionate error handling
 */
static char* read_file_content(const char* file_path) {
    FILE* file = fopen(file_path, "r");
    if (!file) return NULL;

    // Get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (file_size <= 0) {
        fclose(file);
        // Return empty string for empty files instead of NULL
        char* empty_content = malloc(1);
        if (empty_content) {
            empty_content[0] = '\0';
        }
        return empty_content;
    }

    // Allocate buffer with divine blessing
    char* content = malloc(file_size + 1);
    if (!content) {
        fclose(file);
        return NULL;
    }

    // Read content
    size_t bytes_read = fread(content, 1, file_size, file);
    content[bytes_read] = '\0';

    fclose(file);
    return content;
}

// =============================================================================
// COLOR AND DISPLAY UTILITIES
// =============================================================================

/*
 * Get color for violation severity
 */
static const char* get_severity_color(Severity_t severity) {
    switch (severity) {
        case SEVERITY_INFO: return METIS_INFO;
        case SEVERITY_WARNING: return METIS_WARNING;
        case SEVERITY_ERROR: return METIS_ERROR;
        default: return METIS_RESET;
    }
}

/*
 * Get color for violation type
 */
static const char* get_type_color(ViolationType_t type) {
    switch (type) {
        case DOCS_VIOLATION: return METIS_INFO;
        case DAEDALUS_SUGGESTION: return METIS_SUCCESS;
        case PHILOSOPHICAL_VIOLATION: return METIS_ACCENT;
        case HEADER_VIOLATION: return METIS_WARNING;
        default: return METIS_RESET;
    }
}

/*
 * Get type name for display
 */
static const char* get_type_name(ViolationType_t type) {
    switch (type) {
        case DOCS_VIOLATION: return "Docs";
        case DAEDALUS_SUGGESTION: return "Daedalus";
        case PHILOSOPHICAL_VIOLATION: return "Philosophy";
        case HEADER_VIOLATION: return "Header";
        default: return "Unknown";
    }
}

/*
 * Read and validate file content with compassionate error handling
 */
static char* read_and_validate_file(const char* file_path) {
    printf("%s🔍 Analyzing:%s %s%s%s\n",
           METIS_INFO, METIS_RESET,
           METIS_CLICKABLE_LINK, file_path, METIS_RESET);

    char* content = read_file_content(file_path);
    if (!content) {
        printf("%s💀 Error:%s Cannot read file %s\n",
               METIS_ERROR, METIS_RESET, file_path);
    }
    return content;
}

/*
 * Analyze file content and populate violations list
 */
static int analyze_file_violations(const char* file_path, const char* content, ViolationList_t* violations) {
    if (!file_path || !content || !violations) return 0;
    
    return analyze_file_content(file_path, content, violations);
}

/*
 * Display violations in divine format
 */
static void report_violations(ViolationList_t* violations, const char* file_path) {
    if (!violations) return;
    
    int violation_count = violations->count;
    
    if (violation_count == 0) {
        printf("%s✨ Divine analysis complete:%s No issues found in %s%s%s\n",
               METIS_SUCCESS, METIS_RESET,
               METIS_CLICKABLE_LINK, file_path, METIS_RESET);
        return;
    }

    printf("%s📋 Found %d issues in %s:%s\n",
           METIS_WARNING, violation_count, file_path, METIS_RESET);

    for (int i = 0; i < violation_count; i++) {
        LintViolation_t* v = &violations->violations[i];

        printf("%s%s:%d:%d:%s %s[%s%s%s]%s %s%s%s\n",
               METIS_CLICKABLE_LINK, v->file_path, v->line_number, v->column, METIS_RESET,
               get_severity_color(v->severity),
               get_type_color(v->type), get_type_name(v->type), METIS_RESET,
               get_severity_color(v->severity), METIS_RESET,
               METIS_TEXT_SECONDARY, v->violation_message, METIS_RESET);

        if (v->suggestion) {
            printf("    %s💡 %s%s%s\n",
                   METIS_ACCENT, METIS_TEXT_MUTED, v->suggestion, METIS_RESET);
        }
    }
}

/*
 * Build context string for philosophical violations with divine precision and metrics
 */
static void build_philosophical_context(LintViolation_t* violation, char* context, size_t context_size) {
    if (!violation || !context) return;
    
    // Extract specific metrics from violation messages for precise context
    if (strstr(violation->violation_message, "TODO")) {
        snprintf(context, context_size, "TODO comment at %s:%d:%d - convert to proper issue or fix immediately", 
                 violation->file_path, violation->line_number, violation->column);
    } else if (strstr(violation->violation_message, "FIXME")) {
        snprintf(context, context_size, "FIXME comment at %s:%d:%d - broken code requiring urgent attention", 
                 violation->file_path, violation->line_number, violation->column);
    } else if (strstr(violation->violation_message, "HACK")) {
        snprintf(context, context_size, "HACK comment at %s:%d:%d - temporary solution needs proper implementation", 
                 violation->file_path, violation->line_number, violation->column);
    } else if (strstr(violation->violation_message, "XXX")) {
        snprintf(context, context_size, "XXX marker at %s:%d:%d - problematic code section flagged", 
                 violation->file_path, violation->line_number, violation->column);
    } else if (strstr(violation->violation_message, "very long")) {
        // Extract function length from message for specific guidance
        char* length_start = strstr(violation->violation_message, "(");
        char* length_end = strstr(violation->violation_message, " lines)");
        if (length_start && length_end) {
            int length = 0;
            sscanf(length_start + 1, "%d", &length);
            if (length > 100) {
                snprintf(context, context_size, "extremely long function (%d lines) at %s:%d:%d - requires major refactoring", 
                         length, violation->file_path, violation->line_number, violation->column);
            } else if (length > 75) {
                snprintf(context, context_size, "very long function (%d lines) at %s:%d:%d - consider breaking into smaller functions", 
                         length, violation->file_path, violation->line_number, violation->column);
            } else {
                snprintf(context, context_size, "long function (%d lines) at %s:%d:%d - minor refactoring recommended", 
                         length, violation->file_path, violation->line_number, violation->column);
            }
        } else {
            snprintf(context, context_size, "long function at %s:%d:%d - length exceeds recommended guidelines", 
                     violation->file_path, violation->line_number, violation->column);
        }
    } else if (strstr(violation->violation_message, "high complexity")) {
        // Extract complexity score for targeted advice
        char* score_start = strstr(violation->violation_message, "score: ");
        if (score_start) {
            int complexity = 0;
            sscanf(score_start + 7, "%d", &complexity);
            if (complexity > 20) {
                snprintf(context, context_size, "extremely complex function (score: %d) at %s:%d:%d - urgent refactoring needed", 
                         complexity, violation->file_path, violation->line_number, violation->column);
            } else if (complexity > 15) {
                snprintf(context, context_size, "highly complex function (score: %d) at %s:%d:%d - break into smaller functions", 
                         complexity, violation->file_path, violation->line_number, violation->column);
            } else {
                snprintf(context, context_size, "complex function (score: %d) at %s:%d:%d - consider simplification", 
                         complexity, violation->file_path, violation->line_number, violation->column);
            }
        } else {
            snprintf(context, context_size, "complex function at %s:%d:%d - exceeds complexity thresholds", 
                     violation->file_path, violation->line_number, violation->column);
        }
    } else if (strstr(violation->violation_message, "deep nesting")) {
        // Extract nesting depth for specific guidance
        char* depth_start = strstr(violation->violation_message, "depth: ");
        if (depth_start) {
            int depth = 0;
            sscanf(depth_start + 7, "%d", &depth);
            if (depth > 6) {
                snprintf(context, context_size, "extremely deep nesting (%d levels) at %s:%d:%d - extract nested logic immediately", 
                         depth, violation->file_path, violation->line_number, violation->column);
            } else if (depth > 4) {
                snprintf(context, context_size, "deep nesting (%d levels) at %s:%d:%d - extract into helper functions", 
                         depth, violation->file_path, violation->line_number, violation->column);
            } else {
                snprintf(context, context_size, "moderate nesting (%d levels) at %s:%d:%d - consider flattening logic", 
                         depth, violation->file_path, violation->line_number, violation->column);
            }
        } else {
            snprintf(context, context_size, "deeply nested function at %s:%d:%d - exceeds nesting recommendations", 
                     violation->file_path, violation->line_number, violation->column);
        }
    } else {
        snprintf(context, context_size, "code quality issue at %s:%d:%d - requires attention", 
                 violation->file_path, violation->line_number, violation->column);
    }
}

/*
 * Enhanced violation categorization with specific Daedalus function categories
 */
typedef struct {
    // Documentation categories
    bool has_docs;
    bool has_missing_headers;
    bool has_format_violations;
    
    // Daedalus suggestion categories  
    bool has_memory_management;     // malloc, free, realloc, calloc
    bool has_string_operations;     // strcpy, strcat, strcmp, strlen, etc.
    bool has_logging_opportunities; // printf, fprintf, sprintf family
    bool has_input_output;          // gets, fgets, scanf family
    bool has_array_operations;      // qsort, bsearch
    bool has_unsafe_strcmp_dstring; // specific strcmp with dString_t usage
    
    // Philosophy categories with metrics
    bool has_complexity_issues;     // high complexity scores
    bool has_length_issues;         // very long functions
    bool has_nesting_issues;        // deep nesting
    bool has_code_smell_comments;   // TODO, FIXME, HACK, XXX
    
    // Header violations
    bool has_purpose_line_issues;
} ViolationCategories_t;

static ViolationCategories_t categorize_violations(ViolationList_t* violations) {
    ViolationCategories_t categories = {0}; // Zero-initialize all fields
    
    for (int i = 0; i < violations->count; i++) {
        LintViolation_t* v = &violations->violations[i];
        
        switch (v->type) {
            case DOCS_VIOLATION:
                categories.has_docs = true;
                if (strstr(v->violation_message, "documentation violates one-line format")) {
                    categories.has_format_violations = true;
                }
                break;
                
            case DAEDALUS_SUGGESTION:
                // Categorize by specific function type
                if (strstr(v->violation_message, "malloc") || 
                    strstr(v->violation_message, "free") || 
                    strstr(v->violation_message, "realloc") || 
                    strstr(v->violation_message, "calloc")) {
                    categories.has_memory_management = true;
                    
                } else if (strstr(v->violation_message, "strcpy") || 
                          strstr(v->violation_message, "strcat") || 
                          strstr(v->violation_message, "strcmp") || 
                          strstr(v->violation_message, "strlen") || 
                          strstr(v->violation_message, "strdup") ||
                          strstr(v->violation_message, "strncpy") ||
                          strstr(v->violation_message, "strncat")) {
                    categories.has_string_operations = true;
                    
                } else if (strstr(v->violation_message, "printf") || 
                          strstr(v->violation_message, "fprintf") || 
                          strstr(v->violation_message, "sprintf") || 
                          strstr(v->violation_message, "snprintf") ||
                          strstr(v->violation_message, "vprintf")) {
                    categories.has_logging_opportunities = true;
                    
                } else if (strstr(v->violation_message, "gets") || 
                          strstr(v->violation_message, "fgets") || 
                          strstr(v->violation_message, "scanf") || 
                          strstr(v->violation_message, "sscanf")) {
                    categories.has_input_output = true;
                    
                } else if (strstr(v->violation_message, "qsort") || 
                          strstr(v->violation_message, "bsearch")) {
                    categories.has_array_operations = true;
                    
                } else if (strstr(v->violation_message, "Unsafe `strcmp` with `dString_t")) {
                    categories.has_unsafe_strcmp_dstring = true;
                }
                break;
                
            case PHILOSOPHICAL_VIOLATION:
                if (strstr(v->violation_message, "high complexity")) {
                    categories.has_complexity_issues = true;
                } else if (strstr(v->violation_message, "very long")) {
                    categories.has_length_issues = true;
                } else if (strstr(v->violation_message, "deep nesting")) {
                    categories.has_nesting_issues = true;
                } else if (strstr(v->violation_message, "TODO") || 
                        strstr(v->violation_message, "FIXME") || 
                        strstr(v->violation_message, "HACK") || 
                        strstr(v->violation_message, "XXX")) {
                    categories.has_code_smell_comments = true;
                }
                break;
                
            case HEADER_VIOLATION:
                categories.has_purpose_line_issues = true;
                if (strstr(v->violation_message, "missing proper filename header")) {
                    categories.has_missing_headers = true;
                }
                break;
        }
    }
    return categories;
}
/*
 * Helper function to find violation by pattern and type
 */
static LintViolation_t* find_violation_by_pattern(ViolationList_t* violations, const char* pattern, ViolationType_t type) {
    for (int i = 0; i < violations->count; i++) {
        if (violations->violations[i].type == type && 
            strstr(violations->violations[i].violation_message, pattern)) {
            return &violations->violations[i];
        }
    }
    return NULL;
}
/*
 * Deliver highly targeted fragments based on specific violation categories
 */
static void deliver_contextual_fragments(ViolationList_t* violations) {
    if (!violations || violations->count == 0) {
        metis_deliver_fragment(PHILOSOPHICAL_FRAGMENT, "perfect code achieved - divine craftsmanship", NULL, 0, 0);
        return;
    }
    
    ViolationCategories_t categories = categorize_violations(violations);
    
    // Memory Management Guidance  
    if (categories.has_memory_management) {
        LintViolation_t* violation = find_violation_by_pattern(violations, "malloc", DAEDALUS_SUGGESTION);
        if (!violation) violation = find_violation_by_pattern(violations, "free", DAEDALUS_SUGGESTION);
        if (!violation) violation = find_violation_by_pattern(violations, "realloc", DAEDALUS_SUGGESTION);
        
        char context[512] = METIS_TEXT_MUTED "Unsafe malloc detected - use " METIS_ACCENT "dArray_t"
                            METIS_TEXT_MUTED " or " METIS_ACCENT "d_StaticArray_t" METIS_TEXT_MUTED " instead" METIS_RESET;
        metis_deliver_fragment(DAEDALUS_FRAGMENT, context, 
                             violation ? violation->file_path : NULL,
                             violation ? violation->line_number : 0,
                             violation ? violation->column : 0);
    }
    
    // String Operations Guidance
    if (categories.has_string_operations) {
        LintViolation_t* violation = find_violation_by_pattern(violations, "strcmp", DAEDALUS_SUGGESTION);
        if (!violation) violation = find_violation_by_pattern(violations, "strcpy", DAEDALUS_SUGGESTION);
        if (!violation) violation = find_violation_by_pattern(violations, "strcat", DAEDALUS_SUGGESTION);
        
        char context[512];
        if (categories.has_unsafe_strcmp_dstring) {
            snprintf(context, sizeof(context), "Unsafe dString_t comparison detected - use d_CompareStrings() \nor d_CompareStringToCString()");
        } else {
            snprintf(context, sizeof(context), "Unsafe string functions detected - migrate to dString_t system");
        }
        metis_deliver_fragment(DAEDALUS_FRAGMENT, context,
                             violation ? violation->file_path : NULL,
                             violation ? violation->line_number : 0,
                             violation ? violation->column : 0);
    }
    
    // Logging System Guidance  
    if (categories.has_logging_opportunities) {
        LintViolation_t* violation = find_violation_by_pattern(violations, "printf", DAEDALUS_SUGGESTION);
        if (!violation) violation = find_violation_by_pattern(violations, "fprintf", DAEDALUS_SUGGESTION);
        
        char context[512] = "Printf family functions detected - upgrade to\n Daedalus logging system with filtering and handlers";
        metis_deliver_fragment(DAEDALUS_FRAGMENT, context,
                             violation ? violation->file_path : NULL,
                             violation ? violation->line_number : 0,
                             violation ? violation->column : 0);
    }
    
    // Input/Output Safety Guidance
    if (categories.has_input_output) {
        LintViolation_t* violation = find_violation_by_pattern(violations, "gets", DAEDALUS_SUGGESTION);
        if (!violation) violation = find_violation_by_pattern(violations, "scanf", DAEDALUS_SUGGESTION);
        
        char context[512] = "Unsafe input functions detected - use Daedalus\n string builders with validation";
        metis_deliver_fragment(DAEDALUS_FRAGMENT, context,
                             violation ? violation->file_path : NULL,
                             violation ? violation->line_number : 0,
                             violation ? violation->column : 0);
    }
    
    // Array Operations Guidance
    if (categories.has_array_operations) {
        LintViolation_t* violation = find_violation_by_pattern(violations, "qsort", DAEDALUS_SUGGESTION);
        if (!violation) violation = find_violation_by_pattern(violations, "bsearch", DAEDALUS_SUGGESTION);
        
        char context[512] = "Generic array operations detected - d_SortArray()\n and d_FindInArray() will provide type-safe alternatives";
        metis_deliver_fragment(DAEDALUS_FRAGMENT, context,
                             violation ? violation->file_path : NULL,
                             violation ? violation->line_number : 0,
                             violation ? violation->column : 0);
    }
    
    // Complexity-Specific Philosophy Guidance
    if (categories.has_complexity_issues) {
        LintViolation_t* violation = find_violation_by_pattern(violations, "high complexity", PHILOSOPHICAL_VIOLATION);
        
        char context[512] = METIS_TEXT_MUTED "High complexity functions detected - apply "
        METIS_ACCENT "single \nresponsibility principle" METIS_TEXT_MUTED " and " METIS_ACCENT 
        "extract helper functions" METIS_TEXT_MUTED "." METIS_RESET;
        metis_deliver_fragment(PHILOSOPHICAL_FRAGMENT, context,
                             violation ? violation->file_path : NULL,
                             violation ? violation->line_number : 0,
                             violation ? violation->column : 0);
    }
    
    if (categories.has_length_issues) {
        LintViolation_t* violation = find_violation_by_pattern(violations, "very long", PHILOSOPHICAL_VIOLATION);
        
        char context[512] = "Very long functions detected - break into \nfocused, testable units following Unix philosophy";
        metis_deliver_fragment(PHILOSOPHICAL_FRAGMENT, context,
                             violation ? violation->file_path : NULL,
                             violation ? violation->line_number : 0,
                             violation ? violation->column : 0);
    }
    
    if (categories.has_nesting_issues) {
        LintViolation_t* violation = find_violation_by_pattern(violations, "deep nesting", PHILOSOPHICAL_VIOLATION);
        
        char context[512] = "Deep nesting detected - use early \nreturns and guard clauses to flatten logic";
        metis_deliver_fragment(PHILOSOPHICAL_FRAGMENT, context,
                             violation ? violation->file_path : NULL,
                             violation ? violation->line_number : 0,
                             violation ? violation->column : 0);
    }
    
    if (categories.has_code_smell_comments) {
        LintViolation_t* violation = find_violation_by_pattern(violations, "TODO", PHILOSOPHICAL_VIOLATION);
        if (!violation) violation = find_violation_by_pattern(violations, "FIXME", PHILOSOPHICAL_VIOLATION);
        if (!violation) violation = find_violation_by_pattern(violations, "HACK", PHILOSOPHICAL_VIOLATION);
        
        char context[512] = "Code smell comments (TODO/FIXME/HACK) detected - convert to \nproper issues or fix immediately";
        metis_deliver_fragment(PHILOSOPHICAL_FRAGMENT, context,
                             violation ? violation->file_path : NULL,
                             violation ? violation->line_number : 0,
                             violation ? violation->column : 0);
    }
    
    // Documentation Guidance
    if (categories.has_docs) {
        LintViolation_t* violation = find_violation_by_pattern(violations, "documentation", DOCS_VIOLATION);
        
        char context[512];
        if (categories.has_format_violations) {
            snprintf(context, sizeof(context), "Documentation format violations detected - follow \none-line description, blank line, details pattern");
        } else {
            snprintf(context, sizeof(context), METIS_TEXT_MUTED "Missing function documentation detected - add "
            METIS_ACCENT "@brief" METIS_TEXT_MUTED ",\n "
            METIS_ACCENT "@param" METIS_TEXT_MUTED ", and " METIS_ACCENT "@return" METIS_TEXT_MUTED " descriptions." METIS_RESET);
        }
        metis_deliver_fragment(DOCS_FRAGMENT, context,
                             violation ? violation->file_path : NULL,
                             violation ? violation->line_number : 0,
                             violation ? violation->column : 0);
    }
    
    // Header Issues Guidance
    if (categories.has_purpose_line_issues) {
        LintViolation_t* violation = find_violation_by_pattern(violations, "header", HEADER_VIOLATION);
        if (!violation) violation = find_violation_by_pattern(violations, "filename", HEADER_VIOLATION);
        
        char context[512];
        if (categories.has_missing_headers) {
            snprintf(context, sizeof(context), "Missing file headers detected - add filename\n and purpose comments for clarity");
        } else {
            snprintf(context, sizeof(context), "File header issues detected - ensure proper\n filename and purpose documentation");
        }
        metis_deliver_fragment(LINTING_FRAGMENT, context,
                             violation ? violation->file_path : NULL,
                             violation ? violation->line_number : 0,
                             violation ? violation->column : 0);
    }
}

// =============================================================================
// PUBLIC API IMPLEMENTATION
// =============================================================================

/*
 * Initialize a linting session with divine preparation
 */
static bool initialize_linting_session(void) {
    // Force color support check and enable
    if (metis_colors_supported()) {
        metis_colors_enable(true);
    } else {
        printf("Colors not supported by terminal\n");
    }
    metis_fragment_engine_init();
    return true;
}
/*
 * Main file linting function
 */
int metis_lint_file(const char* file_path) {
    if (!file_path) return -1;

    // Initialize session
    if (!initialize_linting_session()) {
        return -1;
    }

    // Read and validate file
    char* content = read_and_validate_file(file_path);
    if (!content) {
        return -1;
    }

    // Create violation list
    ViolationList_t* violations = create_violation_list();
    if (!violations) {
        free(content);
        return -1;
    }

    // Analyze violations
    analyze_file_violations(file_path, content, violations);
    free(content);

    // Report violations
    report_violations(violations, file_path);
    
    // Deliver contextual fragments
    deliver_contextual_fragments(violations);

    // Cleanup and return result
    int violation_count = violations->count;
    free_violation_list(violations);
    return violation_count;
}

/*
 * Check if file should be analyzed (enhanced)
 */
static bool should_analyze_file(const char* file_path) {
    if (!file_path) return false;

    // Get file extension
    const char* ext = strrchr(file_path, '.');
    if (!ext) return false;

    // Check for C source files and headers
    return (strcmp(ext, ".c") == 0 || strcmp(ext, ".h") == 0 || strcmp(ext, ".cpp") == 0);
}

/*
 * Recursively lint directory with divine organization
 */
int metis_lint_directory(const char* dir_path) {
    if (!dir_path) return -1;

    DIR* dir = opendir(dir_path);
    if (!dir) {
        printf("%s💀 Error:%s Cannot open directory %s\n",
               METIS_ERROR, METIS_RESET, dir_path);
        return -1;
    }

    // Initialize fragment engine and reset session for directory scan
    metis_fragment_engine_init();
    metis_reset_session_fragments();

    int total_violations = 0;
    int files_analyzed = 0;
    struct dirent* entry;

    printf("%s🏛️ Analyzing directory:%s %s%s%s\n", // Replaced \u{1f3db}\ufe0f with 🏛️
           METIS_INFO, METIS_RESET,
           METIS_CLICKABLE_LINK, dir_path, METIS_RESET);

    while ((entry = readdir(dir)) != NULL) {
        // Skip . and ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Build full path
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);

        struct stat path_stat;
        if (stat(full_path, &path_stat) != 0) {
            continue;
        }

        if (S_ISDIR(path_stat.st_mode)) {
            // Recursively process subdirectory
            int subdir_violations = metis_lint_directory(full_path);
            if (subdir_violations > 0) {
                total_violations += subdir_violations;
            }
        } else if (should_analyze_file(full_path)) {
            // Analyze file
            int file_violations = metis_lint_file(full_path);
            files_analyzed++;
            if (file_violations > 0) {
                total_violations += file_violations;
            }
        }
    }

    closedir(dir);

    // Summary for directory
    if (files_analyzed > 0) {
        printf("\n%s📊 Directory summary:%s %d files analyzed, %d total issues\n", // Replaced \u{1f4ca} with 📊
               METIS_INFO, METIS_RESET, files_analyzed, total_violations);
    }

    return total_violations;
}

/*
 * Check implementation documentation consistency with headers
 */
static int check_implementation_doc_consistency(ParsedFile_t* parsed, ViolationList_t* violations, ParsedFile_t* header_parsed) {
    if (!parsed || !violations || !header_parsed) return 0;

    int issues_found = 0;
    const char* file_path = parsed->file_path;

    // Only check .c files for implementation consistency
    const char* ext = strrchr(file_path, '.');
    if (!ext || strcmp(ext, ".c") != 0) {
        return 0;
    }

    // Check each function against corresponding header
    for (int i = 0; i < parsed->function_count; i++) {
        FunctionInfo_t* func = &parsed->functions[i];

        // Skip static functions (they don't need to match headers)
        if (func->is_static) continue;

        // Get expected description from header
        char* expected_desc = c_parser_extract_function_description(header_parsed, func->name);
        if (expected_desc) {
            if (!c_parser_implementation_matches_header(parsed, func->name, expected_desc)) {
                char message[256];
                snprintf(message, sizeof(message),
                        "Function '%s' implementation comment doesn't match header description",
                        func->name);

                char suggestion[512];
                snprintf(suggestion, sizeof(suggestion),
                        "Add: /* %s */", expected_desc);

                add_violation(violations, file_path, func->line_number, func->column,
                            message, suggestion, DOCS_VIOLATION, SEVERITY_WARNING);
                issues_found++;
            }
            free(expected_desc);
        }
    }

    return issues_found;
}

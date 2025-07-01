/* metis_linter.c - Core linting engine for divine documentation wisdom */
// INSERT WISDOM HERE

#define _POSIX_C_SOURCE 200809L  // For strdup

#include "metis_linter.h"
#include "metis_config.h"
#include "fragment_engine.h"
#include "metis_colors.h"
#include "c_parser.h"
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
typedef struct {
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
                "File missing proper filename header comment on first line");

        char suggestion[512];
        snprintf(suggestion, sizeof(suggestion),
                "Add: /* %s - brief description */", filename);

        add_violation(violations, file_path, 1, 1, message, suggestion,
                     HEADER_VIOLATION, SEVERITY_WARNING);
        issues_found++;
    }

    // Check for proper wisdom header (second line)
    if (!c_parser_has_proper_wisdom_header(parsed)) {
        char message[256];
        snprintf(message, sizeof(message),
                "File missing proper wisdom fragment comment on second line");

        add_violation(violations, file_path, 2, 1, message,
                     "Add: // INSERT WISDOM HERE (or a proper Metis fragment)",
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

                // Provide specific Daedalus suggestions
                if (strcmp(token->value, "strcpy") == 0) {
                    suggestion = "Use d_StringCopy() for bounds-checked copying";
                } else if (strcmp(token->value, "strcat") == 0) {
                    suggestion = "Use d_StringAppend() for safe concatenation";
                } else if (strcmp(token->value, "sprintf") == 0) {
                    suggestion = "Use d_StringFormat() for buffer-safe formatting";
                } else if (strcmp(token->value, "malloc") == 0 || strcmp(token->value, "realloc") == 0) {
                    suggestion = "Use d_Array for automatic growth and bounds checking";
                } else if (strcmp(token->value, "gets") == 0) {
                    suggestion = "Use d_StringInput() for safe input reading";
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

    // Enhanced philosophical wisdom analysis
    issues_found += check_philosophy_with_parser(parsed, violations);

    // Complexity wisdom analysis
    issues_found += check_complexity_wisdom(parsed, violations);

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

// =============================================================================
// PUBLIC API IMPLEMENTATION
// =============================================================================

/*
 * Main file linting function with divine visual harmony
 */
int metis_lint_file(const char* file_path) {
    if (!file_path) return -1;

    // Initialize colors and fragment engine
    metis_colors_enable(true);
    metis_fragment_engine_init();

    printf("%s🔍 Analyzing:%s %s%s%s\n",
           METIS_INFO, METIS_RESET,
           METIS_CLICKABLE_LINK, file_path, METIS_RESET);

    // Read file content
    char* content = read_file_content(file_path);
    if (!content) {
        printf("%s💀 Error:%s Cannot read file %s\n",
               METIS_ERROR, METIS_RESET, file_path);
        return -1;
    }

    // Create violation list
    ViolationList_t* violations = create_violation_list();
    if (!violations) {
        free(content);
        return -1;
    }

    // Analyze content
    analyze_file_content(file_path, content, violations);
    free(content);

    // Report results with divine beauty
    int violation_count = violations->count;

    if (violation_count == 0) {
        printf("%s✨ Divine analysis complete:%s No issues found in %s%s%s\n",
               METIS_SUCCESS, METIS_RESET,
               METIS_CLICKABLE_LINK, file_path, METIS_RESET);

        // Deliver philosophical fragment for perfect code
        metis_deliver_fragment(PHILOSOPHICAL_FRAGMENT, "perfect code achieved");
    } else {
        printf("%s📋 Found %d issues in %s:%s\n",
               METIS_WARNING, violation_count, file_path, METIS_RESET);

        for (int i = 0; i < violation_count; i++) {
            LintViolation_t* v = &violations->violations[i];

            // Divine clickable link format: file:line:column
            printf("%s%s:%d:%d:%s %s[%s%s%s]%s %s%s%s\n",
                   METIS_CLICKABLE_LINK, v->file_path, v->line_number, v->column, METIS_RESET,
                   get_severity_color(v->severity),
                   get_type_color(v->type),
                   get_type_name(v->type),
                   METIS_RESET, get_severity_color(v->severity), METIS_RESET,
                   METIS_TEXT_SECONDARY, v->violation_message, METIS_RESET);

            if (v->suggestion) {
                printf("    %s💡 %s%s%s\n",
                       METIS_ACCENT, METIS_TEXT_MUTED, v->suggestion, METIS_RESET);
            }
        }

        // Deliver fragment based on violation types found
        bool has_docs = false, has_daedalus = false, has_philosophical = false, has_linting = false;
        
        for (int i = 0; i < violation_count; i++) {
            LintViolation_t* v = &violations->violations[i];
            switch (v->type) {
                case DOCS_VIOLATION:
                    has_docs = true;
                    break;
                case DAEDALUS_SUGGESTION:
                    has_daedalus = true;
                    break;
                case PHILOSOPHICAL_VIOLATION:
                    has_philosophical = true;
                    break;
                case HEADER_VIOLATION:
                    has_linting = true;
                    break;
            }
        }
        
        // Deliver fragments based on violation types (session limits apply)
        if (has_docs) {
            // Find first docs violation for location context
            char context[512] = "undocumented functions detected";
            for (int i = 0; i < violation_count; i++) {
                LintViolation_t* v = &violations->violations[i];
                if (v->type == DOCS_VIOLATION) {
                    snprintf(context, sizeof(context), "undocumented functions detected at %s:%d:%d", 
                             v->file_path, v->line_number, v->column);
                    break;
                }
            }
            metis_deliver_fragment(DOCS_FRAGMENT, context);
        }
        if (has_daedalus) {
            // Find first Daedalus violation for context
            char context[512] = "unsafe function detected";
            for (int i = 0; i < violation_count; i++) {
                LintViolation_t* v = &violations->violations[i];
                if (v->type == DAEDALUS_SUGGESTION) {
                    snprintf(context, sizeof(context), "%s at %s:%d:%d", 
                             v->violation_message, v->file_path, v->line_number, v->column);
                    break;
                }
            }
            metis_deliver_fragment(DAEDALUS_FRAGMENT, context);
        }
        if (has_philosophical) {
            // Find first philosophical violation for location context
            char context[512] = "TODO/FIXME comments found";
            for (int i = 0; i < violation_count; i++) {
                LintViolation_t* v = &violations->violations[i];
                if (v->type == PHILOSOPHICAL_VIOLATION) {
                    snprintf(context, sizeof(context), "TODO/FIXME comments found at %s:%d:%d", 
                             v->file_path, v->line_number, v->column);
                    break;
                }
            }
            metis_deliver_fragment(PHILOSOPHICAL_FRAGMENT, context);
        }
        if (has_linting) {
            // Generate specific context based on header violation type
            char context[512] = "header violations detected";
            for (int i = 0; i < violation_count; i++) {
                LintViolation_t* v = &violations->violations[i];
                if (v->type == HEADER_VIOLATION) {
                    // Provide specific context based on violation type
                    if (strstr(v->violation_message, "missing proper filename header")) {
                        snprintf(context, sizeof(context), 
                                "missing filename header at %s:%d:%d - Add: /* filename.c - description */", 
                                v->file_path, v->line_number, v->column);
                    } else if (strstr(v->violation_message, "missing proper wisdom fragment")) {
                        snprintf(context, sizeof(context), 
                                "missing wisdom header at %s:%d:%d - Add: // INSERT WISDOM HERE", 
                                v->file_path, v->line_number, v->column);
                    } else if (strstr(v->violation_message, "documentation violates one-line format")) {
                        snprintf(context, sizeof(context), 
                                "improper documentation format at %s:%d:%d - Use: description, blank line, details", 
                                v->file_path, v->line_number, v->column);
                    } else {
                        snprintf(context, sizeof(context), "header formatting issues at %s:%d:%d", 
                                v->file_path, v->line_number, v->column);
                    }
                    break;
                }
            }
            metis_deliver_fragment(LINTING_FRAGMENT, context);
        }
    }

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

    printf("%s🏛️ Analyzing directory:%s %s%s%s\n",
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
        printf("\n%s📊 Directory summary:%s %d files analyzed, %d total issues\n",
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

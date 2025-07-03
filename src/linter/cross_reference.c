/* cross_reference.c - Cross-reference validation between headers and implementations */
// INSERT WISDOM HERE

#define _GNU_SOURCE  // For strdup
#include "cross_reference.h"
#include "metis_linter.h"
#include "metis_colors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <libgen.h>
#include <ctype.h>

// Define types before forward declarations
typedef struct {
    const char* tag;
    const char* type_color;
    const char* severity_color;
} XRefViolationMetadata_t;

// Forward declarations for helper functions
static void _process_char_for_whitespace_compression(char current_char, char** write_ptr, bool* seen_non_space_ptr);
static XRefViolationMetadata_t _get_violation_metadata(XRefViolationType_t type);
static void _print_formatted_violation(const char* file_path, int line,
                                       const XRefViolationMetadata_t* metadata,
                                       const char* description);
static bool _xref_parse_files(const char* c_file_path, char* header_path, ParsedFile_t** impl_parsed_out, ParsedFile_t** header_parsed_out);
static void _xref_check_impl_functions(ParsedFile_t* impl_parsed, ParsedFile_t* header_parsed, const char* header_path, XRefViolationList_t* xref_violations);
static void _xref_check_header_functions(ParsedFile_t* impl_parsed, ParsedFile_t* header_parsed, const char* c_file_path, XRefViolationList_t* xref_violations);
static void _xref_free_analysis_resources(XRefViolationList_t* xref_violations, ParsedFile_t* impl_parsed, ParsedFile_t* header_parsed, char* header_path);

static FunctionInfo_t* _find_function_in_parsed_file(const ParsedFile_t* parsed_file, const char* function_name);

/*
 * Helper function to check if a file exists
 */
static bool file_exists(const char* path) {
    if (!path) return false;
    struct stat st;
    return stat(path, &st) == 0;
}

/*
 * Helper function to extract filename without extension
 */
static char* extract_basename_no_ext(const char* path) {
    if (!path) return NULL;
    
    char* path_copy = strdup(path);
    if (!path_copy) return NULL;
    
    char* filename = basename(path_copy);
    char* dot = strrchr(filename, '.');
    if (dot) *dot = '\0';
    
    char* result = strdup(filename);
    free(path_copy);
    return result;
}

/*
 * Find the corresponding header file for a given .c file
 */
char* cross_reference_find_header_file(const char* c_file_path) {
    if (!c_file_path) return NULL;
    
    // Extract base name without extension
    char* basename_no_ext = extract_basename_no_ext(c_file_path);
    if (!basename_no_ext) return NULL;
    
    // Extract directory path for relative searches
    char* c_file_copy = strdup(c_file_path);
    char* c_dir = dirname(c_file_copy);
    
    // Try different locations for the header file
    const char* search_patterns[] = {
        "include/%s.h",           // include/filename.h
        "include/linter/%s.h",    // include/linter/filename.h  
        "include/config/%s.h",    // include/config/filename.h
        "include/wisdom/%s.h",    // include/wisdom/filename.h
        "include/cli/%s.h",       // include/cli/filename.h
        "%s.h",                   // filename.h (same directory as .c)
        NULL
    };
    
    // Try both current working directory and relative to c file directory
    const char* base_dirs[] = { ".", c_dir, NULL };
    
    for (int base_idx = 0; base_dirs[base_idx] != NULL; base_idx++) {
        for (int i = 0; search_patterns[i] != NULL; i++) {
            // Calculate size needed for path
            int path_size = strlen(base_dirs[base_idx]) + 1 + strlen(search_patterns[i]) + strlen(basename_no_ext) + 1;
            char* header_path = malloc(path_size);
            if (!header_path) continue;
            
            if (strcmp(base_dirs[base_idx], ".") == 0) {
                // Current directory - use pattern directly
                sprintf(header_path, search_patterns[i], basename_no_ext);
            } else {
                // Relative to c file directory
                sprintf(header_path, "%s/%s", base_dirs[base_idx], search_patterns[i]);
                char* formatted_path = malloc(strlen(header_path) + strlen(basename_no_ext) + 1);
                if (formatted_path) {
                    sprintf(formatted_path, header_path, basename_no_ext);
                    free(header_path);
                    header_path = formatted_path;
                }
            }
            
            if (file_exists(header_path)) {
                free(basename_no_ext);
                free(c_file_copy);
                return header_path;
            }
            
            free(header_path);
        }
    }
    
    free(basename_no_ext);
    free(c_file_copy);
    return NULL;
}

/*
 * Find the corresponding implementation file for a given .h file
 */
char* cross_reference_find_impl_file(const char* h_file_path) {
    if (!h_file_path) return NULL;
    
    // Extract base name without extension
    char* basename_no_ext = extract_basename_no_ext(h_file_path);
    if (!basename_no_ext) return NULL;
    
    // Extract directory path for relative searches
    char* h_file_copy = strdup(h_file_path);
    char* h_dir = dirname(h_file_copy);
    
    // Try different locations for the implementation file
    const char* search_patterns[] = {
        "src/%s.c",               // src/filename.c
        "src/linter/%s.c",        // src/linter/filename.c
        "src/config/%s.c",        // src/config/filename.c
        "src/wisdom/%s.c",        // src/wisdom/filename.c
        "src/cli/%s.c",           // src/cli/filename.c
        "%s.c",                   // filename.c (same directory as .h)
        NULL
    };
    
    // Try both current working directory and relative to h file directory
    const char* base_dirs[] = { ".", h_dir, NULL };
    
    for (int base_idx = 0; base_dirs[base_idx] != NULL; base_idx++) {
        for (int i = 0; search_patterns[i] != NULL; i++) {
            // Calculate size needed for path
            int path_size = strlen(base_dirs[base_idx]) + 1 + strlen(search_patterns[i]) + strlen(basename_no_ext) + 1;
            char* impl_path = malloc(path_size);
            if (!impl_path) continue;
            
            if (strcmp(base_dirs[base_idx], ".") == 0) {
                // Current directory - use pattern directly
                sprintf(impl_path, search_patterns[i], basename_no_ext);
            } else {
                // Relative to h file directory
                sprintf(impl_path, "%s/%s", base_dirs[base_idx], search_patterns[i]);
                char* formatted_path = malloc(strlen(impl_path) + strlen(basename_no_ext) + 1);
                if (formatted_path) {
                    sprintf(formatted_path, impl_path, basename_no_ext);
                    free(impl_path);
                    impl_path = formatted_path;
                }
            }
            
            if (file_exists(impl_path)) {
                free(basename_no_ext);
                free(h_file_copy);
                return impl_path;
            }
            
            free(impl_path);
        }
    }
    
    free(basename_no_ext);
    free(h_file_copy);
    return NULL;
}
/*
 * Helper function to compress multiple spaces into a single space, in-place.
 * Also trims leading and internal multiple spaces.
 */
static void _compress_whitespace(char* str) {
    if (!str) return;

    char* read_ptr = str;
    char* write_ptr = str;
    bool seen_non_space = false; // To handle leading spaces

    // Skip leading spaces
    while (*read_ptr != '\0' && isspace((unsigned char)*read_ptr)) {
        read_ptr++;
    }

    while (*read_ptr != '\0') {
        _process_char_for_whitespace_compression(*read_ptr, &write_ptr, &seen_non_space);
        read_ptr++;
    }
    *write_ptr = '\0'; // Null-terminate the new string

    // Trim trailing space if necessary (if the last non-space was followed by a space)
    if (write_ptr > str && *(write_ptr - 1) == ' ') {
        *(write_ptr - 1) = '\0';
    }
}

/*
 * Helper function to process a single character for whitespace compression.
 * Updates the write pointer and seen_non_space flag.
 */
static void _process_char_for_whitespace_compression(char current_char, char** write_ptr_ptr, bool* seen_non_space_ptr) {
    char* write_ptr = *write_ptr_ptr; // Dereference to get the actual pointer
    bool seen_non_space = *seen_non_space_ptr; // Dereference to get the actual value

    if (isspace((unsigned char)current_char)) {
        // If we've seen a non-space character, and we haven't already written a space
        // at the current write_ptr position (to avoid double-spacing if this is the
        // second character after a block of spaces), then write a space.
        if (seen_non_space && (write_ptr == *write_ptr_ptr || *(write_ptr - 1) != ' ')) {
            *write_ptr_ptr = write_ptr + 1; // Update the pointer that write_ptr_ptr points to
            *write_ptr = ' ';
        }
    } else {
        // It's a non-space character
        if (!seen_non_space) { // First non-space character encountered
            *seen_non_space_ptr = true; // Update the flag via its pointer
        }
        *write_ptr_ptr = write_ptr + 1; // Update the pointer
        *write_ptr = current_char; // Write the character
    }
}

// normalize_type_string remains unchanged from the previous suggestion
static char* normalize_type_string(const char* type_str) {
    if (!type_str) {
        return NULL;
    }

    char* normalized = strdup(type_str);
    if (!normalized) {
        return NULL;
    }

    _compress_whitespace(normalized);

    // This trailing trim might be redundant now, but keep for robustness
    int len = strlen(normalized);
    if (len > 0 && normalized[len - 1] == ' ') {
        normalized[len - 1] = '\0';
    }

    return normalized;
}
/*
 * Compare function signatures between header and implementation with smart matching
 */
bool cross_reference_compare_signatures(const FunctionInfo_t* header_func, const FunctionInfo_t* impl_func) {
    if (!header_func || !impl_func) return false;
    
    // Compare function names - must be exact
    if (strcmp(header_func->name, impl_func->name) != 0) {
        return false;
    }
    
    // Compare return types with normalization
    char* header_ret = normalize_type_string(header_func->return_type);
    char* impl_ret = normalize_type_string(impl_func->return_type);
    
    bool ret_match = true;
    if (header_ret && impl_ret) {
        ret_match = (strcmp(header_ret, impl_ret) == 0);
    } else {
        ret_match = (header_ret == impl_ret); // Both NULL
    }
    
    free(header_ret);
    free(impl_ret);
    
    if (!ret_match) {
        return false;
    }
    
    // Compare parameter count - must be exact
    if (header_func->param_count != impl_func->param_count) {
        return false;
    }
    
    // For now, if parameter counts match, consider signatures compatible
    // This is because parameter extraction can vary between declaration and definition
    // The important thing is that the function exists with the right name and return type
    return true;
}

/*
 * Compare documentation between header and implementation
 */
bool cross_reference_compare_documentation(const FunctionInfo_t* header_func, const FunctionInfo_t* impl_func) {
    if (!header_func || !impl_func) return false;
    
    // If header has no documentation, that's a separate issue
    if (!header_func->has_documentation) {
        return true; // Not a cross-reference documentation issue
    }
    
    // If implementation has no documentation, that's okay if header has it
    if (!impl_func->has_documentation) {
        return true; // Header documentation is sufficient
    }
    
    // Both have documentation - check for major inconsistencies
    if (header_func->documentation && impl_func->documentation) {
        // For now, just check they both exist
        // Could be expanded to check parameter descriptions, etc.
        return true;
    }
    
    return true;
}

/*
 * Check if a function should be cross-referenced
 */
bool cross_reference_should_check_function(const FunctionInfo_t* func) {
    if (!func || !func->name) return false;
    
    // Skip static functions (internal to .c file)
    if (func->is_static) return false;
    
    // Skip main function
    if (strcmp(func->name, "main") == 0) return false;
    
    // Skip test functions
    if (strncmp(func->name, "test_", 5) == 0) return false;
    
    // Skip debug functions
    if (strncmp(func->name, "debug_", 6) == 0) return false;
    
    return true;
}

/*
 * Initialize cross-reference violation list
 */
XRefViolationList_t* cross_reference_init_violations(void) {
    XRefViolationList_t* violations = malloc(sizeof(XRefViolationList_t));
    if (!violations) return NULL;
    
    violations->violations = NULL;
    violations->violation_count = 0;
    violations->violation_capacity = 0;
    
    return violations;
}

/*
 * Add a cross-reference violation to the list
 */
void cross_reference_add_violation(XRefViolationList_t* violations, 
                                 const char* func_name,
                                 XRefViolationType_t type,
                                 const char* description,
                                 int header_line, 
                                 int impl_line) {
    if (!violations || !func_name || !description) return;
    
    // Expand array if needed
    if (violations->violation_count >= violations->violation_capacity) {
        int new_capacity = violations->violation_capacity == 0 ? 10 : violations->violation_capacity * 2;
        XRefViolation_t* new_violations = realloc(violations->violations, 
                                                 new_capacity * sizeof(XRefViolation_t));
        if (!new_violations) return;
        
        violations->violations = new_violations;
        violations->violation_capacity = new_capacity;
    }
    
    // Add the violation
    XRefViolation_t* violation = &violations->violations[violations->violation_count];
    violation->function_name = strdup(func_name);
    violation->violation_type = type;
    violation->description = strdup(description);
    violation->header_signature = NULL;
    violation->impl_signature = NULL;
    violation->header_line = header_line;
    violation->impl_line = impl_line;
    
    violations->violation_count++;
}

/*
 * Clean up cross-reference violation list
 */
void cross_reference_free_violations(XRefViolationList_t* violations) {
    if (!violations) return;
    
    for (int i = 0; i < violations->violation_count; i++) {
        free(violations->violations[i].function_name);
        free(violations->violations[i].description);
        free(violations->violations[i].header_signature);
        free(violations->violations[i].impl_signature);
    }
    
    free(violations->violations);
    free(violations);
}

/*
 * Helper function to determine metadata (tag, colors) for a given cross-reference violation type.
 *
 * `type` - The type of cross-reference violation.
 *
 * `XRefViolationMetadata_t` - A struct containing the appropriate tag and color codes.
 */
static XRefViolationMetadata_t _get_violation_metadata(XRefViolationType_t type) {
    XRefViolationMetadata_t metadata;

    switch (type) {
        case XREF_MISSING_DECLARATION:
        case XREF_MISSING_IMPLEMENTATION:
        case XREF_SIGNATURE_MISMATCH:
            metadata.tag = "Header";
            metadata.type_color = METIS_WARNING;
            metadata.severity_color = METIS_WARNING;
            break;
        case XREF_DOC_INCONSISTENCY:
            metadata.tag = "Docs";
            metadata.type_color = METIS_INFO;
            metadata.severity_color = METIS_INFO;
            break;
        case XREF_PARAMETER_MISMATCH: // Added based on cross_reference.h enum
        case XREF_RETURN_TYPE_MISMATCH: // Added based on cross_reference.h enum
        default:
            // Fallback for any unhandled or new types
            metadata.tag = "Header";
            metadata.type_color = METIS_WARNING;
            metadata.severity_color = METIS_WARNING;
            break;
    }
    return metadata;
}

/*
 * Helper function to print a cross-reference violation using Metis color system.
 *
 * `file_path` - Path to the file where the violation occurred.
 * `line` - Line number of the violation.
 * `metadata` - Pointer to XRefViolationMetadata_t containing tag and colors.
 * `description` - Human-readable description of the violation.
 */
static void _print_formatted_violation(const char* file_path, int line,
                                       const XRefViolationMetadata_t* metadata,
                                       const char* description) {
    // Use the proper Metis color format matching the main linter output
    printf("%s%s:%d:%d:%s %s[%s%s%s]%s %s%s%s\n",
           METIS_CLICKABLE_LINK, file_path, line, 1, METIS_RESET,
           metadata->severity_color,
           metadata->type_color,
           metadata->tag,
           METIS_RESET, metadata->severity_color,
           METIS_TEXT_SECONDARY, description, METIS_RESET);
}

/*
 * Convert cross-reference violations to regular linter violations using Metis color system
 */
int cross_reference_convert_violations(const XRefViolationList_t* xref_violations,
                                     const char* file_path,
                                     ViolationList_t* violations) {
    if (!xref_violations || !file_path) return 0;
    
    int converted = 0;
    
    for (int i = 0; i < xref_violations->violation_count; i++) {
        const XRefViolation_t* xref = &xref_violations->violations[i];
        
        XRefViolationMetadata_t metadata = _get_violation_metadata(xref->violation_type);
        
        // Use implementation line if available, otherwise header line
        int line = xref->impl_line > 0 ? xref->impl_line : xref->header_line;
        line = line > 0 ? line : 1; // Ensure line is at least 1
        
        _print_formatted_violation(file_path, line, &metadata, xref->description);
        
        converted++;
    }
    
    return converted;
}


/*
 * Helper function to find a function by name in a ParsedFile_t structure.
 *
 * `parsed_file` - The parsed file structure to search within.
 * `function_name` - The name of the function to find.
 *
 * `FunctionInfo_t*` - Pointer to the found FunctionInfo_t, or NULL if not found.
 */
static FunctionInfo_t* _find_function_in_parsed_file(const ParsedFile_t* parsed_file, const char* function_name) {
    if (!parsed_file || !function_name) {
        return NULL;
    }
    for (int i = 0; i < parsed_file->function_count; i++) {
        if (strcmp(parsed_file->functions[i].name, function_name) == 0) {
            return &parsed_file->functions[i];
        }
    }
    return NULL;
}


/*
 * Helper function to parse the C implementation and header files.
 *
 * `c_file_path` - Path to the .c file.
 * `header_path` - Path to the .h file.
 * `impl_parsed_out` - Output parameter for the parsed .c file.
 * `header_parsed_out` - Output parameter for the parsed .h file.
 *
 * `bool` - true if both files were successfully parsed, false otherwise.
 * Frees any partially allocated resources on failure.
 */
static bool _xref_parse_files(const char* c_file_path, char* header_path,
                              ParsedFile_t** impl_parsed_out, ParsedFile_t** header_parsed_out) {
    *impl_parsed_out = c_parser_parse_file(c_file_path);
    *header_parsed_out = c_parser_parse_file(header_path);

    if (!*impl_parsed_out || !*header_parsed_out) {
        if (*impl_parsed_out) c_parser_free_parsed_file(*impl_parsed_out);
        if (*header_parsed_out) c_parser_free_parsed_file(*header_parsed_out);
        *impl_parsed_out = NULL;
        *header_parsed_out = NULL;
        return false;
    }
    return true;
}

/*
 * Helper function to check functions implemented in .c against their header declarations.
 * Adds violations for missing declarations, signature mismatches, and documentation inconsistencies.
 *
 * `impl_parsed` - Parsed information for the .c implementation file.
 * `header_parsed` - Parsed information for the .h header file.
 * `header_path` - Path to the header file (for violation descriptions).
 * `xref_violations` - The list to add cross-reference violations to.
 */
static void _xref_check_impl_functions(ParsedFile_t* impl_parsed, ParsedFile_t* header_parsed,
                                       const char* header_path, XRefViolationList_t* xref_violations) {
    for (int i = 0; i < impl_parsed->function_count; i++) {
        FunctionInfo_t* impl_func = &impl_parsed->functions[i];
        
        if (!cross_reference_should_check_function(impl_func)) {
            continue;
        }
        
        FunctionInfo_t* header_func = _find_function_in_parsed_file(header_parsed, impl_func->name);
        
        if (!header_func) {
            // Function implemented but not declared in header
            char description[256];
            snprintf(description, sizeof(description),
                    "Function '%s' is implemented but not declared in header '%s'",
                    impl_func->name, header_path);
            
            cross_reference_add_violation(xref_violations, impl_func->name,
                                        XREF_MISSING_DECLARATION, description,
                                        0, impl_func->line_number);
        } else {
            // Compare signatures
            if (!cross_reference_compare_signatures(header_func, impl_func)) {
                char description[256];
                snprintf(description, sizeof(description),
                        "Function '%s' signature mismatch between header and implementation",
                        impl_func->name);
                
                cross_reference_add_violation(xref_violations, impl_func->name,
                                            XREF_SIGNATURE_MISMATCH, description,
                                            header_func->line_number, impl_func->line_number);
            }
            
            
            
            // Compare documentation
            if (!cross_reference_compare_documentation(header_func, impl_func)) {
                char description[256];
                snprintf(description, sizeof(description),
                        "Function '%s' documentation inconsistency between header and implementation",
                        impl_func->name);
                
                cross_reference_add_violation(xref_violations, impl_func->name,
                                            XREF_DOC_INCONSISTENCY, description,
                                            header_func->line_number, impl_func->line_number);
            }
        }
    }
}

/*
 * Helper function to check functions declared in the header against their .c implementations.
 * Adds violations for missing implementations.
 *
 * `impl_parsed` - Parsed information for the .c implementation file.
 * `header_parsed` - Parsed information for the .h header file.
 * `c_file_path` - Path to the .c file (for violation descriptions).
 * `xref_violations` - The list to add cross-reference violations to.
 */
static void _xref_check_header_functions(ParsedFile_t* impl_parsed, ParsedFile_t* header_parsed,
                                        const char* c_file_path, XRefViolationList_t* xref_violations) {
    for (int i = 0; i < header_parsed->function_count; i++) {
        FunctionInfo_t* header_func = &header_parsed->functions[i];
        
        if (!cross_reference_should_check_function(header_func)) {
            continue;
        }
        
        FunctionInfo_t* impl_func = _find_function_in_parsed_file(impl_parsed, header_func->name);
        
        // Check for missing documentation in header (regardless of implementation)
        if (!header_func->has_documentation) {
            char description[256];
            snprintf(description, sizeof(description),
                    "Function '%s' is declared in header but lacks proper documentation",
                    header_func->name);
            
            cross_reference_add_violation(xref_violations, header_func->name,
                                        XREF_DOC_INCONSISTENCY, description,
                                        header_func->line_number, 0);
        }
        
        if (!impl_func) {
            char description[256];
            snprintf(description, sizeof(description),
                    "Function '%s' is declared in header but not implemented in '%s'",
                    header_func->name, c_file_path);
            
            cross_reference_add_violation(xref_violations, header_func->name,
                                        XREF_MISSING_IMPLEMENTATION, description,
                                        header_func->line_number, 0);
        }
    }
}

/*
 * Helper function to free all resources allocated during cross-reference analysis.
 *
 * `xref_violations` - The list of cross-reference violations.
 * `impl_parsed` - Parsed information for the .c implementation file.
 * `header_parsed` - Parsed information for the .h header file.
 * `header_path` - Dynamically allocated path to the header file.
 */
static void _xref_free_analysis_resources(XRefViolationList_t* xref_violations,
                                         ParsedFile_t* impl_parsed,
                                         ParsedFile_t* header_parsed,
                                         char* header_path) {
    if (xref_violations) cross_reference_free_violations(xref_violations);
    if (impl_parsed) c_parser_free_parsed_file(impl_parsed);
    if (header_parsed) c_parser_free_parsed_file(header_parsed);
    if (header_path) free(header_path);
}
/*
 * Perform full cross-reference analysis between a .c file and its header
 */
int cross_reference_analyze_file(const char* c_file_path, ViolationList_t* violations) {
    if (!c_file_path) return 0;
    
    char* header_path = NULL;
    ParsedFile_t* impl_parsed = NULL;
    ParsedFile_t* header_parsed = NULL;
    XRefViolationList_t* xref_violations = NULL;
    int violation_count = 0;

    // 1. Find the corresponding header file
    header_path = cross_reference_find_header_file(c_file_path);
    if (!header_path) {
        return 0; // No header file found - not an error for cross-reference
    }

    // 2. Parse both files
    if (!_xref_parse_files(c_file_path, header_path, &impl_parsed, &header_parsed)) {
        _xref_free_analysis_resources(NULL, NULL, NULL, header_path); // Only free header_path here
        return 0;
    }
    
    // 3. Initialize cross-reference violations list
    xref_violations = cross_reference_init_violations();
    if (!xref_violations) {
        _xref_free_analysis_resources(NULL, impl_parsed, header_parsed, header_path);
        return 0;
    }
    
    // 4. Check functions implemented in .c against header declarations
    _xref_check_impl_functions(impl_parsed, header_parsed, header_path, xref_violations);
    
    // 5. Check functions declared in header against .c implementations
    _xref_check_header_functions(impl_parsed, header_parsed, c_file_path, xref_violations);
    
    // 6. Convert cross-reference violations to regular linter violations
    violation_count = cross_reference_convert_violations(xref_violations, c_file_path, violations);
    
    // 7. Cleanup all allocated resources
    _xref_free_analysis_resources(xref_violations, impl_parsed, header_parsed, header_path);
    
    return violation_count;
}
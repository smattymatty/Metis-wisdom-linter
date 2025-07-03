
/* cross_reference.h - Cross-reference validation between headers and implementations */
// INSERT WISDOM HERE

#ifndef CROSS_REFERENCE_H
#define CROSS_REFERENCE_H

#include "c_parser.h"

// Forward declaration to avoid circular includes
// ViolationList_t is defined in metis_linter.c
typedef struct ViolationList ViolationList_t;

// Cross-reference violation types
typedef enum {
    XREF_SIGNATURE_MISMATCH,
    XREF_MISSING_DECLARATION,
    XREF_MISSING_IMPLEMENTATION,
    XREF_DOC_INCONSISTENCY,
    XREF_PARAMETER_MISMATCH,
    XREF_RETURN_TYPE_MISMATCH
} XRefViolationType_t;

// Structure to hold cross-reference analysis results
typedef struct {
    char* function_name;
    XRefViolationType_t violation_type;
    char* header_signature;
    char* impl_signature;
    char* description;
    int header_line;
    int impl_line;
} XRefViolation_t;

typedef struct {
    XRefViolation_t* violations;
    int violation_count;
    int violation_capacity;
} XRefViolationList_t;

/*
 * Find the corresponding header file for a given .c file
 *
 * `c_file_path` - Path to the .c file
 *
 * `char*` - Path to corresponding .h file, or NULL if not found
 *
 * -- Returns dynamically allocated string that must be freed
 * -- Searches in same directory and include/ directory
 * -- Handles common naming patterns (file.c -> file.h)
 */
char* cross_reference_find_header_file(const char* c_file_path);

/*
 * Find the corresponding implementation file for a given .h file
 *
 * `h_file_path` - Path to the .h file
 *
 * `char*` - Path to corresponding .c file, or NULL if not found
 *
 * -- Returns dynamically allocated string that must be freed
 * -- Searches in src/ directory and subdirectories
 * -- Handles common naming patterns (file.h -> file.c)
 */
char* cross_reference_find_impl_file(const char* h_file_path);

/*
 * Compare function signatures between header and implementation
 *
 * `header_func` - Function info from header file
 * `impl_func` - Function info from implementation file
 *
 * `bool` - true if signatures match, false otherwise
 *
 * -- Compares parameter types, names, and order
 * -- Compares return types
 * -- Handles common variations (const, pointer differences)
 */
bool cross_reference_compare_signatures(const FunctionInfo_t* header_func, const FunctionInfo_t* impl_func);

/*
 * Compare documentation between header and implementation
 *
 * `header_func` - Function info from header file
 * `impl_func` - Function info from implementation file
 *
 * `bool` - true if documentation is consistent, false otherwise
 *
 * -- Checks if parameter descriptions match actual parameters
 * -- Verifies return value descriptions are consistent
 * -- Allows implementation docs to be more detailed than header docs
 */
bool cross_reference_compare_documentation(const FunctionInfo_t* header_func, const FunctionInfo_t* impl_func);

/*
 * Perform full cross-reference analysis between a .c file and its header
 *
 * `c_file_path` - Path to the .c implementation file
 * `violations` - Violation list to append results to
 *
 * `int` - Number of cross-reference violations found
 *
 * -- Automatically finds corresponding header file
 * -- Compares all public functions between header and implementation
 * -- Checks for missing declarations/implementations
 * -- Validates signature and documentation consistency
 */
int cross_reference_analyze_file(const char* c_file_path, ViolationList_t* violations);

/*
 * Check if a function should be cross-referenced
 *
 * `func` - Function info to check
 *
 * `bool` - true if function should be cross-referenced, false otherwise
 *
 * -- Skips static functions (internal to .c file)
 * -- Skips main function
 * -- Skips functions with certain naming patterns (test_, debug_, etc.)
 */
bool cross_reference_should_check_function(const FunctionInfo_t* func);

/*
 * Initialize cross-reference violation list
 *
 * `XRefViolationList_t*` - Newly allocated violation list
 *
 * -- Allocates memory for violation tracking
 * -- Must be freed with cross_reference_free_violations()
 */
XRefViolationList_t* cross_reference_init_violations(void);

/*
 * Add a cross-reference violation to the list
 *
 * `violations` - Violation list to add to
 * `func_name` - Name of the function with the violation
 * `type` - Type of cross-reference violation
 * `description` - Human-readable description of the issue
 * `header_line` - Line number in header file (0 if not applicable)
 * `impl_line` - Line number in implementation file (0 if not applicable)
 */
void cross_reference_add_violation(XRefViolationList_t* violations, 
                                 const char* func_name,
                                 XRefViolationType_t type,
                                 const char* description,
                                 int header_line, 
                                 int impl_line);

/*
 * Clean up cross-reference violation list
 *
 * `violations` - Violation list to free
 *
 * -- Frees all allocated memory including strings
 * -- Sets pointer to NULL after freeing
 */
void cross_reference_free_violations(XRefViolationList_t* violations);

/*
 * Convert cross-reference violations to regular linter violations
 *
 * `xref_violations` - Cross-reference violations to convert
 * `file_path` - File path for the violations
 * `violations` - Regular violation list to append to
 *
 * `int` - Number of violations converted
 *
 * -- Maps cross-reference violation types to linter violation types
 * -- Preserves line numbers and descriptions
 * -- Allows cross-reference results to integrate with main linter output
 */
int cross_reference_convert_violations(const XRefViolationList_t* xref_violations,
                                     const char* file_path,
                                     ViolationList_t* violations);

#endif // CROSS_REFERENCE_H
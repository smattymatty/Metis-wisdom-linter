/* c_parser.h - Divine C language parser for Metis wisdom extraction */
// INSERT WISDOM HERE

#ifndef C_PARSER_H
#define C_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

// =============================================================================
// TOKEN SYSTEM - DIVINE CODE UNDERSTANDING
// =============================================================================

/*
 * Token types for C language parsing with divine precision
 */
typedef enum {
    TOKEN_UNKNOWN = 0,
    TOKEN_KEYWORD,          // C language keywords (if, while, int, etc.)
    TOKEN_IDENTIFIER,       // Variable/function names
    TOKEN_NUMBER,           // Numeric literals (123, 3.14, 0xAB)
    TOKEN_STRING,           // String literals ("hello")
    TOKEN_CHAR,             // Character literals ('a')
    TOKEN_OPERATOR,         // Operators (+, -, ==, !=, etc.)
    TOKEN_PUNCTUATION,      // Punctuation ((), {}, [], ;, ,, .)
    TOKEN_COMMENT_LINE,     // Line comments (//)
    TOKEN_COMMENT_BLOCK,    // Block comments (/* */)
    TOKEN_PREPROCESSOR,     // Preprocessor directives (#include, #define)
    TOKEN_NEWLINE,          // Newline characters
    TOKEN_EOF               // End of file
} TokenType_t;

/*
 * Individual token with position and content information
 */
typedef struct {
    TokenType_t type;       // Type of this token
    char* value;            // String content of the token
    int line;               // Line number (1-based)
    int column;             // Column number (1-based)
    int length;             // Length of the token in characters
} Token_t;

/*
 * Parser context for tracking position during tokenization
 */
typedef struct {
    const char* source;     // Source code being parsed
    size_t source_length;   // Total length of source
    size_t position;        // Current position in source
    int line;               // Current line number
    int column;             // Current column number
} ParserContext_t;

// =============================================================================
// FUNCTION ANALYSIS STRUCTURES
// =============================================================================

/*
 * Information about a detected function definition
 */
typedef struct {
    char* name;             // Function name
    char* return_type;      // Return type string
    char* documentation;    // Associated documentation (if any)
    char** parameters;      // Parameter list (if parsed)
    int param_count;        // Number of parameters
    int line_number;        // Line where function is defined
    int column;             // Column where function name appears
    bool is_static;         // Whether function is static
    bool is_inline;         // Whether function is inline
    bool has_documentation; // Whether function has associated comments
} FunctionInfo_t;

/*
 * Function complexity analysis results for philosophical guidance
 */
typedef struct {
    int complexity_score;   // Overall complexity score
    int nesting_depth;      // Maximum nesting depth
    int function_length;    // Function length in lines
    int branch_count;       // Number of branching statements
    bool has_multiple_returns;  // Whether function has multiple return points
    bool has_deep_nesting;      // Whether function has deep nesting (>3 levels)
} ComplexityAnalysis_t;

/*
 * Complete parsed file structure containing all extracted information
 */
typedef struct {
    char* file_path;        // Path to the source file

    // Tokens
    Token_t* tokens;        // Array of all tokens
    int token_count;        // Number of tokens
    size_t token_capacity;  // Allocated capacity for tokens

    // Functions
    FunctionInfo_t* functions;     // Array of detected functions
    int function_count;            // Number of functions
    size_t function_capacity;      // Allocated capacity for functions

    // Includes
    char** includes;        // Array of include file names
    int include_count;      // Number of includes
    size_t include_capacity; // Allocated capacity for includes
} ParsedFile_t;

// =============================================================================
// CORE PARSING FUNCTIONS
// =============================================================================

/*
 * Tokenize C source code into an array of tokens with divine precision
 *
 * `content` - C source code content to tokenize (must be null-terminated)
 * `token_count` - Output parameter for number of tokens created
 *
 * `Token_t*` - Array of tokens, or NULL on failure
 *
 * -- Must be freed with c_parser_free_tokens() to prevent memory leaks
 * -- Handles all C language constructs including comments and preprocessor
 * -- Preserves exact line and column position information for each token
 * -- Supports multi-character operators (==, <=, >=, etc.)
 * -- Properly handles string/character literals with escape sequences
 * -- Returns NULL if content is NULL or memory allocation fails
 * -- Essential for documentation analysis and dangerous function detection
 */
Token_t* c_parser_tokenize(const char* content, int* token_count);

/*
 * Parse C source code content into structured analysis format
 *
 * `content` - C source code content to parse (must be null-terminated)
 * `file_path` - File path for reference and error reporting (can be NULL)
 *
 * `ParsedFile_t*` - Complete parsed file structure, or NULL on failure
 *
 * -- Must be freed with c_parser_free_parsed_file() to prevent memory leaks
 * -- Extracts functions, includes, and complete token stream for analysis
 * -- Distinguishes function definitions from function calls with high accuracy
 * -- Identifies dangerous function usage patterns for Daedalus suggestions
 * -- Preserves all positional information for precise error reporting
 * -- Primary tool for documentation linting and code quality analysis
 * -- Returns NULL if content is NULL or critical parsing failure occurs
 */
ParsedFile_t* c_parser_parse_content(const char* content, const char* file_path);

/*
 * Parse a C source file from disk into structured analysis format
 *
 * `file_path` - Path to C source file to parse (must be readable)
 *
 * `ParsedFile_t*` - Complete parsed file structure, or NULL on failure
 *
 * -- Must be freed with c_parser_free_parsed_file() to prevent memory leaks
 * -- Reads entire file into memory before parsing for optimal performance
 * -- Handles files of any reasonable size with efficient memory management
 * -- Returns NULL if file cannot be opened, read, or parsed
 * -- Automatically detects file encoding issues and truncated reads
 * -- Convenience wrapper around c_parser_parse_content() for file operations
 */
ParsedFile_t* c_parser_parse_file(const char* file_path);

// =============================================================================
// MEMORY MANAGEMENT
// =============================================================================

/*
 * Free an array of tokens with all associated memory
 *
 * `tokens` - Array of tokens to free
 * `count` - Number of tokens in the array
 *
 * -- Safe to call with NULL pointer (does nothing)
 * -- Frees all token value strings and the token array itself
 * -- Must be called for every array returned by c_parser_tokenize()
 * -- After calling, the tokens pointer becomes invalid
 */
void c_parser_free_tokens(Token_t* tokens, int count);

/*
 * Free a parsed file structure with complete memory cleanup
 *
 * `parsed` - Parsed file structure to free
 *
 * -- Safe to call with NULL pointer (does nothing)
 * -- Frees all tokens, functions, includes, and file path strings
 * -- Recursively frees all dynamically allocated strings and arrays
 * -- Must be called for every structure returned by parse functions
 * -- After calling, the parsed pointer becomes invalid
 */
void c_parser_free_parsed_file(ParsedFile_t* parsed);

// =============================================================================
// LANGUAGE RECOGNITION UTILITIES
// =============================================================================

/*
 * Check if a word is a standard C language keyword
 *
 * `word` - Word to check (must be null-terminated)
 *
 * `bool` - true if word is a C keyword, false otherwise
 *
 * -- Returns false if word is NULL
 * -- Recognizes all C99, C11, and common compiler extension keywords
 * -- Case-sensitive matching (C keywords are lowercase)
 * -- Includes modern keywords like _Static_assert, _Thread_local
 * -- Used for syntax highlighting and token classification
 */
bool c_parser_is_c_keyword(const char* word);

/*
 * Check if a function name is dangerous and should be replaced with Daedalus alternatives
 *
 * `func_name` - Function name to check (must be null-terminated)
 *
 * `bool` - true if function is dangerous, false otherwise
 *
 * -- Returns false if func_name is NULL
 * -- Identifies unsafe string functions (strcpy, sprintf, etc.)
 * -- Detects memory management functions (malloc, free, etc.)
 * -- Covers buffer overflow risks and unbounded operations
 * -- Used by linters to suggest Daedalus library alternatives
 * -- Essential for security-focused code analysis
 */
bool c_parser_is_dangerous_function(const char* func_name);

/*
 * Check if a word is a standard C type keyword or common type identifier
 *
 * `word` - Word to check (must be null-terminated)
 *
 * `bool` - true if word is a type keyword, false otherwise
 *
 * -- Returns false if word is NULL
 * -- Recognizes primitive types (int, char, float, etc.)
 * -- Includes standard library types (size_t, FILE, etc.)
 * -- Covers fixed-width integer types (uint32_t, int64_t, etc.)
 * -- Used for variable declaration detection and type analysis
 */
bool c_parser_is_type_keyword(const char* word);

/*
 * Get human-readable name for a token type
 *
 * `type` - Token type to convert to string
 *
 * `const char*` - String representation of token type
 *
 * -- Always returns a valid string, never NULL
 * -- Used for debugging, error messages, and development tools
 * -- Returns "INVALID" for unrecognized token types
 * -- String contents are static and do not need to be freed
 */
const char* c_parser_token_type_name(TokenType_t type);

// =============================================================================
// QUERY AND ANALYSIS INTERFACE
// =============================================================================

/*
 * Find function information at a specific line number
 *
 * `parsed` - Parsed file structure to search
 * `line` - Line number to search for (1-based)
 *
 * `FunctionInfo_t*` - Pointer to function info, or NULL if not found
 *
 * -- Returns NULL if parsed is NULL or no function found at line
 * -- Returned pointer is valid until parsed structure is freed
 * -- Only finds functions that start exactly on the specified line
 * -- Used for IDE integration and precise error reporting
 * -- Essential for documentation linting of specific functions
 */
FunctionInfo_t* c_parser_find_function_at_line(ParsedFile_t* parsed, int line);


/*
 * Check if a specific line contains a text pattern
 *
 * `parsed` - Parsed file structure to search
 * `line` - Line number to check (1-based)
 * `pattern` - Text pattern to search for (must be null-terminated)
 *
 * `bool` - true if pattern found on line, false otherwise
 *
 * -- Returns false if parsed or pattern is NULL
 * -- Searches through all tokens on the specified line
 * -- Uses substring matching (strstr) for flexible pattern detection
 * -- Case-sensitive pattern matching
 * -- Used for custom linting rules and pattern-based analysis
*/
bool c_parser_has_documentation_for_function(ParsedFile_t* parsed, const char* func_name);

/*
 * Check if a specific line contains a text pattern
 *
 * `parsed` - Parsed file structure to search
 * `line` - Line number to check (1-based)
 * `pattern` - Text pattern to search for (must be null-terminated)
 *
 * `bool` - true if pattern found on line, false otherwise
 *
 * -- Returns false if parsed or pattern is NULL
 * -- Searches through all tokens on the specified line
 * -- Uses substring matching (strstr) for flexible pattern detection
 * -- Case-sensitive pattern matching
 * -- Used for custom linting rules and pattern-based analysis
 */
bool c_parser_line_has_pattern(ParsedFile_t* parsed, int line, const char* pattern);

/**
 * @brief Represents an unsafe strcmp usage with dString_t->str.
 */
typedef struct {
    int line;
    int column;
    bool is_dstring_vs_cstring;
    bool is_dstring_vs_dstring;
} UnsafeStrcmpUsage_t;

/**
 * @brief Detects unsafe strcmp usage with dString_t->str.
 *
 * @param parsed The parsed file structure.
 * @param usages A pointer to a dynamically allocated array of UnsafeStrcmpUsage_t.
 * @param count The number of usages found.
 * @return true if unsafe usages were found, false otherwise.
 */
bool c_parser_detect_unsafe_strcmp_dstring_usage(ParsedFile_t* parsed, UnsafeStrcmpUsage_t** usages, int* count);

/*
 * Analyze function complexity with mathematical precision for philosophical guidance
 *
 * `parsed` - Parsed file structure containing the function
 * `func_name` - Name of function to analyze (must be null-terminated)
 *
 * `ComplexityAnalysis_t` - Complexity analysis results structure
 *
 * -- Returns zero-initialized structure if parsed, func_name is NULL or function not found
 * -- Calculates cyclomatic complexity score based on branching statements
 * -- Measures maximum nesting depth and overall function length
 * -- Counts return statements to detect multiple exit points
 * -- Identifies deeply nested code that may be hard to understand
 * -- Used for code quality assessment and refactoring guidance
 * -- Helps identify functions that violate philosophical simplicity principles
 */
ComplexityAnalysis_t c_parser_analyze_function_complexity(ParsedFile_t* parsed, const char* func_name);

// =============================================================================
// DIVINE DOCUMENTATION ANALYSIS - METIS'S DOMAIN
// =============================================================================

/*
 * Check if file has proper first line comment with filename
 *
 * `parsed` - Parsed file structure to analyze
 * `expected_filename` - Expected filename to find in first comment
 *
 * `bool` - true if first line contains proper filename comment, false otherwise
 *
 * -- Returns false if parsed or expected_filename is NULL
 * -- Checks that very first comment contains the expected filename
 * -- Enforces Metis documentation standard: first line must identify the file
 * -- Used by auto-formatters to detect missing filename headers
 * -- Essential for maintaining consistent file identification across projects
 * -- Fails if any code appears before the filename comment
 */
bool c_parser_has_proper_filename_header(ParsedFile_t* parsed, const char* expected_filename);

/*
 * Check if file has proper second line wisdom comment
 *
 * `parsed` - Parsed file structure to analyze
 *
 * `bool` - true if second comment line contains wisdom fragment, false otherwise
 *
 * -- Returns false if parsed is NULL
 * -- Looks for second comment containing wisdom markers
 * -- Accepts "INSERT WISDOM HERE", "Fragment #", or "Metis Fragment"
 * -- Enforces Metis documentation standard: second line must contain wisdom
 * -- Used by auto-formatters to detect missing wisdom headers
 * -- Essential for maintaining philosophical depth in code documentation
 * -- Fails if significant code appears before finding second comment
 */
bool c_parser_has_proper_wisdom_header(ParsedFile_t* parsed);

/*
 * Extract filename from full file path for header validation
 *
 * `file_path` - Full file path to extract filename from
 *
 * `const char*` - Pointer to filename portion, or NULL if file_path is NULL
 *
 * -- Returns NULL if file_path is NULL
 * -- Handles both Unix (/) and Windows (\) path separators
 * -- Returns pointer to filename portion within original string
 * -- Does not allocate new memory - returned pointer is into original string
 * -- Used internally by header validation functions
 * -- Essential for cross-platform filename extraction
 */
const char* c_parser_extract_filename(const char* file_path);

/*
 * Check if file has both proper filename and wisdom headers
 *
 * `parsed` - Parsed file structure to analyze
 *
 * `bool` - true if file has both required headers, false otherwise
 *
 * -- Returns false if parsed is NULL
 * -- Combines filename and wisdom header checks for complete validation
 * -- Extracts filename from parsed file path automatically
 * -- Used by linters for comprehensive header compliance checking
 * -- Essential for enforcing complete Metis documentation standards
 * -- Single function to verify all required file header elements
 */
bool c_parser_has_proper_file_headers(ParsedFile_t* parsed);

/*
 * Check if a function's header documentation follows proper one-line format
 *
 * `parsed` - Parsed file structure to analyze
 * `func_name` - Function name to check documentation format for
 *
 * `bool` - true if documentation follows one-line format, false otherwise
 *
 * -- Returns false if function has multi-line description before blank line
 * -- Enforces rule: description must be single line, then blank line, then details
 * -- Used to ensure consistent documentation format across headers
 * -- Essential for maintaining clean, scannable function documentation
 */
bool c_parser_has_proper_header_doc_format(ParsedFile_t* parsed, const char* func_name);

/*
 * Extract the one-line description from a function's documentation
 *
 * `parsed` - Parsed file structure to search
 * `func_name` - Function name to extract description for
 *
 * `char*` - Allocated string with one-line description, or NULL if not found
 *
 * -- Returns NULL if function not found or has no documentation
 * -- Extracts only the first line of documentation before blank line
 * -- Caller must free returned string to prevent memory leaks
 * -- Used for comparing header and implementation documentation consistency
 */
char* c_parser_extract_function_description(ParsedFile_t* parsed, const char* func_name);

/*
 * Check if implementation function comment matches header description exactly
 *
 * `parsed` - Parsed file structure containing implementation
 * `func_name` - Function name to check
 * `expected_description` - Expected description from header file
 *
 * `bool` - true if implementation comment matches header exactly, false otherwise
 *
 * -- Returns false if implementation has no comment or doesn't match
 * -- Enforces consistency between header declarations and implementations
 * -- Ensures documentation stays synchronized across .h and .c files
 * -- Essential for maintaining single source of truth for function descriptions
 */
bool c_parser_implementation_matches_header(ParsedFile_t* parsed, const char* func_name, const char* expected_description);


bool c_parser_detect_unsafe_strcmp_dstring_usage(ParsedFile_t* parsed,
                                                 UnsafeStrcmpUsage_t** usages_out,
                                                 int* count_out);
#endif /* C_PARSER_H */

/* test_metis_linter_advanced.c - Advanced tests for the Metis linter divine wisdom */
// INSERT WISDOM HERE

#include "tests.h"
#include "metis_linter.h"
#include "c_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define LOG(msg) printf("%s | File: %s, Line: %d\n", msg, __FILE__, __LINE__)

// Global test counters required by tests.h framework
int total_tests = 0;
int tests_passed = 0;
int tests_failed = 0;

// =============================================================================
// ADVANCED TEST HELPER FUNCTIONS
// =============================================================================

/*
 * Create a temporary test file with specified content
 */
static char* create_temp_test_file(const char* filename, const char* content) {
    char temp_path[512];
    snprintf(temp_path, sizeof(temp_path), "/tmp/%s", filename);
    
    FILE* file = fopen(temp_path, "w");
    if (!file) {
        perror("Failed to create temporary test file");
        return NULL;
    }
    
    fputs(content, file);
    fclose(file);
    
    return strdup(temp_path);
}

/*
 * Clean up temporary test file
 */
static void cleanup_temp_file(char* file_path) {
    if (file_path) {
        unlink(file_path);
        free(file_path);
    }
}

/*
 * Create content that violates documentation format with inappropriate content
 * This tests the specific bug where "Also piss" and "FIXED:" weren't being detected
 */
static const char* create_format_violation_content(void) {
    return "/* format_violation.c - File with improper documentation format */\n"
           "// INSERT WISDOM HERE\n"
           "\n"
           "#include <stdio.h>\n"
           "\n"
           "/*\n"
           " * Check if a function has documentation\n"
           " * Also piss\n"
           " *\n"
           " * `parsed` - Parsed file structure to search\n"
           " * `func_name` - Function name to check\n"
           " *\n"
           " * `bool` - true if documented, false otherwise\n"
           " */\n"
           "bool c_parser_has_documentation_for_function(ParsedFile_t* parsed, const char* func_name) {\n"
           "    return false;\n"
           "}\n"
           "\n"
           "/*\n"
           " * FIXED: This function was broken before\n"
           " * Now it works properly\n"
           " */\n"
           "int fixed_function(void) {\n"
           "    return 42;\n"
           "}\n"
           "\n"
           "/*\n"
           " * TODO: Implement this properly\n"
           " */\n"
           "void todo_function(void) {\n"
           "    // Empty implementation\n"
           "}\n";
}

/*
 * Create content with proper 3-line block comment format
 */
static const char* create_proper_3line_format_content(void) {
    return "/* proper_3line.c - File with proper 3-line documentation format */\n"
           "// INSERT WISDOM HERE\n"
           "\n"
           "#include <stdio.h>\n"
           "\n"
           "/*\n"
           " * Parse function declarations and definitions\n"
           " */\n"
           "int parse_function_declaration(ParsedFile_t* parsed) {\n"
           "    return 0;\n"
           "}\n"
           "\n"
           "/*\n"
           " * Extract documentation comment from token stream\n"
           " */\n"
           "char* extract_documentation_comment(Token_t* tokens) {\n"
           "    return NULL;\n"
           "}\n";
}

/*
 * Create content with proper 5-line block comment format
 */
static const char* create_proper_5line_format_content(void) {
    return "/* proper_5line.c - File with proper 5-line documentation format */\n"
           "// INSERT WISDOM HERE\n"
           "\n"
           "#include <stdio.h>\n"
           "\n"
           "/*\n"
           " * Check if a word is a standard C language keyword\n"
           " *\n"
           " * `word` - Word to check (must be null-terminated)\n"
           " * `bool` - true if word is a C keyword, false otherwise\n"
           " */\n"
           "bool c_parser_is_c_keyword(const char* word) {\n"
           "    return false;\n"
           "}\n";
}

/*
 * Create content with 4-line block comment format (invalid)
 */
static const char* create_invalid_4line_format_content(void) {
    return "/* invalid_4line.c - File with invalid 4-line documentation format */\n"
           "// INSERT WISDOM HERE\n"
           "\n"
           "#include <stdio.h>\n"
           "\n"
           "/*\n"
           " * This is a 4-line comment\n"
           " * which is not allowed\n"
           " * in Metis format rules\n"
           " */\n"
           "int invalid_format_function(void) {\n"
           "    return 0;\n"
           "}\n";
}

/*
 * Create content with line comments instead of block comments
 */
static const char* create_line_comment_content(void) {
    return "/* line_comment.c - File with line comments instead of block comments */\n"
           "// INSERT WISDOM HERE\n"
           "\n"
           "#include <stdio.h>\n"
           "\n"
           "// This function uses line comments\n"
           "// instead of proper block comments\n"
           "int line_comment_function(void) {\n"
           "    return 0;\n"
           "}\n"
           "\n"
           "// Another function with line comments\n"
           "void another_line_comment_function(void) {\n"
           "    // Implementation here\n"
           "}\n";
}

/*
 * Create content with complex parameter function for parameter detection test
 */
static const char* create_complex_parameter_content(void) {
    return "/* complex_params.c - File with complex parameter functions */\n"
           "// INSERT WISDOM HERE\n"
           "\n"
           "#include <stdio.h>\n"
           "\n"
           "/*\n"
           " * Function with complex parameter list for testing\n"
           " *\n"
           " * `param1` - First parameter\n"
           " * `callback` - Function pointer parameter\n"
           " * `array` - Array parameter with static size\n"
           " * `...` - Variable arguments\n"
           " *\n"
           " * `int` - Returns status code\n"
           " */\n"
           "int complex_function(\n"
           "    const ComplexStruct_t* param1,\n"
           "    void (*callback)(int, char*),\n"
           "    int array[static 10],\n"
           "    ...\n"
           ") {\n"
           "    return 0;\n"
           "}\n";
}

/*
 * Create content with undocumented function for false negative test
 */
static const char* create_undocumented_function_content(void) {
    return "/* undocumented.c - File with truly undocumented function */\n"
           "// INSERT WISDOM HERE\n"
           "\n"
           "#include <stdio.h>\n"
           "\n"
           "int undocumented_function(int a, int b) {\n"
           "    return a + b;\n"
           "}\n"
           "\n"
           "/*\n"
           " * This function is properly documented\n"
           " */\n"
           "int documented_function(void) {\n"
           "    return 0;\n"
           "}\n";
}

// =============================================================================
// DOCUMENTATION FORMAT VALIDATION TESTS (SPECIFIC BUG FOCUS)
// =============================================================================

/*
 * Test the specific bug where inappropriate content "Also piss" wasn't detected
 */
static int test_inappropriate_content_detection(void) {
    LOG("Testing inappropriate content detection in documentation");
    
    const char* content = create_format_violation_content();
    char* temp_file = create_temp_test_file("format_violation.c", content);
    TEST_ASSERT(temp_file != NULL, "Should create temporary test file");
    
    LOG("DEBUG: Running linter on file with inappropriate content");
    int result = metis_lint_file(temp_file);
    
    // Should detect format violations for inappropriate content
    TEST_ASSERT(result > 0, "Should detect documentation format violations with inappropriate content");
    
    cleanup_temp_file(temp_file);
    return 1;
}

/*
 * Test that proper 3-line block comment format is accepted
 */
static int test_proper_3line_format_acceptance(void) {
    LOG("Testing proper 3-line format acceptance");
    
    const char* content = create_proper_3line_format_content();
    char* temp_file = create_temp_test_file("proper_3line.c", content);
    TEST_ASSERT(temp_file != NULL, "Should create temporary test file");
    
    LOG("DEBUG: Running linter on file with proper 3-line format");
    int result = metis_lint_file(temp_file);
    
    // Should not detect format violations for proper 3-line format
    // (may have other violations like missing headers, but not format violations)
    printf("DEBUG: Linter result for proper 3-line format: %d violations\n", result);
    
    cleanup_temp_file(temp_file);
    return 1;
}

/*
 * Test that proper 5-line block comment format is accepted
 */
static int test_proper_5line_format_acceptance(void) {
    LOG("Testing proper 5-line format acceptance");
    
    const char* content = create_proper_5line_format_content();
    char* temp_file = create_temp_test_file("proper_5line.c", content);
    TEST_ASSERT(temp_file != NULL, "Should create temporary test file");
    
    LOG("DEBUG: Running linter on file with proper 5-line format");
    int result = metis_lint_file(temp_file);
    
    // Should not detect format violations for proper 5-line format
    printf("DEBUG: Linter result for proper 5-line format: %d violations\n", result);
    
    cleanup_temp_file(temp_file);
    return 1;
}

/*
 * Test that invalid 4-line block comment format is rejected
 */
static int test_invalid_4line_format_rejection(void) {
    LOG("Testing invalid 4-line format rejection");
    
    const char* content = create_invalid_4line_format_content();
    char* temp_file = create_temp_test_file("invalid_4line.c", content);
    TEST_ASSERT(temp_file != NULL, "Should create temporary test file");
    
    LOG("DEBUG: Running linter on file with invalid 4-line format");
    int result = metis_lint_file(temp_file);
    
    // Should detect format violations for invalid 4-line format
    TEST_ASSERT(result > 0, "Should detect documentation format violations for 4-line format");
    
    cleanup_temp_file(temp_file);
    return 1;
}

/*
 * Test that line comments are properly rejected as documentation
 */
static int test_line_comment_rejection(void) {
    LOG("Testing line comment rejection for documentation");
    
    const char* content = create_line_comment_content();
    char* temp_file = create_temp_test_file("line_comment.c", content);
    TEST_ASSERT(temp_file != NULL, "Should create temporary test file");
    
    LOG("DEBUG: Running linter on file with line comments");
    int result = metis_lint_file(temp_file);
    
    // Should detect that functions are undocumented (line comments don't count)
    TEST_ASSERT(result > 0, "Should detect undocumented functions when only line comments present");
    
    cleanup_temp_file(temp_file);
    return 1;
}

// =============================================================================
// PARAMETER DETECTION TESTS
// =============================================================================

/*
 * Test parameter detection for complex functions
 */
static int test_complex_parameter_detection(void) {
    LOG("Testing complex parameter detection");
    
    const char* content = create_complex_parameter_content();
    char* temp_file = create_temp_test_file("complex_params.c", content);
    TEST_ASSERT(temp_file != NULL, "Should create temporary test file");
    
    // Parse the file to check parameter detection
    char* file_content = malloc(strlen(content) + 1);
    strcpy(file_content, content);
    
    ParsedFile_t* parsed = c_parser_parse_content(file_content, temp_file);
    TEST_ASSERT(parsed != NULL, "Should successfully parse complex parameter content");
    
    LOG("DEBUG: Checking parameter detection for complex_function");
    bool found_complex_func = false;
    for (int i = 0; i < parsed->function_count; i++) {
        printf("Function %d: '%s' with %d parameters\n", 
               i, parsed->functions[i].name, parsed->functions[i].param_count);
        
        if (strcmp(parsed->functions[i].name, "complex_function") == 0) {
            found_complex_func = true;
            TEST_ASSERT(parsed->functions[i].param_count > 0, 
                       "complex_function should have parameters detected");
            LOG("DEBUG: complex_function parameter count detected correctly");
        }
    }
    
    TEST_ASSERT(found_complex_func, "Should find complex_function in parsed results");
    
    c_parser_free_parsed_file(parsed);
    free(file_content);
    cleanup_temp_file(temp_file);
    return 1;
}

// =============================================================================
// FALSE POSITIVE/NEGATIVE REGRESSION TESTS
// =============================================================================

/*
 * Test for false negatives - undocumented functions should be detected
 */
static int test_undocumented_function_detection(void) {
    LOG("Testing undocumented function detection (false negative prevention)");
    
    const char* content = create_undocumented_function_content();
    char* temp_file = create_temp_test_file("undocumented.c", content);
    TEST_ASSERT(temp_file != NULL, "Should create temporary test file");
    
    LOG("DEBUG: Running linter to check undocumented function detection");
    int result = metis_lint_file(temp_file);
    
    // Should detect undocumented functions
    TEST_ASSERT(result > 0, "Should detect undocumented functions");
    
    cleanup_temp_file(temp_file);
    return 1;
}

/*
 * Regression test for the specific header format violation bug
 */
static int test_header_format_violation_regression(void) {
    LOG("Testing header format violation regression");
    
    // Create header content similar to include/c_parser.h with format violations
    const char* header_content = 
        "/* test_header.h - Test header with format violations */\n"
        "// INSERT WISDOM HERE\n"
        "\n"
        "#ifndef TEST_HEADER_H\n"
        "#define TEST_HEADER_H\n"
        "\n"
        "/*\n"
        " * Check if a function has associated documentation\n"
        " * Also piss\n"
        " *\n"
        " * `parsed` - Parsed file structure to search\n"
        " * `func_name` - Function name to check\n"
        " *\n"
        " * `bool` - true if function has documentation, false otherwise\n"
        " */\n"
        "bool test_has_documentation_for_function(ParsedFile_t* parsed, const char* func_name);\n"
        "\n"
        "#endif\n";
    
    char* temp_file = create_temp_test_file("test_header.h", header_content);
    TEST_ASSERT(temp_file != NULL, "Should create temporary header file");
    
    LOG("DEBUG: Running linter on header file with format violations");
    int result = metis_lint_file(temp_file);
    
    // Should detect format violations in header file
    TEST_ASSERT(result > 0, "Should detect header documentation format violations");
    
    cleanup_temp_file(temp_file);
    return 1;
}

// =============================================================================
// DANGEROUS FUNCTION DETECTION TESTS
// =============================================================================

/*
 * Test detection of various dangerous functions
 */
static int test_comprehensive_dangerous_function_detection(void) {
    LOG("Testing comprehensive dangerous function detection");
    
    const char* dangerous_content = 
        "/* dangerous_comprehensive.c - File with various dangerous functions */\n"
        "// INSERT WISDOM HERE\n"
        "\n"
        "#include <string.h>\n"
        "#include <stdio.h>\n"
        "\n"
        "void test_dangerous_functions(void) {\n"
        "    char buffer[100];\n"
        "    char* src = \"test\";\n"
        "    \n"
        "    strcpy(buffer, src);        // Dangerous\n"
        "    strcat(buffer, src);        // Dangerous\n"
        "    sprintf(buffer, \"%s\", src); // Dangerous\n"
        "    gets(buffer);               // Very dangerous\n"
        "}\n";
    
    char* temp_file = create_temp_test_file("dangerous_comprehensive.c", dangerous_content);
    TEST_ASSERT(temp_file != NULL, "Should create temporary test file");
    
    LOG("DEBUG: Running linter to detect dangerous functions");
    int result = metis_lint_file(temp_file);
    
    // Should detect multiple dangerous function violations
    TEST_ASSERT(result > 0, "Should detect multiple dangerous function violations");
    
    cleanup_temp_file(temp_file);
    return 1;
}

// =============================================================================
// PHILOSOPHICAL VIOLATION TESTS
// =============================================================================

/*
 * Test detection of various philosophical violations
 */
static int test_comprehensive_philosophical_violations(void) {
    LOG("Testing comprehensive philosophical violation detection");
    
    const char* philosophical_content = 
        "/* philosophical.c - File with various philosophical violations */\n"
        "// INSERT WISDOM HERE\n"
        "\n"
        "#include <stdio.h>\n"
        "\n"
        "void test_philosophical_violations(void) {\n"
        "    // TODO: Implement this function properly\n"
        "    printf(\"Hello\");\n"
        "    \n"
        "    // FIXME: This is broken and needs fixing\n"
        "    int x = 42;\n"
        "    \n"
        "    // HACK: Quick and dirty solution\n"
        "    return;\n"
        "    \n"
        "    // XXX: This is problematic\n"
        "    int y = x + 1;\n"
        "}\n";
    
    char* temp_file = create_temp_test_file("philosophical.c", philosophical_content);
    TEST_ASSERT(temp_file != NULL, "Should create temporary test file");
    
    LOG("DEBUG: Running linter to detect philosophical violations");
    int result = metis_lint_file(temp_file);
    
    // Should detect multiple philosophical violations
    TEST_ASSERT(result > 0, "Should detect multiple philosophical violations");
    
    cleanup_temp_file(temp_file);
    return 1;
}

// =============================================================================
// COMPLEXITY ANALYSIS TESTS
// =============================================================================

/*
 * Test detection of overly complex functions
 */
static int test_complexity_analysis_detection(void) {
    LOG("Testing complexity analysis detection");
    
    const char* complex_content = 
        "/* complex.c - File with overly complex function */\n"
        "// INSERT WISDOM HERE\n"
        "\n"
        "#include <stdio.h>\n"
        "\n"
        "/*\n"
        " * Very complex function for testing\n"
        " */\n"
        "int very_complex_function(int a, int b, int c, int d, int e) {\n"
        "    if (a > 0) {\n"
        "        if (b > 0) {\n"
        "            if (c > 0) {\n"
        "                for (int i = 0; i < d; i++) {\n"
        "                    if (i % 2 == 0) {\n"
        "                        if (e > i) {\n"
        "                            return 1;\n"
        "                        } else {\n"
        "                            return 2;\n"
        "                        }\n"
        "                    } else {\n"
        "                        while (e > 0) {\n"
        "                            if (e % 3 == 0) {\n"
        "                                return 3;\n"
        "                            }\n"
        "                            e--;\n"
        "                        }\n"
        "                    }\n"
        "                }\n"
        "            } else {\n"
        "                return 4;\n"
        "            }\n"
        "        } else {\n"
        "            return 5;\n"
        "        }\n"
        "    } else {\n"
        "        return 6;\n"
        "    }\n"
        "    return 0;\n"
        "}\n";
    
    char* temp_file = create_temp_test_file("complex.c", complex_content);
    TEST_ASSERT(temp_file != NULL, "Should create temporary test file");
    
    LOG("DEBUG: Running linter to detect complexity violations");
    int result = metis_lint_file(temp_file);
    
    // Should detect complexity violations
    TEST_ASSERT(result > 0, "Should detect complexity violations");
    
    cleanup_temp_file(temp_file);
    return 1;
}

// =============================================================================
// HEADER FILE ANALYSIS TESTS
// =============================================================================

/*
 * Test header file analysis for missing headers
 */
static int test_header_file_analysis(void) {
    LOG("Testing header file analysis");
    
    const char* bad_header_content = 
        "#ifndef BAD_HEADER_H\n"
        "#define BAD_HEADER_H\n"
        "\n"
        "int some_function(void);\n"
        "\n"
        "#endif\n";
    
    char* temp_file = create_temp_test_file("bad_header.h", bad_header_content);
    TEST_ASSERT(temp_file != NULL, "Should create temporary header file");
    
    LOG("DEBUG: Running linter on header file without proper headers");
    int result = metis_lint_file(temp_file);
    
    // Should detect missing filename and wisdom headers
    TEST_ASSERT(result > 0, "Should detect missing headers in header file");
    
    cleanup_temp_file(temp_file);
    return 1;
}

// =============================================================================
// MEMORY AND EDGE CASE TESTS
// =============================================================================





/*
 * Test linter with empty file
 */
static int test_empty_file_handling(void) {
    LOG("Testing empty file handling");
    
    char* temp_file = create_temp_test_file("empty.c", "");
    TEST_ASSERT(temp_file != NULL, "Should create temporary empty file");
    
    int result = metis_lint_file(temp_file);
    
    // Should handle empty file gracefully (may detect missing headers)
    TEST_ASSERT(result >= 0, "Should handle empty file without crashing");
    
    cleanup_temp_file(temp_file);
    return 1;
}

/*
 * Test linter with file containing only whitespace
 */
static int test_whitespace_only_file_handling(void) {
    LOG("Testing whitespace-only file handling");
    
    char* temp_file = create_temp_test_file("whitespace.c", "   \n\n  \t  \n");
    TEST_ASSERT(temp_file != NULL, "Should create temporary whitespace file");
    
    int result = metis_lint_file(temp_file);
    
    // Should handle whitespace-only file gracefully
    TEST_ASSERT(result >= 0, "Should handle whitespace-only file without crashing");
    
    cleanup_temp_file(temp_file);
    return 1;
}

// =============================================================================
// REGRESSION TESTS FOR HEADER FILE DOCUMENTATION DETECTION
// =============================================================================

/*
 * Test that well-documented header functions are properly recognized
 * This is a regression test for the bug where header functions with perfect docs
 * are being marked as undocumented
 */
static int test_header_documentation_recognition_regression(void) {
    LOG("Testing header documentation recognition regression");
    
    // Create header content similar to include/c_parser.h with PROPER documentation
    const char* well_documented_header = 
        "/* test_documented.h - Test header with perfect documentation */\n"
        "// INSERT WISDOM HERE\n"
        "\n"
        "#ifndef TEST_DOCUMENTED_H\n"
        "#define TEST_DOCUMENTED_H\n"
        "\n"
        "/*\n"
        " * Tokenize C source code into an array of tokens with divine precision\n"
        " *\n"
        " * `content` - C source code content to tokenize (must be null-terminated)\n"
        " * `token_count` - Output parameter for number of tokens created\n"
        " *\n"
        " * `Token_t*` - Array of tokens, or NULL on failure\n"
        " */\n"
        "Token_t* c_parser_tokenize(const char* content, int* token_count);\n"
        "\n"
        "/*\n"
        " * Parse C source code content into structured analysis format\n"
        " *\n"
        " * `content` - C source code content to parse (must be null-terminated)\n"
        " * `file_path` - File path for reference and error reporting (can be NULL)\n"
        " *\n"
        " * `ParsedFile_t*` - Complete parsed file structure, or NULL on failure\n"
        " */\n"
        "ParsedFile_t* c_parser_parse_content(const char* content, const char* file_path);\n"
        "\n"
        "#endif\n";
    
    char* temp_file = create_temp_test_file("test_documented.h", well_documented_header);
    TEST_ASSERT(temp_file != NULL, "Should create temporary well-documented header file");
    
    LOG("DEBUG: Running linter on well-documented header file");
    printf("DEBUG: Testing file: %s\\n", temp_file);
    
    // Parse the file directly to debug documentation detection
    char* file_content = malloc(strlen(well_documented_header) + 1);
    strcpy(file_content, well_documented_header);
    
    ParsedFile_t* parsed = c_parser_parse_content(file_content, temp_file);
    TEST_ASSERT(parsed != NULL, "Should successfully parse well-documented header");
    
    LOG("DEBUG: Checking documentation detection for header functions");
    printf("Total functions found: %d\\n", parsed->function_count);
    
    for (int i = 0; i < parsed->function_count; i++) {
        printf("Function %d: '%s' at line %d, has_documentation=%s\\n", 
               i, parsed->functions[i].name, parsed->functions[i].line_number,
               parsed->functions[i].has_documentation ? "TRUE" : "FALSE");
        
        if (parsed->functions[i].documentation) {
            printf("  Documentation: '%s'\\n", parsed->functions[i].documentation);
        } else {
            printf("  Documentation: NULL\\n");
        }
        
        // Check documentation detection manually
        bool has_docs = c_parser_has_documentation_for_function(parsed, parsed->functions[i].name);
        printf("  Manual doc check result: %s\\n", has_docs ? "TRUE" : "FALSE");
    }
    
    int result = metis_lint_file(temp_file);
    printf("DEBUG: Linter result for well-documented header: %d violations\\n", result);
    
    // Should NOT detect undocumented function violations (may have other violations)
    // The key test is that functions with proper docs shouldn't be marked as undocumented
    
    // Add specific assertions based on what we learned
    bool found_tokenize = false;
    bool found_parse_content = false;
    
    for (int i = 0; i < parsed->function_count; i++) {
        if (strcmp(parsed->functions[i].name, "c_parser_tokenize") == 0) {
            found_tokenize = true;
            TEST_ASSERT(parsed->functions[i].has_documentation, 
                       "c_parser_tokenize should be detected as documented");
            
            // Check if format validation is working correctly for proper docs
            bool proper_format = c_parser_has_proper_header_doc_format(parsed, "c_parser_tokenize");
            printf("  c_parser_tokenize format validation: %s\\n", proper_format ? "PROPER" : "IMPROPER");
            // This should pass format validation - it's a proper 5-line format
            TEST_ASSERT(proper_format, "c_parser_tokenize should pass format validation");
        }
        else if (strcmp(parsed->functions[i].name, "c_parser_parse_content") == 0) {
            found_parse_content = true;
            TEST_ASSERT(parsed->functions[i].has_documentation, 
                       "c_parser_parse_content should be detected as documented");
            
            // Check if format validation is working correctly for proper docs
            bool proper_format = c_parser_has_proper_header_doc_format(parsed, "c_parser_parse_content");
            printf("  c_parser_parse_content format validation: %s\\n", proper_format ? "PROPER" : "IMPROPER");
            // This should pass format validation - it's a proper 5-line format
            TEST_ASSERT(proper_format, "c_parser_parse_content should pass format validation");
        }
    }
    
    TEST_ASSERT(found_tokenize, "Should find c_parser_tokenize function");
    TEST_ASSERT(found_parse_content, "Should find c_parser_parse_content function");
    
    c_parser_free_parsed_file(parsed);
    free(file_content);
    cleanup_temp_file(temp_file);
    return 1;
}

/*
 * Test that the "Also piss" inappropriate content is properly detected
 * This is a regression test for the bug where format violations in headers aren't detected
 */
static int test_header_inappropriate_content_detection_regression(void) {
    LOG("Testing header inappropriate content detection regression");
    
    // Create content exactly like the c_parser.h function that should fail
    const char* inappropriate_header = 
        "/* test_inappropriate.h - Test header with inappropriate content */\n"
        "// INSERT WISDOM HERE\n"
        "\n"
        "#ifndef TEST_INAPPROPRIATE_H\n"
        "#define TEST_INAPPROPRIATE_H\n"
        "\n"
        "/*\n"
        " * Check if a function has associated documentation\n"
        " * Also piss\n"
        " *\n"
        " * `parsed` - Parsed file structure to search\n"
        " * `func_name` - Function name to check (must be null-terminated)\n"
        " *\n"
        " * `bool` - true if function has documentation, false otherwise\n"
        " */\n"
        "bool test_has_documentation_for_function(ParsedFile_t* parsed, const char* func_name);\n"
        "\n"
        "#endif\n";
    
    char* temp_file = create_temp_test_file("test_inappropriate.h", inappropriate_header);
    TEST_ASSERT(temp_file != NULL, "Should create temporary inappropriate header file");
    
    LOG("DEBUG: Running linter on header with inappropriate content");
    printf("DEBUG: Testing file: %s\\n", temp_file);
    
    // Parse and check format validation manually
    char* file_content = malloc(strlen(inappropriate_header) + 1);
    strcpy(file_content, inappropriate_header);
    
    ParsedFile_t* parsed = c_parser_parse_content(file_content, temp_file);
    TEST_ASSERT(parsed != NULL, "Should successfully parse inappropriate header");
    
    LOG("DEBUG: Checking format validation for inappropriate content");
    for (int i = 0; i < parsed->function_count; i++) {
        printf("Function %d: '%s' at line %d\\n", 
               i, parsed->functions[i].name, parsed->functions[i].line_number);
        
        // Check format validation manually
        bool proper_format = c_parser_has_proper_header_doc_format(parsed, parsed->functions[i].name);
        printf("  Format validation result: %s\\n", proper_format ? "PROPER" : "IMPROPER");
        
        if (strcmp(parsed->functions[i].name, "test_has_documentation_for_function") == 0) {
            TEST_ASSERT(!proper_format, "Function with 'Also piss' should fail format validation");
            LOG("DEBUG: Format validation correctly detected inappropriate content");
        }
    }
    
    int result = metis_lint_file(temp_file);
    printf("DEBUG: Linter result for inappropriate header: %d violations\\n", result);
    
    // Should detect format violations for inappropriate content
    TEST_ASSERT(result > 0, "Should detect header format violations with inappropriate content");
    
    c_parser_free_parsed_file(parsed);
    free(file_content);
    cleanup_temp_file(temp_file);
    return 1;
}

/*
 * Test documentation detection logic with step-by-step debugging
 * This helps identify exactly where the documentation detection is failing
 */
static int test_documentation_detection_debugging(void) {
    LOG("Testing documentation detection with step-by-step debugging");
    
    const char* debug_content = 
        "/* debug.h - Debug documentation detection */\n"
        "// INSERT WISDOM HERE\n"
        "\n"
        "/*\n"
        " * Simple documented function\n"
        " */\n"
        "int simple_function(void);\n"
        "\n"
        "int undocumented_function(void);\n"
        "\n"
        "/*\n"
        " * Another documented function\n"
        " *\n"
        " * `param` - A parameter\n"
        " *\n"
        " * `int` - Return value\n"
        " */\n"
        "int complex_documented_function(int param);\n";
    
    char* temp_file = create_temp_test_file("debug.h", debug_content);
    TEST_ASSERT(temp_file != NULL, "Should create temporary debug file");
    
    LOG("DEBUG: Step-by-step documentation detection analysis");
    
    char* file_content = malloc(strlen(debug_content) + 1);
    strcpy(file_content, debug_content);
    
    ParsedFile_t* parsed = c_parser_parse_content(file_content, temp_file);
    TEST_ASSERT(parsed != NULL, "Should successfully parse debug content");
    
    printf("\\n=== TOKEN ANALYSIS ===\\n");
    printf("Total tokens: %d\\n", parsed->token_count);
    
    // Show tokens around functions to understand the parsing
    for (int i = 0; i < parsed->token_count && i < 50; i++) {
        Token_t* token = &parsed->tokens[i];
        printf("Token %d: type=%s, value='%s', line=%d, col=%d\\n", 
               i, c_parser_token_type_name(token->type),
               token->value ? token->value : "NULL",
               token->line, token->column);
    }
    
    printf("\\n=== FUNCTION ANALYSIS ===\\n");
    for (int i = 0; i < parsed->function_count; i++) {
        FunctionInfo_t* func = &parsed->functions[i];
        printf("Function %d: '%s' at line %d\\n", i, func->name, func->line_number);
        printf("  Initial has_documentation: %s\\n", func->has_documentation ? "TRUE" : "FALSE");
        
        // Test documentation detection manually
        bool manual_check = c_parser_has_documentation_for_function(parsed, func->name);
        printf("  Manual documentation check: %s\\n", manual_check ? "TRUE" : "FALSE");
        printf("  After manual check has_documentation: %s\\n", func->has_documentation ? "TRUE" : "FALSE");
        
        if (func->documentation) {
            printf("  Documentation content: '%s'\\n", func->documentation);
        } else {
            printf("  Documentation content: NULL\\n");
        }
        
        // Add specific test assertions
        if (strcmp(func->name, "simple_function") == 0) {
            TEST_ASSERT(func->has_documentation, "simple_function should be documented");
            TEST_ASSERT(manual_check, "simple_function manual check should be TRUE");
        }
        else if (strcmp(func->name, "undocumented_function") == 0) {
            // This is the key test - undocumented_function should NOT be documented
            printf("  CRITICAL TEST: undocumented_function should be FALSE but is %s\\n", 
                   func->has_documentation ? "TRUE" : "FALSE");
            TEST_ASSERT(!func->has_documentation, "undocumented_function should NOT be documented");
            TEST_ASSERT(!manual_check, "undocumented_function manual check should be FALSE");
        }
        else if (strcmp(func->name, "complex_documented_function") == 0) {
            TEST_ASSERT(func->has_documentation, "complex_documented_function should be documented");
            TEST_ASSERT(manual_check, "complex_documented_function manual check should be TRUE");
        }
        
        printf("\\n");
    }
    
    c_parser_free_parsed_file(parsed);
    free(file_content);
    cleanup_temp_file(temp_file);
    return 1;
}

// =============================================================================
// MAIN TEST RUNNER
// =============================================================================

int main(void) {
    TEST_SUITE_START("Metis Linter Advanced Tests");
    
    // Documentation format validation tests (specific bug focus)
    RUN_TEST(test_inappropriate_content_detection);
    RUN_TEST(test_proper_3line_format_acceptance);
    RUN_TEST(test_proper_5line_format_acceptance);
    RUN_TEST(test_invalid_4line_format_rejection);
    RUN_TEST(test_line_comment_rejection);
    
    // Parameter detection tests
    RUN_TEST(test_complex_parameter_detection);
    
    // False positive/negative regression tests
    RUN_TEST(test_undocumented_function_detection);
    RUN_TEST(test_header_format_violation_regression);
    
    // Dangerous function detection tests
    RUN_TEST(test_comprehensive_dangerous_function_detection);
    
    // Philosophical violation tests
    RUN_TEST(test_comprehensive_philosophical_violations);
    
    // Complexity analysis tests
    RUN_TEST(test_complexity_analysis_detection);
    
    // Header file analysis tests
    RUN_TEST(test_header_file_analysis);
    
    // Memory and edge case tests
    RUN_TEST(test_empty_file_handling);
    RUN_TEST(test_whitespace_only_file_handling);
    
    // Regression tests for header file documentation detection bugs
    RUN_TEST(test_header_documentation_recognition_regression);
    RUN_TEST(test_header_inappropriate_content_detection_regression);
    RUN_TEST(test_documentation_detection_debugging);
    
    TEST_SUITE_END();
}
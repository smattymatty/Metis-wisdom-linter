/* test_c_parser_advanced.c - Advanced tests for the C parser divine wisdom */
// INSERT WISDOM HERE

#include "tests.h"
#include "c_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOG(msg) printf("%s | File: %s, Line: %d\n", msg, __FILE__, __LINE__)

// Global test counters required by tests.h framework
int total_tests = 0;
int tests_passed = 0;
int tests_failed = 0;

// =============================================================================
// ADVANCED TEST HELPER FUNCTIONS
// =============================================================================

/*
 * Create content with valid one-line documentation format
 * This tests the false positive bug where valid one-line docs are flagged
 */
static const char* create_valid_oneline_docs_content(void) {
    return "/* valid_oneline.c - File with proper one-line documentation */\n"
           "// INSERT WISDOM HERE\n"
           "\n"
           "#include <stdio.h>\n"
           "\n"
           "/* Check if a token is a C keyword */\n"
           "bool c_parser_is_c_keyword(const char* token) {\n"
           "    return false;\n"
           "}\n"
           "\n"
           "/* Parse function declarations and definitions */\n"
           "int parse_function_declaration(ParsedFile_t* parsed) {\n"
           "    return 0;\n"
           "}\n"
           "\n"
           "/* Extract documentation comment from token stream */\n"
           "char* extract_documentation_comment(Token_t* tokens) {\n"
           "    return NULL;\n"
           "}\n";
}

/*
 * Create content with mixed documentation formats (valid and invalid)
 */
static const char* create_mixed_docs_content(void) {
    return "/* mixed.c - File with mixed documentation formats */\n"
           "// INSERT WISDOM HERE\n"
           "\n"
           "#include <stdio.h>\n"
           "\n"
           "/* Valid one-line documentation */\n"
           "void valid_function_one(void) {}\n"
           "\n"
           "/*\n"
           " * Valid multi-line documentation\n"
           " * with proper format\n"
           " *\n"
           " * Returns: nothing\n"
           " */\n"
           "void valid_function_multi(void) {}\n"
           "\n"
           "/* Invalid: missing space after asterisk\n"
           " *Missing space here\n"
           " */\n"
           "void invalid_function(void) {}\n"
           "\n"
           "// This is not proper documentation format\n"
           "void undocumented_function(void) {}\n";
}

/*
 * Create content with complex nested structures and edge cases
 */
static const char* create_complex_parsing_content(void) {
    return "/* complex.c - Complex parsing scenarios */\n"
           "// INSERT WISDOM HERE\n"
           "\n"
           "#include <stdio.h>\n"
           "#include <stdlib.h>\n"
           "\n"
           "typedef struct {\n"
           "    int nested_field;\n"
           "    struct {\n"
           "        char* inner_field;\n"
           "    } inner_struct;\n"
           "} ComplexStruct_t;\n"
           "\n"
           "/* Function with complex parameter list */\n"
           "int complex_function(\n"
           "    const ComplexStruct_t* param1,\n"
           "    void (*callback)(int, char*),\n"
           "    int array[static 10],\n"
           "    ...\n"
           ") {\n"
           "    if (param1 && callback) {\n"
           "        for (int i = 0; i < 10; i++) {\n"
           "            if (array[i] > 0) {\n"
           "                callback(array[i], \"test\");\n"
           "            }\n"
           "        }\n"
           "    }\n"
           "    return 0;\n"
           "}\n"
           "\n"
           "#define COMPLEX_MACRO(x, y) \\\n"
           "    do { \\\n"
           "        if ((x) > (y)) { \\\n"
           "            printf(\"x is greater\\n\"); \\\n"
           "        } \\\n"
           "    } while(0)\n";
}

/*
 * Create content with edge case documentation patterns
 */
static const char* create_edge_case_docs_content(void) {
    return "/* edge_cases.c - Edge case documentation patterns */\n"
           "// INSERT WISDOM HERE\n"
           "\n"
           "#include <stdio.h>\n"
           "\n"
           "/* */\n"
           "void empty_doc_comment(void) {}\n"
           "\n"
           "/**/\n"
           "void minimal_doc_comment(void) {}\n"
           "\n"
           "/* Single line with trailing spaces   */\n"
           "void trailing_spaces_doc(void) {}\n"
           "\n"
           "/*\n"
           " * Multi-line with empty lines\n"
           " *\n"
           " *\n"
           " * And more content\n"
           " */\n"
           "void multi_with_empty_lines(void) {}\n"
           "\n"
           "/* Unicode characters: � � � � � */\n"
           "void unicode_in_docs(void) {}\n"
           "\n"
           "/* Very long single line documentation that exceeds typical line length limits and keeps going and going */\n"
           "void very_long_doc(void) {}\n";
}

/*
 * Create content with stress test scenarios
 */
static const char* create_stress_test_content(void) {
    return "/* stress.c - Stress test with many functions */\n"
           "// INSERT WISDOM HERE\n"
           "\n"
           "#include <stdio.h>\n"
           "\n"
           "/* Function 1 */\n"
           "void func1(void) {}\n"
           "/* Function 2 */\n"
           "void func2(void) {}\n"
           "/* Function 3 */\n"
           "void func3(void) {}\n"
           "/* Function 4 */\n"
           "void func4(void) {}\n"
           "/* Function 5 */\n"
           "void func5(void) {}\n"
           "/* Function 6 */\n"
           "void func6(void) {}\n"
           "/* Function 7 */\n"
           "void func7(void) {}\n"
           "/* Function 8 */\n"
           "void func8(void) {}\n"
           "/* Function 9 */\n"
           "void func9(void) {}\n"
           "/* Function 10 */\n"
           "void func10(void) {}\n";
}

// =============================================================================
// DOCUMENTATION FORMAT VALIDATION TESTS (FALSE POSITIVE BUG FOCUS)
// =============================================================================

/*
 * Test that valid one-line documentation is properly recognized
 * This is the core test for the false positive bug
 */
static int test_valid_oneline_documentation_recognition(void) {
    LOG("Testing valid one-line documentation recognition");
    
    const char* content = create_valid_oneline_docs_content();
    ParsedFile_t* parsed = c_parser_parse_content(content, "valid_oneline.c");
    
    TEST_ASSERT(parsed != NULL, "Parser should successfully parse valid one-line docs");
    TEST_ASSERT(parsed->function_count >= 3, "Should detect at least 3 functions");
    
    // Check that functions are properly documented
    bool found_keyword_func = false;
    bool found_parse_func = false;
    bool found_extract_func = false;
    
    LOG("DEBUG: Analyzing parsed functions for documentation detection");
    printf("Total functions found: %d\n", parsed->function_count);
    printf("Total tokens found: %d\n", parsed->token_count);
    
    // Show first 20 tokens to understand the token stream
    printf("\nFirst 20 tokens:\n");
    for (int i = 0; i < parsed->token_count && i < 20; i++) {
        printf("Token %d: type=%s, value='%s', line=%d\n", 
               i, c_parser_token_type_name(parsed->tokens[i].type),
               parsed->tokens[i].value ? parsed->tokens[i].value : "NULL",
               parsed->tokens[i].line);
    }
    printf("\n");
    
    for (int i = 0; i < parsed->function_count; i++) {
        printf("Function %d: '%s' at line %d, has_documentation=%s\n", 
               i, parsed->functions[i].name, parsed->functions[i].line_number,
               parsed->functions[i].has_documentation ? "TRUE" : "FALSE");
        
        if (parsed->functions[i].documentation) {
            printf("  Documentation: '%s'\n", parsed->functions[i].documentation);
        } else {
            printf("  Documentation: NULL\n");
        }
        
        if (strcmp(parsed->functions[i].name, "c_parser_is_c_keyword") == 0) {
            found_keyword_func = true;
            LOG("DEBUG: Found c_parser_is_c_keyword function");
            TEST_ASSERT(parsed->functions[i].has_documentation, 
                       "c_parser_is_c_keyword should be marked as documented");
        }
        else if (strcmp(parsed->functions[i].name, "parse_function_declaration") == 0) {
            found_parse_func = true;
            LOG("DEBUG: Found parse_function_declaration function");
            TEST_ASSERT(parsed->functions[i].has_documentation, 
                       "parse_function_declaration should be marked as documented");
        }
        else if (strcmp(parsed->functions[i].name, "extract_documentation_comment") == 0) {
            found_extract_func = true;
            LOG("DEBUG: Found extract_documentation_comment function");
            TEST_ASSERT(parsed->functions[i].has_documentation, 
                       "extract_documentation_comment should be marked as documented");
        }
    }
    
    TEST_ASSERT(found_keyword_func, "Should find c_parser_is_c_keyword function");
    TEST_ASSERT(found_parse_func, "Should find parse_function_declaration function");
    TEST_ASSERT(found_extract_func, "Should find extract_documentation_comment function");
    
    c_parser_free_parsed_file(parsed);
    return 1;
}

/*
 * Test mixed documentation format recognition
 */
static int test_mixed_documentation_formats(void) {
    LOG("Testing mixed documentation formats");
    
    const char* content = create_mixed_docs_content();
    ParsedFile_t* parsed = c_parser_parse_content(content, "mixed.c");
    
    TEST_ASSERT(parsed != NULL, "Parser should handle mixed documentation formats");
    TEST_ASSERT(parsed->function_count >= 4, "Should detect at least 4 functions");
    
    // Check documentation status for each function
    for (int i = 0; i < parsed->function_count; i++) {
        if (strcmp(parsed->functions[i].name, "valid_function_one") == 0) {
            TEST_ASSERT(parsed->functions[i].has_documentation, 
                       "valid_function_one should be marked as documented");
        }
        else if (strcmp(parsed->functions[i].name, "valid_function_multi") == 0) {
            TEST_ASSERT(parsed->functions[i].has_documentation, 
                       "valid_function_multi should be marked as documented");
        }
        else if (strcmp(parsed->functions[i].name, "undocumented_function") == 0) {
            TEST_ASSERT(!parsed->functions[i].has_documentation, 
                       "undocumented_function should be marked as undocumented");
        }
    }
    
    c_parser_free_parsed_file(parsed);
    return 1;
}

/*
 * Test edge case documentation patterns
 */
static int test_edge_case_documentation_patterns(void) {
    LOG("Testing edge case documentation patterns");
    
    const char* content = create_edge_case_docs_content();
    ParsedFile_t* parsed = c_parser_parse_content(content, "edge_cases.c");
    
    TEST_ASSERT(parsed != NULL, "Parser should handle edge case documentation");
    TEST_ASSERT(parsed->function_count >= 6, "Should detect at least 6 functions");
    
    // Test that various edge cases are handled properly
    for (int i = 0; i < parsed->function_count; i++) {
        if (strcmp(parsed->functions[i].name, "unicode_in_docs") == 0) {
            TEST_ASSERT(parsed->functions[i].has_documentation, 
                       "Function with unicode in docs should be marked as documented");
        }
        else if (strcmp(parsed->functions[i].name, "very_long_doc") == 0) {
            TEST_ASSERT(parsed->functions[i].has_documentation, 
                       "Function with very long doc should be marked as documented");
        }
    }
    
    c_parser_free_parsed_file(parsed);
    return 1;
}

// =============================================================================
// COMPLEX PARSING SCENARIO TESTS
// =============================================================================

/*
 * Test complex function parsing with nested structures
 */
static int test_complex_function_parsing(void) {
    LOG("Testing complex function parsing");
    
    const char* content = create_complex_parsing_content();
    ParsedFile_t* parsed = c_parser_parse_content(content, "complex.c");
    
    TEST_ASSERT(parsed != NULL, "Parser should handle complex parsing scenarios");
    TEST_ASSERT(parsed->function_count >= 1, "Should detect at least 1 function");
    
    // Find the complex_function and verify its properties
    bool found_complex_func = false;
    for (int i = 0; i < parsed->function_count; i++) {
        if (strcmp(parsed->functions[i].name, "complex_function") == 0) {
            found_complex_func = true;
            TEST_ASSERT(parsed->functions[i].has_documentation, 
                       "complex_function should be marked as documented");
            TEST_ASSERT(parsed->functions[i].param_count > 0, 
                       "complex_function should have parameters detected");
        }
    }
    
    TEST_ASSERT(found_complex_func, "Should find complex_function");
    
    c_parser_free_parsed_file(parsed);
    return 1;
}

/*
 * Test token stream parsing with various token types
 */
static int test_comprehensive_token_parsing(void) {
    LOG("Testing comprehensive token parsing");
    
    const char* content = create_complex_parsing_content();
    ParsedFile_t* parsed = c_parser_parse_content(content, "complex.c");
    
    TEST_ASSERT(parsed != NULL, "Parser should create parsed file structure");
    TEST_ASSERT(parsed->token_count > 0, "Should have tokens");
    
    // Verify various token types are present
    bool has_keywords = false;
    bool has_identifiers = false;
    bool has_operators = false;
    
    for (int i = 0; i < parsed->token_count; i++) {
        switch (parsed->tokens[i].type) {
            case TOKEN_KEYWORD:
                has_keywords = true;
                break;
            case TOKEN_IDENTIFIER:
                has_identifiers = true;
                break;
            case TOKEN_OPERATOR:
                has_operators = true;
                break;
            case TOKEN_STRING:
            case TOKEN_NUMBER:
            case TOKEN_CHAR:
                // Literal tokens detected but not tested
                break;
            default:
                break;
        }
    }
    
    TEST_ASSERT(has_keywords, "Should have detected keyword tokens");
    TEST_ASSERT(has_identifiers, "Should have detected identifier tokens");
    TEST_ASSERT(has_operators, "Should have detected operator tokens");
    
    c_parser_free_parsed_file(parsed);
    return 1;
}

// =============================================================================
// STRESS AND PERFORMANCE TESTS
// =============================================================================

/*
 * Test parser performance with many functions
 */
static int test_parser_stress_many_functions(void) {
    LOG("Testing parser stress with many functions");
    
    const char* content = create_stress_test_content();
    ParsedFile_t* parsed = c_parser_parse_content(content, "stress.c");
    
    TEST_ASSERT(parsed != NULL, "Parser should handle stress test with many functions");
    TEST_ASSERT(parsed->function_count == 10, "Should detect exactly 10 functions");
    
    // Verify all functions are properly documented
    for (int i = 0; i < parsed->function_count; i++) {
        TEST_ASSERT(parsed->functions[i].has_documentation, 
                   "All stress test functions should be marked as documented");
        TEST_ASSERT(strlen(parsed->functions[i].name) > 0, 
                   "All functions should have non-empty names");
    }
    
    c_parser_free_parsed_file(parsed);
    return 1;
}

/*
 * Test parser with very large input
 */
static int test_parser_large_input_handling(void) {
    LOG("Testing parser with large input");
    
    // Create a large input by repeating the stress test content
    size_t base_len = strlen(create_stress_test_content());
    size_t total_len = base_len * 5; // 5x repetition
    char* large_content = malloc(total_len + 1);
    
    if (!large_content) {
        TEST_ASSERT(false, "Failed to allocate memory for large content test");
        return 0;
    }
    
    strcpy(large_content, create_stress_test_content());
    for (int i = 1; i < 5; i++) {
        strcat(large_content, create_stress_test_content());
    }
    
    ParsedFile_t* parsed = c_parser_parse_content(large_content, "large.c");
    
    TEST_ASSERT(parsed != NULL, "Parser should handle large input gracefully");
    TEST_ASSERT(parsed->function_count >= 50, "Should detect many functions in large input");
    
    free(large_content);
    c_parser_free_parsed_file(parsed);
    return 1;
}

// =============================================================================
// REGRESSION TESTS FOR SPECIFIC BUGS
// =============================================================================

/*
 * Regression test for the false positive documentation format bug
 * This specifically tests the case where c_parser_is_c_keyword was flagged incorrectly
 */
static int test_regression_false_positive_docs_bug(void) {
    LOG("Testing regression for false positive documentation bug");
    
    // This is the exact case that was causing false positives
    const char* content = 
        "/* c_parser.c - Test file */\n"
        "// INSERT WISDOM HERE\n"
        "\n"
        "/* Check if a token is a C keyword */\n"
        "bool c_parser_is_c_keyword(const char* token) {\n"
        "    return false;\n"
        "}\n"
        "\n"
        "/* Parse function declarations and definitions */\n"
        "int parse_function_declaration(ParsedFile_t* parsed) {\n"
        "    return 0;\n"
        "}\n";
    
    ParsedFile_t* parsed = c_parser_parse_content(content, "c_parser.c");
    
    TEST_ASSERT(parsed != NULL, "Parser should handle the regression case");
    TEST_ASSERT(parsed->function_count >= 2, "Should detect both functions");
    
    // Specifically check that c_parser_is_c_keyword is marked as documented
    bool found_keyword_func = false;
    bool found_parse_func = false;
    
    for (int i = 0; i < parsed->function_count; i++) {
        if (strcmp(parsed->functions[i].name, "c_parser_is_c_keyword") == 0) {
            found_keyword_func = true;
            TEST_ASSERT(parsed->functions[i].has_documentation, 
                       "REGRESSION: c_parser_is_c_keyword should NOT be flagged as undocumented");
        }
        else if (strcmp(parsed->functions[i].name, "parse_function_declaration") == 0) {
            found_parse_func = true;
            TEST_ASSERT(parsed->functions[i].has_documentation, 
                       "REGRESSION: parse_function_declaration should NOT be flagged as undocumented");
        }
    }
    
    TEST_ASSERT(found_keyword_func, "Should find c_parser_is_c_keyword function");
    TEST_ASSERT(found_parse_func, "Should find parse_function_declaration function");
    
    c_parser_free_parsed_file(parsed);
    return 1;
}

/*
 * Test memory management and cleanup
 */
static int test_memory_management_edge_cases(void) {
    LOG("Testing memory management edge cases");
    
    // Test with NULL input
    ParsedFile_t* parsed_null = c_parser_parse_content(NULL, "test.c");
    TEST_ASSERT(parsed_null == NULL, "Parser should handle NULL content gracefully");
    
    // Test with empty input
    ParsedFile_t* parsed_empty = c_parser_parse_content("", "test.c");
    TEST_ASSERT(parsed_empty != NULL, "Parser should handle empty content");
    if (parsed_empty) {
        TEST_ASSERT(parsed_empty->function_count == 0, "Empty content should have no functions");
        c_parser_free_parsed_file(parsed_empty);
    }
    
    // Test with NULL filename
    const char* content = "/* test */ void func(void) {}";
    ParsedFile_t* parsed_no_name = c_parser_parse_content(content, NULL);
    TEST_ASSERT(parsed_no_name != NULL, "Parser should handle NULL filename");
    if (parsed_no_name) {
        c_parser_free_parsed_file(parsed_no_name);
    }
    
    return 1;
}

// =============================================================================
// MAIN TEST RUNNER
// =============================================================================

int main(void) {
    TEST_SUITE_START("C Parser Advanced Tests");
    
    // Documentation format validation tests (false positive bug focus)
    RUN_TEST(test_valid_oneline_documentation_recognition);
    RUN_TEST(test_mixed_documentation_formats);
    RUN_TEST(test_edge_case_documentation_patterns);
    
    // Complex parsing scenario tests
    RUN_TEST(test_complex_function_parsing);
    RUN_TEST(test_comprehensive_token_parsing);
    
    // Stress and performance tests
    RUN_TEST(test_parser_stress_many_functions);
    RUN_TEST(test_parser_large_input_handling);
    
    // Regression tests for specific bugs
    RUN_TEST(test_regression_false_positive_docs_bug);
    RUN_TEST(test_memory_management_edge_cases);
    
    TEST_SUITE_END();
}
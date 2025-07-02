/* test_c_parser_basic.c - Basic tests for the C parser divine wisdom */
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
// TEST HELPER FUNCTIONS
// =============================================================================

/*
 * Create a simple test program string for parsing
 */
static const char* create_simple_test_program(void) {
    return "/* simple.c - A basic test program */\n"
           "// INSERT WISDOM HERE\n"
           "\n"
           "#include <stdio.h>\n"
           "\n"
           "/*\n"
           " * Print a greeting message\n"
           " *\n"
           " * Returns: 0 on success\n"
           " */\n"
           "int main(void) {\n"
           "    printf(\"Hello, World!\\n\");\n"
           "    return 0;\n"
           "}\n";
}

/*
 * Create a test program with the "Also piss" documentation error
 */
static const char* create_problematic_header_content(void) {
    return "/* problematic.h - Test header with documentation issues */\n"
           "// INSERT WISDOM HERE\n"
           "\n"
           "#ifndef PROBLEMATIC_H\n"
           "#define PROBLEMATIC_H\n"
           "\n"
           "/*\n"
           " * Check if a function has associated documentation\n"
           " * Also piss\n"
           " *\n"
           " * `parsed` - Parsed file structure to search\n"
           " * `func_name` - Function name to check\n"
           " *\n"
           " * `bool` - true if function has documentation\n"
           " */\n"
           "bool check_documentation(ParsedFile_t* parsed, const char* func_name);\n"
           "\n"
           "#endif\n";
}

/*
 * Create test content with dangerous functions
 */
static const char* create_dangerous_functions_content(void) {
    return "/* dangerous.c - File with unsafe function calls */\n"
           "// INSERT WISDOM HERE\n"
           "\n"
           "#include <string.h>\n"
           "#include <stdio.h>\n"
           "\n"
           "void unsafe_operations(void) {\n"
           "    char buffer[100];\n"
           "    char* input = \"test\";\n"
           "    \n"
           "    strlen(input);          // Unsafe in Daedalus context\n"
           "    strcpy(buffer, input);  // Definitely unsafe\n"
           "    sprintf(buffer, \"test %s\", input);  // Very unsafe\n"
           "}\n";
}

// =============================================================================
// BASIC PARSING TESTS
// =============================================================================

/*
 * Test basic content parsing functionality
 */
static int test_basic_content_parsing(void) {
    const char* test_content = create_simple_test_program();
    
    ParsedFile_t* parsed = c_parser_parse_content(test_content, "test.c");
    TEST_ASSERT(parsed != NULL, "Parser should successfully parse basic C content");
    
    TEST_ASSERT(parsed->file_path != NULL, "Parsed file should have a file path");
    TEST_ASSERT(strcmp(parsed->file_path, "test.c") == 0, "File path should match input");
    
    TEST_ASSERT(parsed->token_count > 0, "Parser should generate tokens from content");
    TEST_ASSERT(parsed->tokens != NULL, "Token array should be allocated");
    
    // Debug: Print function count
    LOG("DEBUG: Function count detected");
    printf("Function count: %d\n", parsed->function_count);
    for (int i = 0; i < parsed->function_count; i++) {
        printf("Function %d: %s\n", i, parsed->functions[i].name);
    }
    
    // Should find the main function
    TEST_ASSERT(parsed->function_count == 1, "Should detect exactly one function");
    TEST_ASSERT(strcmp(parsed->functions[0].name, "main") == 0, "Should detect main function");
    
    // Should find include directive
    TEST_ASSERT(parsed->include_count == 1, "Should detect exactly one include");
    TEST_ASSERT(strcmp(parsed->includes[0], "stdio.h") == 0, "Should detect stdio.h include");
    
    c_parser_free_parsed_file(parsed);
    return 1;
}

/*
 * Test tokenization functionality
 */
static int test_tokenization(void) {
    const char* simple_code = "int x = 42; /* comment */ \"string\"";
    int token_count;
    
    Token_t* tokens = c_parser_tokenize(simple_code, &token_count);
    TEST_ASSERT(tokens != NULL, "Tokenizer should return valid token array");
    TEST_ASSERT(token_count > 0, "Should generate multiple tokens");
    
    // Verify we get expected token types
    bool found_keyword = false;
    bool found_identifier = false;
    bool found_number = false;
    bool found_comment = false;
    bool found_string = false;
    
    for (int i = 0; i < token_count; i++) {
        switch (tokens[i].type) {
            case TOKEN_KEYWORD:
                if (strcmp(tokens[i].value, "int") == 0) found_keyword = true;
                break;
            case TOKEN_IDENTIFIER:
                if (strcmp(tokens[i].value, "x") == 0) found_identifier = true;
                break;
            case TOKEN_NUMBER:
                if (strcmp(tokens[i].value, "42") == 0) found_number = true;
                break;
            case TOKEN_COMMENT_BLOCK:
                if (strstr(tokens[i].value, "comment")) found_comment = true;
                break;
            case TOKEN_STRING:
                if (strstr(tokens[i].value, "string")) found_string = true;
                break;
            default:
                break;
        }
    }
    
    TEST_ASSERT(found_keyword, "Should tokenize 'int' as keyword");
    TEST_ASSERT(found_identifier, "Should tokenize 'x' as identifier");
    TEST_ASSERT(found_number, "Should tokenize '42' as number");
    TEST_ASSERT(found_comment, "Should tokenize comment block");
    TEST_ASSERT(found_string, "Should tokenize string literal");
    
    c_parser_free_tokens(tokens, token_count);
    return 1;
}

/*
 * Test keyword recognition functionality
 */
static int test_keyword_recognition(void) {
    // Test C keywords
    TEST_ASSERT(c_parser_is_c_keyword("int"), "Should recognize 'int' as keyword");
    TEST_ASSERT(c_parser_is_c_keyword("if"), "Should recognize 'if' as keyword");
    TEST_ASSERT(c_parser_is_c_keyword("while"), "Should recognize 'while' as keyword");
    TEST_ASSERT(c_parser_is_c_keyword("return"), "Should recognize 'return' as keyword");
    TEST_ASSERT(c_parser_is_c_keyword("static"), "Should recognize 'static' as keyword");
    
    // Test non-keywords
    TEST_ASSERT(!c_parser_is_c_keyword("main"), "Should not recognize 'main' as keyword");
    TEST_ASSERT(!c_parser_is_c_keyword("printf"), "Should not recognize 'printf' as keyword");
    TEST_ASSERT(!c_parser_is_c_keyword(""), "Should not recognize empty string as keyword");
    TEST_ASSERT(!c_parser_is_c_keyword(NULL), "Should handle NULL input gracefully");
    
    return 1;
}

/*
 * Test dangerous function detection
 */
static int test_dangerous_function_detection(void) {
    // Test dangerous functions
    TEST_ASSERT(c_parser_is_dangerous_function("strcpy"), "Should recognize 'strcpy' as dangerous");
    TEST_ASSERT(c_parser_is_dangerous_function("strcat"), "Should recognize 'strcat' as dangerous");
    TEST_ASSERT(c_parser_is_dangerous_function("sprintf"), "Should recognize 'sprintf' as dangerous");
    TEST_ASSERT(c_parser_is_dangerous_function("gets"), "Should recognize 'gets' as dangerous");
    TEST_ASSERT(c_parser_is_dangerous_function("strlen"), "Should recognize 'strlen' as dangerous in this context");
    
    // Test safe functions
//    TEST_ASSERT(!c_parser_is_dangerous_function("malloc"), "Should not recognize 'malloc' as dangerous in this context");
    TEST_ASSERT(!c_parser_is_dangerous_function(""), "Should handle empty string");
    TEST_ASSERT(!c_parser_is_dangerous_function(NULL), "Should handle NULL input");
    
    return 1;
}

/*
 * Test type keyword recognition
 */
static int test_type_keyword_recognition(void) {
    // Test basic types
    TEST_ASSERT(c_parser_is_type_keyword("int"), "Should recognize 'int' as type");
    TEST_ASSERT(c_parser_is_type_keyword("char"), "Should recognize 'char' as type");
    TEST_ASSERT(c_parser_is_type_keyword("float"), "Should recognize 'float' as type");
    TEST_ASSERT(c_parser_is_type_keyword("double"), "Should recognize 'double' as type");
    TEST_ASSERT(c_parser_is_type_keyword("void"), "Should recognize 'void' as type");
    
    // Test standard library types
    TEST_ASSERT(c_parser_is_type_keyword("size_t"), "Should recognize 'size_t' as type");
    TEST_ASSERT(c_parser_is_type_keyword("FILE"), "Should recognize 'FILE' as type");
    TEST_ASSERT(c_parser_is_type_keyword("bool"), "Should recognize 'bool' as type");
    
    // Test fixed-width types
    TEST_ASSERT(c_parser_is_type_keyword("uint32_t"), "Should recognize 'uint32_t' as type");
    TEST_ASSERT(c_parser_is_type_keyword("int64_t"), "Should recognize 'int64_t' as type");
    
    // Test non-types
    TEST_ASSERT(!c_parser_is_type_keyword("main"), "Should not recognize 'main' as type");
    TEST_ASSERT(!c_parser_is_type_keyword("printf"), "Should not recognize 'printf' as type");
    TEST_ASSERT(!c_parser_is_type_keyword(NULL), "Should handle NULL input");
    
    return 1;
}

// =============================================================================
// DOCUMENTATION ANALYSIS TESTS
// =============================================================================

/*
 * Test documentation detection for functions
 */
static int test_documentation_detection(void) {
    const char* test_content = create_simple_test_program();
    
    ParsedFile_t* parsed = c_parser_parse_content(test_content, "test.c");
    TEST_ASSERT(parsed != NULL, "Parser should successfully parse content");
    
    // The main function in our test has documentation
    bool has_docs = c_parser_has_documentation_for_function(parsed, "main");
    TEST_ASSERT(has_docs, "Should detect documentation for main function");
    
    // Test non-existent function
    bool no_docs = c_parser_has_documentation_for_function(parsed, "nonexistent");
    TEST_ASSERT(!no_docs, "Should not find documentation for non-existent function");
    
    c_parser_free_parsed_file(parsed);
    return 1;
}

/*
 * Test header file analysis
 */
static int test_header_file_analysis(void) {
    const char* test_content = create_simple_test_program();
    
    ParsedFile_t* parsed = c_parser_parse_content(test_content, "simple.c");
    TEST_ASSERT(parsed != NULL, "Parser should successfully parse content");
    
    // Debug: Check what the parser found
    LOG("DEBUG: Testing filename header detection");
    printf("Token count: %d\n", parsed->token_count);
    for (int i = 0; i < parsed->token_count && i < 10; i++) {
        printf("Token %d: %s (type: %s)\n", i, parsed->tokens[i].value, c_parser_token_type_name(parsed->tokens[i].type));
    }
    
    // Test filename header check - content contains "simple.c" so we should look for that
    bool has_filename_header = c_parser_has_proper_filename_header(parsed, "simple.c");
    printf("Has filename header: %s\n", has_filename_header ? "true" : "false");
    TEST_ASSERT(has_filename_header, "Should detect proper filename in header");
    
    // Test wisdom header check  
    bool has_wisdom_header = c_parser_has_proper_wisdom_header(parsed);
    TEST_ASSERT(has_wisdom_header, "Should detect wisdom header");
    
    // Test combined header check
    bool has_proper_headers = c_parser_has_proper_file_headers(parsed);
    TEST_ASSERT(has_proper_headers, "Should pass complete header validation");
    
    c_parser_free_parsed_file(parsed);
    return 1;
}

/*
 * Test documentation format validation
 */
static int test_documentation_format_validation(void) {
    const char* problematic_content = create_problematic_header_content();
    
    ParsedFile_t* parsed = c_parser_parse_content(problematic_content, "problematic.h");
    TEST_ASSERT(parsed != NULL, "Parser should successfully parse problematic content");
    
    // Should detect that function has documentation
    bool has_docs = c_parser_has_documentation_for_function(parsed, "check_documentation");
    TEST_ASSERT(has_docs, "Should detect that function has documentation");
    
    // Should detect that documentation format is wrong (multi-line description)
    bool proper_format = c_parser_has_proper_header_doc_format(parsed, "check_documentation");
    TEST_ASSERT(!proper_format, "Should detect improper documentation format with 'Also piss'");
    
    c_parser_free_parsed_file(parsed);
    return 1;
}

/*
 * Test function description extraction
 */
static int test_function_description_extraction(void) {
    const char* problematic_content = create_problematic_header_content();
    
    ParsedFile_t* parsed = c_parser_parse_content(problematic_content, "problematic.h");
    TEST_ASSERT(parsed != NULL, "Parser should successfully parse content");
    
    // Debug: Check what functions were found
    LOG("DEBUG: Function extraction test");
    printf("Function count: %d\n", parsed->function_count);
    for (int i = 0; i < parsed->function_count; i++) {
        printf("Function %d: %s\n", i, parsed->functions[i].name);
    }
    
    char* description = c_parser_extract_function_description(parsed, "check_documentation");
    printf("Extracted description: %s\n", description ? description : "NULL");
    TEST_ASSERT(description != NULL, "Should extract function description");
    TEST_ASSERT(strstr(description, "Check if a function has associated documentation") != NULL, 
                "Should extract first line of description");
    
    free(description);
    c_parser_free_parsed_file(parsed);
    return 1;
}

// =============================================================================
// DANGEROUS FUNCTION ANALYSIS TESTS
// =============================================================================

/*
 * Test dangerous function detection in parsed content
 */
static int test_dangerous_functions_in_content(void) {
    const char* dangerous_content = create_dangerous_functions_content();
    
    ParsedFile_t* parsed = c_parser_parse_content(dangerous_content, "dangerous.c");
    TEST_ASSERT(parsed != NULL, "Parser should successfully parse dangerous content");
    
    // Check that we can find dangerous function calls in the token stream
    bool found_strlen = false;
    bool found_strcpy = false;
    bool found_sprintf = false;
    
    for (int i = 0; i < parsed->token_count; i++) {
        if (parsed->tokens[i].type == TOKEN_IDENTIFIER) {
            if (strcmp(parsed->tokens[i].value, "strlen") == 0) found_strlen = true;
            if (strcmp(parsed->tokens[i].value, "strcpy") == 0) found_strcpy = true;
            if (strcmp(parsed->tokens[i].value, "sprintf") == 0) found_sprintf = true;
        }
    }
    
    TEST_ASSERT(found_strlen, "Should find strlen in token stream");
    TEST_ASSERT(found_strcpy, "Should find strcpy in token stream");
    TEST_ASSERT(found_sprintf, "Should find sprintf in token stream");
    
    c_parser_free_parsed_file(parsed);
    return 1;
}

// =============================================================================
// COMPLEXITY ANALYSIS TESTS
// =============================================================================

/*
 * Test function complexity analysis
 */
static int test_function_complexity_analysis(void) {
    const char* complex_function = 
        "/* complex.c - Function with some complexity */\n"
        "int complex_function(int x) {\n"
        "    if (x > 0) {\n"
        "        for (int i = 0; i < x; i++) {\n"
        "            if (i % 2 == 0) {\n"
        "                return i;\n"
        "            }\n"
        "        }\n"
        "        return x;\n"
        "    } else {\n"
        "        return -1;\n"
        "    }\n"
        "}\n";
    
    ParsedFile_t* parsed = c_parser_parse_content(complex_function, "complex.c");
    TEST_ASSERT(parsed != NULL, "Parser should parse complex function");
    
    ComplexityAnalysis_t analysis = c_parser_analyze_function_complexity(parsed, "complex_function");
    
    TEST_ASSERT(analysis.complexity_score > 0, "Should calculate non-zero complexity score");
    TEST_ASSERT(analysis.nesting_depth > 1, "Should detect nested structures");
    TEST_ASSERT(analysis.has_multiple_returns, "Should detect multiple return statements");
    TEST_ASSERT(analysis.branch_count > 0, "Should count branching statements");
    
    c_parser_free_parsed_file(parsed);
    return 1;
}

// =============================================================================
// UTILITY FUNCTION TESTS
// =============================================================================

/*
 * Test filename extraction utility
 */
static int test_filename_extraction(void) {
    // Test Unix-style paths
    const char* filename1 = c_parser_extract_filename("/home/user/project/test.c");
    TEST_ASSERT(filename1 != NULL, "Should extract filename from Unix path");
    TEST_ASSERT(strcmp(filename1, "test.c") == 0, "Should extract correct filename");
    
    // Test Windows-style paths
    const char* filename2 = c_parser_extract_filename("C:\\Users\\test\\project\\main.c");
    TEST_ASSERT(filename2 != NULL, "Should extract filename from Windows path");
    TEST_ASSERT(strcmp(filename2, "main.c") == 0, "Should extract correct filename");
    
    // Test filename without path
    const char* filename3 = c_parser_extract_filename("simple.c");
    TEST_ASSERT(filename3 != NULL, "Should handle filename without path");
    TEST_ASSERT(strcmp(filename3, "simple.c") == 0, "Should return same filename");
    
    // Test NULL input
    const char* filename4 = c_parser_extract_filename(NULL);
    TEST_ASSERT(filename4 == NULL, "Should handle NULL input gracefully");
    
    return 1;
}

/*
 * Test token type name utility
 */
static int test_token_type_names(void) {
    TEST_ASSERT(strcmp(c_parser_token_type_name(TOKEN_KEYWORD), "KEYWORD") == 0, 
                "Should return correct name for KEYWORD token");
    TEST_ASSERT(strcmp(c_parser_token_type_name(TOKEN_IDENTIFIER), "IDENTIFIER") == 0,
                "Should return correct name for IDENTIFIER token");
    TEST_ASSERT(strcmp(c_parser_token_type_name(TOKEN_STRING), "STRING") == 0,
                "Should return correct name for STRING token");
    TEST_ASSERT(strcmp(c_parser_token_type_name(TOKEN_COMMENT_BLOCK), "COMMENT_BLOCK") == 0,
                "Should return correct name for COMMENT_BLOCK token");
    
    // Test invalid token type
    const char* invalid_name = c_parser_token_type_name((TokenType_t)999);
    TEST_ASSERT(strcmp(invalid_name, "INVALID") == 0, "Should handle invalid token type");
    
    return 1;
}

// =============================================================================
// FIXME REGRESSION TESTS - Test specific bugs found in FIXME comments
// =============================================================================

/*
 * Test for FIXME: is_function_definition incorrectly identifies function calls as definitions
 */
static int test_function_definition_vs_call_detection(void) {
    const char* code_with_calls = 
        "int main(void) {\n"
        "    printf(\"Hello World\");\n"
        "    strlen(\"test\");\n"
        "    return 0;\n"
        "}\n";
    
    ParsedFile_t* parsed = c_parser_parse_content(code_with_calls, "test_calls.c");
    TEST_ASSERT(parsed != NULL, "Parser should parse code with function calls");
    
    LOG("DEBUG: Testing function call vs definition detection");
    printf("Function count: %d\n", parsed->function_count);
    for (int i = 0; i < parsed->function_count; i++) {
        printf("Function %d: %s (line %d)\n", i, parsed->functions[i].name, parsed->functions[i].line_number);
    }
    
    // Debug: Let's see what tokens we have
    printf("Tokens:\n");
    for (int i = 0; i < parsed->token_count && i < 20; i++) {
        printf("Token %d: '%s' (type: %s, line: %d)\n", i, parsed->tokens[i].value, 
               c_parser_token_type_name(parsed->tokens[i].type), parsed->tokens[i].line);
    }
    
    // Should only detect main function, NOT printf or strlen calls
    TEST_ASSERT(parsed->function_count == 1, "Should detect exactly one function definition (main)");
    TEST_ASSERT(strcmp(parsed->functions[0].name, "main") == 0, "Should detect main function");
    
    // Verify printf and strlen are NOT detected as function definitions
    bool found_printf = false, found_strlen = false;
    for (int i = 0; i < parsed->function_count; i++) {
        if (strcmp(parsed->functions[i].name, "printf") == 0) found_printf = true;
        if (strcmp(parsed->functions[i].name, "strlen") == 0) found_strlen = true;
    }
    TEST_ASSERT(!found_printf, "Should NOT detect printf call as function definition");
    TEST_ASSERT(!found_strlen, "Should NOT detect strlen call as function definition");
    
    c_parser_free_parsed_file(parsed);
    return 1;
}

/*
 * Test for FIXME: c_parser_has_proper_filename_header can't detect block comments
 */
static int test_filename_header_detection_specific(void) {
    const char* code_with_header = 
        "/* test_file.c - Test file for header detection */\n"
        "// INSERT WISDOM HERE\n"
        "\n"
        "int main(void) { return 0; }\n";
    
    ParsedFile_t* parsed = c_parser_parse_content(code_with_header, "test_file.c");
    TEST_ASSERT(parsed != NULL, "Parser should parse code with header");
    
    LOG("DEBUG: Testing specific filename header detection");
    printf("Token count: %d\n", parsed->token_count);
    for (int i = 0; i < 3 && i < parsed->token_count; i++) {
        printf("Token %d: '%s' (type: %s)\n", i, parsed->tokens[i].value, c_parser_token_type_name(parsed->tokens[i].type));
    }
    
    // Test the specific function that's failing
    bool has_header = c_parser_has_proper_filename_header(parsed, "test_file.c");
    printf("Has filename header result: %s\n", has_header ? "true" : "false");
    
    TEST_ASSERT(has_header, "Should detect filename in block comment header");
    
    c_parser_free_parsed_file(parsed);
    return 1;
}

/*
 * Test for FIXME: c_parser_extract_function_description returns NULL
 */
static int test_function_description_extraction_specific(void) {
    const char* code_with_doc = 
        "/*\n"
        " * Calculate the sum of two numbers\n"
        " *\n"
        " * Returns: sum of a and b\n"
        " */\n"
        "int add(int a, int b) {\n"
        "    return a + b;\n"
        "}\n";
    
    ParsedFile_t* parsed = c_parser_parse_content(code_with_doc, "test_doc.c");
    TEST_ASSERT(parsed != NULL, "Parser should parse code with documentation");
    
    LOG("DEBUG: Testing specific function description extraction");
    printf("Function count: %d\n", parsed->function_count);
    for (int i = 0; i < parsed->function_count; i++) {
        printf("Function %d: %s\n", i, parsed->functions[i].name);
    }
    
    // Test the specific function that's failing
    char* description = c_parser_extract_function_description(parsed, "add");
    printf("Extracted description: '%s'\n", description ? description : "NULL");
    
    TEST_ASSERT(description != NULL, "Should extract function description");
    TEST_ASSERT(strstr(description, "Calculate the sum of two numbers") != NULL, 
                "Should extract correct description text");
    
    if (description) free(description);
    c_parser_free_parsed_file(parsed);
    return 1;
}

// =============================================================================
// DEBUG HUNTING TESTS - Stress tests to find bugs and security issues
// =============================================================================

/*
 * Test 1: Buffer overflow attack with extremely long identifiers and comments
 */
static int test_buffer_overflow_attack(void) {
    // Create a very long identifier (potential buffer overflow)
    char long_identifier[2000];
    memset(long_identifier, 'A', 1999);
    long_identifier[1999] = '\0';
    
    // Create content with very long function name and comment
    char malicious_content[4000];
    snprintf(malicious_content, sizeof(malicious_content),
        "/* %s - Very long comment that might overflow buffers */\n"
        "int %s(void) {\n"
        "    return 0;\n"
        "}\n", long_identifier, long_identifier);
    
    LOG("DEBUG: Testing buffer overflow with long identifiers");
    
    ParsedFile_t* parsed = c_parser_parse_content(malicious_content, "overflow_test.c");
    TEST_ASSERT(parsed != NULL, "Parser should handle very long identifiers gracefully");
    
    if (parsed) {
        printf("Function count: %d\n", parsed->function_count);
        if (parsed->function_count > 0) {
            printf("Function name length: %zu\n", strlen(parsed->functions[0].name));
            TEST_ASSERT(strlen(parsed->functions[0].name) < 2000, "Function name should be truncated safely");
        }
        c_parser_free_parsed_file(parsed);
    }
    
    return 1;
}

/*
 * Test 2: Malformed comment injection attack
 */
static int test_malformed_comment_injection(void) {
    const char* malicious_code = 
        "/* Innocent comment */ /* */ /*\n"
        " * This looks like a comment but has malformed structure\n"
        " * /* nested comment /* deeper nesting */\n"
        " * Potential code injection: system(\"rm -rf /\");\n"
        " */\n"
        "int main(void) {\n"
        "    /* Another /* nested /* comment */ */ */\n"
        "    return 0;\n"
        "}\n";
    
    LOG("DEBUG: Testing malformed comment injection");
    
    ParsedFile_t* parsed = c_parser_parse_content(malicious_code, "injection_test.c");
    TEST_ASSERT(parsed != NULL, "Parser should handle malformed nested comments");
    
    if (parsed) {
        printf("Token count: %d\n", parsed->token_count);
        // Check that parser doesn't crash and produces reasonable output
        bool found_main = false;
        for (int i = 0; i < parsed->function_count; i++) {
            if (strcmp(parsed->functions[i].name, "main") == 0) {
                found_main = true;
                break;
            }
        }
        TEST_ASSERT(found_main, "Should still detect main function despite malformed comments");
        c_parser_free_parsed_file(parsed);
    }
    
    return 1;
}

/*
 * Test 3: Unicode and special character attack
 */
static int test_unicode_special_chars_attack(void) {
    const char* unicode_code = 
        "/* тест.c - Unicode filename test */\n"
        "// INSERT WISDOM HERE\n"
        "\n"
        "// Function with unicode: héllo_wörld\n"
        "int函数(void) {\n"  // Chinese characters in function name
        "    char* str = \"Hello\\x00\\xFF\\x01World\";\n"  // Null bytes and high bytes
        "    return 0;\n"
        "}\n"
        "\n"
        "// Function with control characters\n"
        "int test\\x08\\x09function(void) {\n"  // Backspace and tab in name
        "    return 1;\n"
        "}\n";
    
    LOG("DEBUG: Testing Unicode and special character handling");
    
    ParsedFile_t* parsed = c_parser_parse_content(unicode_code, "unicode_test.c");
    TEST_ASSERT(parsed != NULL, "Parser should handle Unicode and special characters");
    
    if (parsed) {
        printf("Function count: %d\n", parsed->function_count);
        printf("Token count: %d\n", parsed->token_count);
        
        // Verify parser doesn't crash on special characters
        for (int i = 0; i < parsed->function_count; i++) {
            printf("Function %d name: '%s'\n", i, parsed->functions[i].name);
            TEST_ASSERT(parsed->functions[i].name != NULL, "Function name should not be NULL");
        }
        
        c_parser_free_parsed_file(parsed);
    }
    
    return 1;
}

/*
 * Test 4: Memory exhaustion attack with deeply nested structures
 */
static int test_memory_exhaustion_attack(void) {
    // Create deeply nested function calls and comments
    char huge_content[8000];
    strcpy(huge_content, "/* memory_test.c - Memory exhaustion test */\n");
    
    // Add many nested function calls to stress the parser
    strcat(huge_content, "int main(void) {\n");
    for (int i = 0; i < 50; i++) {
        char line[100];
        snprintf(line, sizeof(line), "    func%d(func%d(func%d()));\n", i, i+1, i+2);
        strcat(huge_content, line);
    }
    strcat(huge_content, "    return 0;\n}\n");
    
    // Add many function declarations
    for (int i = 0; i < 20; i++) {
        char func_decl[200];
        snprintf(func_decl, sizeof(func_decl), 
            "/* Function %d with long comment */\nint func%d(int param%d);\n", i, i, i);
        strcat(huge_content, func_decl);
    }
    
    LOG("DEBUG: Testing memory exhaustion with many nested calls");
    
    ParsedFile_t* parsed = c_parser_parse_content(huge_content, "memory_test.c");
    TEST_ASSERT(parsed != NULL, "Parser should handle large content without memory exhaustion");
    
    if (parsed) {
        printf("Function count: %d\n", parsed->function_count);
        printf("Token count: %d\n", parsed->token_count);
        TEST_ASSERT(parsed->function_count < 100, "Function count should be reasonable");
        TEST_ASSERT(parsed->token_count < 10000, "Token count should not be excessive");
        c_parser_free_parsed_file(parsed);
    }
    
    return 1;
}

/*
 * Test for one-line documentation format bug - proper format incorrectly flagged
 */
static int test_one_line_documentation_format_bug(void) {
    LOG("DEBUG: Testing one-line documentation format detection");
    
    // This should be considered PROPER format according to the rules
    const char* proper_format_code = 
        "/*\n"
        " * Check if a word is a C keyword\n"
        " *\n"
        " * Returns: true if word is keyword, false otherwise\n"
        " */\n"
        "bool c_parser_is_c_keyword(const char* word) {\n"
        "    return false;\n"
        "}\n";
    
    ParsedFile_t* parsed = c_parser_parse_content(proper_format_code, "test_format.c");
    TEST_ASSERT(parsed != NULL, "Parser should parse proper format code");
    
    printf("Function count: %d\n", parsed->function_count);
    if (parsed->function_count > 0) {
        printf("Function name: %s, line: %d\n", parsed->functions[0].name, parsed->functions[0].line_number);
    }
    
    // Debug: Let's see what comment tokens were found
    printf("Comment tokens found:\n");
    for (int i = 0; i < parsed->token_count; i++) {
        if (parsed->tokens[i].type == TOKEN_COMMENT_BLOCK) {
            printf("Comment token %d (line %d):\n", i, parsed->tokens[i].line);
            printf("'%s'\n", parsed->tokens[i].value);
            printf("Length: %zu\n", strlen(parsed->tokens[i].value));
        }
    }
    
    // This should return TRUE because it follows the one-line format:
    // 1. One line description: "Check if a word is a C keyword"
    // 2. Blank line: " *"
    // 3. Additional details: "Returns: true if word is keyword, false otherwise"
    bool has_proper_format = c_parser_has_proper_header_doc_format(parsed, "c_parser_is_c_keyword");
    printf("Has proper format result: %s\n", has_proper_format ? "true" : "false");
    
    TEST_ASSERT(has_proper_format, "Should detect proper one-line documentation format");
    
    c_parser_free_parsed_file(parsed);
    return 1;
}

/*
 * Test for the "piss" detection bug - inappropriate content should be caught
 */
static int test_inappropriate_content_detection_bug(void) {
    LOG("DEBUG: Testing inappropriate content detection in header documentation");
    
    // This content mirrors the actual bug in include/c_parser.h line 283
    const char* header_with_piss = 
        "/* test_header.h - Header with inappropriate content */\n"
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
        " * `func_name` - Function name to check (must be null-terminated)\n"
        " *\n"
        " * `bool` - true if function has documentation, false otherwise\n"
        " */\n"
        "bool c_parser_has_documentation_for_function(ParsedFile_t* parsed, const char* func_name);\n"
        "\n"
        "#endif\n";
    
    ParsedFile_t* parsed = c_parser_parse_content(header_with_piss, "test_header.h");
    TEST_ASSERT(parsed != NULL, "Parser should successfully parse header content");
    
    printf("Function count: %d\n", parsed->function_count);
    for (int i = 0; i < parsed->function_count; i++) {
        printf("Function %d: %s (line %d)\n", i, parsed->functions[i].name, parsed->functions[i].line_number);
    }
    
    // Debug: Check what comment tokens were found
    printf("Comment tokens found:\n");
    for (int i = 0; i < parsed->token_count; i++) {
        if (parsed->tokens[i].type == TOKEN_COMMENT_BLOCK) {
            printf("Comment token %d (line %d):\n", i, parsed->tokens[i].line);
            printf("Content: '%s'\n", parsed->tokens[i].value);
            
            // Check if "piss" is in the raw comment content
            if (strstr(parsed->tokens[i].value, "piss")) {
                printf("*** FOUND 'piss' in comment token!\n");
            }
        }
    }
    
    // Test that function has documentation
    bool has_docs = c_parser_has_documentation_for_function(parsed, "c_parser_has_documentation_for_function");
    printf("Has documentation: %s\n", has_docs ? "true" : "false");
    TEST_ASSERT(has_docs, "Function should have documentation detected");
    
    // CRITICAL TEST: The format check should return FALSE due to "piss" content
    bool proper_format = c_parser_has_proper_header_doc_format(parsed, "c_parser_has_documentation_for_function");
    printf("Has proper format: %s\n", proper_format ? "true" : "false");
    
    // This is the main bug - this should return FALSE because of inappropriate content
    TEST_ASSERT(!proper_format, "Should detect inappropriate content 'piss' and return false");
    
    c_parser_free_parsed_file(parsed);
    return 1;
}

/*
 * Test FIXED content detection - should also be flagged as inappropriate
 */
static int test_fixed_comment_detection_bug(void) {
    LOG("DEBUG: Testing FIXED comment detection in header documentation");
    
    // Test content with FIXED: comment which should also be inappropriate
    const char* header_with_fixed = 
        "/* test_fixed.h - Header with FIXED comment */\n"
        "// INSERT WISDOM HERE\n"
        "\n"
        "/*\n"
        " * Parse C source code content\n"
        " * FIXED: Memory leak in tokenizer\n"
        " *\n"
        " * `content` - C source code content to parse\n"
        " *\n"
        " * `ParsedFile_t*` - Complete parsed file structure\n"
        " */\n"
        "ParsedFile_t* c_parser_parse_content(const char* content, const char* file_path);\n";
    
    ParsedFile_t* parsed = c_parser_parse_content(header_with_fixed, "test_fixed.h");
    TEST_ASSERT(parsed != NULL, "Parser should successfully parse content with FIXED comment");
    
    printf("Function count: %d\n", parsed->function_count);
    for (int i = 0; i < parsed->function_count; i++) {
        printf("Function %d: %s\n", i, parsed->functions[i].name);
    }
    
    // Debug: Check comment content
    printf("Comment tokens:\n");
    for (int i = 0; i < parsed->token_count; i++) {
        if (parsed->tokens[i].type == TOKEN_COMMENT_BLOCK) {
            printf("Comment: '%s'\n", parsed->tokens[i].value);
            if (strstr(parsed->tokens[i].value, "FIXED:")) {
                printf("*** FOUND 'FIXED:' in comment token!\n");
            }
        }
    }
    
    // Test documentation detection
    bool has_docs = c_parser_has_documentation_for_function(parsed, "c_parser_parse_content");
    TEST_ASSERT(has_docs, "Function should have documentation detected");
    
    // Test format validation - should fail due to FIXED: content
    bool proper_format = c_parser_has_proper_header_doc_format(parsed, "c_parser_parse_content");
    printf("Has proper format: %s\n", proper_format ? "true" : "false");
    
    TEST_ASSERT(!proper_format, "Should detect inappropriate content 'FIXED:' and return false");
    
    c_parser_free_parsed_file(parsed);
    return 1;
}

/*
 * Test 5: Null pointer and edge case attack
 */
static int test_null_pointer_edge_cases(void) {
    LOG("DEBUG: Testing null pointers and edge cases");
    
    // Test NULL inputs
    ParsedFile_t* parsed1 = c_parser_parse_content(NULL, "test.c");
    TEST_ASSERT(parsed1 == NULL, "Should handle NULL content gracefully");
    
    ParsedFile_t* parsed2 = c_parser_parse_content("int main(){}", NULL);
    TEST_ASSERT(parsed2 != NULL, "Should handle NULL filename by using default");
    if (parsed2) {
        TEST_ASSERT(parsed2->file_path != NULL, "Should have default file path");
        c_parser_free_parsed_file(parsed2);
    }
    
    // Test empty content
    ParsedFile_t* parsed3 = c_parser_parse_content("", "empty.c");
    TEST_ASSERT(parsed3 != NULL, "Should handle empty content");
    if (parsed3) {
        TEST_ASSERT(parsed3->function_count == 0, "Empty content should have no functions");
        TEST_ASSERT(parsed3->token_count == 0, "Empty content should have no tokens");
        c_parser_free_parsed_file(parsed3);
    }
    
    // Test malformed function calls
    const char* broken_code = 
        "int main(\n"  // Missing closing paren
        "    printf(\"Hello\n"  // Missing closing quote and paren
        "    return 0\n"  // Missing semicolon
        "// Missing closing brace\n";
    
    ParsedFile_t* parsed4 = c_parser_parse_content(broken_code, "broken.c");
    TEST_ASSERT(parsed4 != NULL, "Should handle malformed code without crashing");
    if (parsed4) {
        printf("Malformed code - Function count: %d, Token count: %d\n", 
               parsed4->function_count, parsed4->token_count);
        c_parser_free_parsed_file(parsed4);
    }
    
    // Test utility functions with edge cases
    TEST_ASSERT(c_parser_is_c_keyword(NULL) == false, "Should handle NULL keyword check");
    TEST_ASSERT(c_parser_is_dangerous_function(NULL) == false, "Should handle NULL function check");
    TEST_ASSERT(c_parser_extract_filename(NULL) == NULL, "Should handle NULL path extraction");
    
    return 1;
}

/*
 * Test unsafe strcmp detection with dString_t->str patterns
 */
static int test_unsafe_strcmp_detection(void) {
    LOG("DEBUG: Testing unsafe strcmp detection with dString_t patterns");
    
    // Create test content with unsafe strcmp patterns
    const char* unsafe_strcmp_code = 
        "/* test_strcmp.c - Test file for unsafe strcmp detection */\n"
        "// INSERT WISDOM HERE\n"
        "\n"
        "#include <string.h>\n"
        "\n"
        "typedef struct {\n"
        "    char* str;\n"
        "    int len;\n"
        "} dString_t;\n"
        "\n"
        "/*\n"
        " * Test function with unsafe strcmp usage\n"
        " */\n"
        "void test_unsafe_strcmp(dString_t* item, dString_t* other) {\n"
        "    // Pattern 1: dString vs C-string (should trigger detection)\n"
        "    if (strcmp(item->str, \"some_id\") == 0) {\n"
        "        // This should be detected\n"
        "    }\n"
        "    \n"
        "    // Pattern 2: dString vs dString (should trigger detection)\n"
        "    if (strcmp(item->str, other->str) == 0) {\n"
        "        // This should also be detected\n"
        "    }\n"
        "    \n"
        "    // Safe patterns (should NOT trigger)\n"
        "    if (strcmp(\"hello\", \"world\") == 0) {\n"
        "        // Regular C strings - safe\n"
        "    }\n"
        "}\n";
    
    ParsedFile_t* parsed = c_parser_parse_content(unsafe_strcmp_code, "test_strcmp.c");
    TEST_ASSERT(parsed != NULL, "Parser should successfully parse strcmp test content");
    
    printf("Function count: %d\n", parsed->function_count);
    printf("Token count: %d\n", parsed->token_count);
    
    // Test the unsafe strcmp detection function
    UnsafeStrcmpUsage_t* usages = NULL;
    int usage_count = 0;
    
    bool found_unsafe = c_parser_detect_unsafe_strcmp_dstring_usage(parsed, &usages, &usage_count);
    
    printf("Found unsafe strcmp usages: %s\n", found_unsafe ? "true" : "false");
    printf("Usage count: %d\n", usage_count);
    
    TEST_ASSERT(found_unsafe, "Should detect unsafe strcmp usage patterns");
    TEST_ASSERT(usage_count == 2, "Should detect exactly 2 unsafe strcmp patterns");
    
    if (usages && usage_count > 0) {
        for (int i = 0; i < usage_count; i++) {
            printf("Usage %d: line %d, column %d\n", i, usages[i].line, usages[i].column);
            printf("  dString vs C-string: %s\n", usages[i].is_dstring_vs_cstring ? "true" : "false");
            printf("  dString vs dString: %s\n", usages[i].is_dstring_vs_dstring ? "true" : "false");
        }
        
        // Verify the patterns are correctly identified
        bool found_dstring_vs_cstring = false;
        bool found_dstring_vs_dstring = false;
        
        for (int i = 0; i < usage_count; i++) {
            if (usages[i].is_dstring_vs_cstring) found_dstring_vs_cstring = true;
            if (usages[i].is_dstring_vs_dstring) found_dstring_vs_dstring = true;
        }
        
        TEST_ASSERT(found_dstring_vs_cstring, "Should detect dString vs C-string pattern");
        TEST_ASSERT(found_dstring_vs_dstring, "Should detect dString vs dString pattern");
        
        free(usages);
    }
    
    c_parser_free_parsed_file(parsed);
    return 1;
}

/*
 * Test that safe strcmp patterns are NOT detected as unsafe
 */
static int test_safe_strcmp_not_detected(void) {
    LOG("DEBUG: Testing that safe strcmp patterns are not flagged as unsafe");
    
    // Create test content with only safe strcmp patterns
    const char* safe_strcmp_code = 
        "/* safe_strcmp.c - Test file with only safe strcmp usage */\n"
        "// INSERT WISDOM HERE\n"
        "\n"
        "#include <string.h>\n"
        "\n"
        "/*\n"
        " * Test function with only safe strcmp usage\n"
        " */\n"
        "void test_safe_strcmp(void) {\n"
        "    char* str1 = \"hello\";\n"
        "    char* str2 = \"world\";\n"
        "    \n"
        "    // All safe patterns - should NOT trigger detection\n"
        "    if (strcmp(str1, str2) == 0) {\n"
        "        // Regular C strings - safe\n"
        "    }\n"
        "    \n"
        "    if (strcmp(\"literal1\", \"literal2\") == 0) {\n"
        "        // String literals - safe\n"
        "    }\n"
        "    \n"
        "    if (strcmp(str1, \"literal\") == 0) {\n"
        "        // C string vs literal - safe\n"
        "    }\n"
        "}\n";
    
    ParsedFile_t* parsed = c_parser_parse_content(safe_strcmp_code, "safe_strcmp.c");
    TEST_ASSERT(parsed != NULL, "Parser should successfully parse safe strcmp content");
    
    printf("Function count: %d\n", parsed->function_count);
    printf("Token count: %d\n", parsed->token_count);
    
    // Test the unsafe strcmp detection function
    UnsafeStrcmpUsage_t* usages = NULL;
    int usage_count = 0;
    
    bool found_unsafe = c_parser_detect_unsafe_strcmp_dstring_usage(parsed, &usages, &usage_count);
    
    printf("Found unsafe strcmp usages: %s\n", found_unsafe ? "true" : "false");
    printf("Usage count: %d\n", usage_count);
    
    TEST_ASSERT(!found_unsafe, "Should NOT detect any unsafe strcmp usage in safe code");
    TEST_ASSERT(usage_count == 0, "Should find zero unsafe strcmp patterns");
    
    if (usages) {
        free(usages);
    }
    
    c_parser_free_parsed_file(parsed);
    return 1;
}

// =============================================================================
// MAIN TEST RUNNER
// =============================================================================

int main(void) {
    TEST_SUITE_START("C Parser Basic Tests");
    
    // Basic parsing functionality
    RUN_TEST(test_basic_content_parsing);
    RUN_TEST(test_tokenization);
    
    // Language recognition
    RUN_TEST(test_keyword_recognition);
    RUN_TEST(test_dangerous_function_detection);
    RUN_TEST(test_type_keyword_recognition);
    
    // Documentation analysis
    RUN_TEST(test_documentation_detection);
    RUN_TEST(test_header_file_analysis);
    RUN_TEST(test_documentation_format_validation);
    RUN_TEST(test_function_description_extraction);
    
    // Safety analysis
    RUN_TEST(test_dangerous_functions_in_content);
    
    // Complexity analysis
    RUN_TEST(test_function_complexity_analysis);
    
    // Utility functions
    RUN_TEST(test_filename_extraction);
    RUN_TEST(test_token_type_names);
    
    // FIXME regression tests
    RUN_TEST(test_function_definition_vs_call_detection);
    RUN_TEST(test_filename_header_detection_specific);
    RUN_TEST(test_function_description_extraction_specific);
    
    // Debug hunting tests - stress testing for bugs and security
    RUN_TEST(test_buffer_overflow_attack);
    RUN_TEST(test_malformed_comment_injection);
    RUN_TEST(test_unicode_special_chars_attack);
    RUN_TEST(test_memory_exhaustion_attack);
    RUN_TEST(test_one_line_documentation_format_bug);
    RUN_TEST(test_null_pointer_edge_cases);
    
    // Bug reproduction tests
    RUN_TEST(test_inappropriate_content_detection_bug);
    RUN_TEST(test_fixed_comment_detection_bug);
    
    // Unsafe strcmp detection tests
    RUN_TEST(test_unsafe_strcmp_detection);
    RUN_TEST(test_safe_strcmp_not_detected);
    
    TEST_SUITE_END();
}
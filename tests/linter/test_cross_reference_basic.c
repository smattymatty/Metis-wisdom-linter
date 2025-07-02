/* test_cross_reference_basic.c - Basic tests for cross-reference validation */
// INSERT WISDOM HERE

#include "tests.h"
#include "cross_reference.h"
#include "metis_linter.h"
#include "c_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <libgen.h>
#include <errno.h>

#define LOG(msg) printf("%s | File: %s, Line: %d\n", msg, __FILE__, __LINE__)

// Global test counters required by tests.h framework
int total_tests = 0;
int tests_passed = 0;
int tests_failed = 0;

// =============================================================================
// TEST HELPER FUNCTIONS
// =============================================================================


/*
 * Create a temporary test file with specified content
 */
static char* create_temp_test_file(const char* filename, const char* content) {
    char temp_path[512];
    snprintf(temp_path, sizeof(temp_path), "%s", filename);  // Use full path as given
    
    printf("DEBUG: Attempting to create file: %s\n", temp_path);
    
    // Create parent directories if they don't exist
    char* path_copy = strdup(temp_path);
    if (path_copy) {
        char* dir = dirname(path_copy);
        if (dir && strcmp(dir, ".") != 0) {
            printf("DEBUG: Creating directory: %s\n", dir);
            char mkdir_cmd[1024];
            snprintf(mkdir_cmd, sizeof(mkdir_cmd), "mkdir -p %s", dir);
            system(mkdir_cmd);
        }
        free(path_copy);
    }
    
    FILE* file = fopen(temp_path, "w");
    if (!file) {
        printf("DEBUG: Failed to create file %s: %s\n", temp_path, strerror(errno));
        perror("Failed to create temporary test file");
        return NULL;
    }
    
    fputs(content, file);
    fclose(file);
    
    printf("DEBUG: Successfully created file: %s\n", temp_path);
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
 * Create a temporary directory for testing
 */
static char* create_temp_test_directory(const char* dirname) {
    char temp_dir[512];
    snprintf(temp_dir, sizeof(temp_dir), "/tmp/%s", dirname);
    mkdir(temp_dir, 0755);
    return strdup(temp_dir);
}

/*
 * Clean up temporary test directory
 */
static void cleanup_temp_directory(char* dir_path) {
    if (dir_path) {
        rmdir(dir_path);
        free(dir_path);
    }
}

// =============================================================================
// TEST CONTENT GENERATORS
// =============================================================================

/*
 * Create a simple header file content
 */
static const char* create_simple_header_content(void) {
    return "/* simple.h - Simple test header */\n"
           "// INSERT WISDOM HERE\n"
           "\n"
           "#ifndef SIMPLE_H\n"
           "#define SIMPLE_H\n"
           "\n"
           "/*\n"
           " * Add two numbers together\n"
           " *\n"
           " * `a` - First number\n"
           " * `b` - Second number\n"
           " *\n"
           " * `int` - Sum of a and b\n"
           " */\n"
           "int add_numbers(int a, int b);\n"
           "\n"
           "/*\n"
           " * Multiply two numbers\n"
           " *\n"
           " * `x` - First number\n"
           " * `y` - Second number\n"
           " *\n"
           " * `int` - Product of x and y\n"
           " */\n"
           "int multiply_numbers(int x, int y);\n"
           "\n"
           "#endif\n";
}

/*
 * Create matching implementation file content
 */
static const char* create_matching_impl_content(void) {
    return "/* simple.c - Simple test implementation */\n"
           "// INSERT WISDOM HERE\n"
           "\n"
           "#include \"simple.h\"\n"
           "\n"
           "/*\n"
           " * Add two numbers together\n"
           " */\n"
           "int add_numbers(int a, int b) {\n"
           "    return a + b;\n"
           "}\n"
           "\n"
           "/*\n"
           " * Multiply two numbers\n"
           " */\n"
           "int multiply_numbers(int x, int y) {\n"
           "    return x * y;\n"
           "}\n";
}

/*
 * Create implementation with missing function
 */
static const char* create_missing_impl_content(void) {
    return "/* missing.c - Implementation missing a function */\n"
           "// INSERT WISDOM HERE\n"
           "\n"
           "#include \"simple.h\"\n"
           "\n"
           "/*\n"
           " * Add two numbers together\n"
           " */\n"
           "int add_numbers(int a, int b) {\n"
           "    return a + b;\n"
           "}\n"
           "\n"
           "// multiply_numbers is declared in header but not implemented here\n";
}

/*
 * Create implementation with extra function
 */
static const char* create_extra_impl_content(void) {
    return "/* extra.c - Implementation with extra function */\n"
           "// INSERT WISDOM HERE\n"
           "\n"
           "#include \"simple.h\"\n"
           "\n"
           "/*\n"
           " * Add two numbers together\n"
           " */\n"
           "int add_numbers(int a, int b) {\n"
           "    return a + b;\n"
           "}\n"
           "\n"
           "/*\n"
           " * Multiply two numbers\n"
           " */\n"
           "int multiply_numbers(int x, int y) {\n"
           "    return x * y;\n"
           "}\n"
           "\n"
           "/*\n"
           " * Subtract two numbers (not in header)\n"
           " */\n"
           "int subtract_numbers(int a, int b) {\n"
           "    return a - b;\n"
           "}\n";
}

/*
 * Create implementation with signature mismatch
 */
static const char* create_mismatch_impl_content(void) {
    return "/* mismatch.c - Implementation with signature mismatch */\n"
           "// INSERT WISDOM HERE\n"
           "\n"
           "#include \"simple.h\"\n"
           "\n"
           "/*\n"
           " * Add two numbers together\n"
           " */\n"
           "int add_numbers(int a, int b) {\n"
           "    return a + b;\n"
           "}\n"
           "\n"
           "/*\n"
           " * Multiply two numbers - but with different signature\n"
           " */\n"
           "float multiply_numbers(float x, float y) {\n"
           "    return x * y;\n"
           "}\n";
}

/*
 * Create header for file discovery tests
 */
static const char* create_discovery_header_content(void) {
    return "/* discovery.h - Header for discovery tests */\n"
           "// INSERT WISDOM HERE\n"
           "\n"
           "#ifndef DISCOVERY_H\n"
           "#define DISCOVERY_H\n"
           "\n"
           "/*\n"
           " * Test function for discovery\n"
           " */\n"
           "void test_function(void);\n"
           "\n"
           "#endif\n";
}

/*
 * Create implementation for file discovery tests
 */
static const char* create_discovery_impl_content(void) {
    return "/* discovery.c - Implementation for discovery tests */\n"
           "// INSERT WISDOM HERE\n"
           "\n"
           "#include \"discovery.h\"\n"
           "\n"
           "/*\n"
           " * Test function for discovery\n"
           " */\n"
           "void test_function(void) {\n"
           "    // Empty implementation\n"
           "}\n";
}

// =============================================================================
// BASIC CROSS-REFERENCE TESTS
// =============================================================================

/*
 * Test file discovery functionality
 */
static int test_header_file_discovery(void) {
    LOG("Testing header file discovery");
    
    // Create temporary directory structure
    char* temp_dir = create_temp_test_directory("xref_discovery_test");
    char* include_dir = create_temp_test_directory("xref_discovery_test/include");
    
    // Create header file in include directory
    char header_path[512];
    snprintf(header_path, sizeof(header_path), "%s/discovery.h", include_dir);
    char* header_file = create_temp_test_file(header_path, create_discovery_header_content());
    
    // Create implementation file in temp directory
    char impl_path[512];
    snprintf(impl_path, sizeof(impl_path), "%s/discovery.c", temp_dir);
    char* impl_file = create_temp_test_file(impl_path, create_discovery_impl_content());
    
    // Test header discovery from implementation path
    char* found_header = cross_reference_find_header_file(impl_path);
    
    printf("Looking for header for: %s\n", impl_path);
    printf("Found header: %s\n", found_header ? found_header : "NULL");
    
    // Should find the header file
    TEST_ASSERT(found_header != NULL, "Should find corresponding header file");
    if (found_header) {
        TEST_ASSERT(strstr(found_header, "discovery.h") != NULL, "Should find correct header file");
    }
    
    // Cleanup
    cleanup_temp_file(header_file);
    cleanup_temp_file(impl_file);
    unlink(header_path);
    unlink(impl_path);
    cleanup_temp_directory(include_dir);
    cleanup_temp_directory(temp_dir);
    if (found_header) free(found_header);
    
    return 1;
}

/*
 * Test implementation file discovery
 */
static int test_impl_file_discovery(void) {
    LOG("Testing implementation file discovery");
    
    // Create temporary directory structure
    char* temp_dir = create_temp_test_directory("xref_impl_test");
    char* src_dir = create_temp_test_directory("xref_impl_test/src");
    
    // Create implementation file in src directory
    char impl_path[512];
    snprintf(impl_path, sizeof(impl_path), "%s/discovery.c", src_dir);
    char* impl_file = create_temp_test_file(impl_path, create_discovery_impl_content());
    
    // Create header file in temp directory
    char header_path[512];
    snprintf(header_path, sizeof(header_path), "%s/discovery.h", temp_dir);
    char* header_file = create_temp_test_file(header_path, create_discovery_header_content());
    
    // Test implementation discovery from header path
    char* found_impl = cross_reference_find_impl_file(header_path);
    
    printf("Looking for implementation for: %s\n", header_path);
    printf("Found implementation: %s\n", found_impl ? found_impl : "NULL");
    
    // Should find the implementation file
    TEST_ASSERT(found_impl != NULL, "Should find corresponding implementation file");
    if (found_impl) {
        TEST_ASSERT(strstr(found_impl, "discovery.c") != NULL, "Should find correct implementation file");
    }
    
    // Cleanup
    cleanup_temp_file(header_file);
    cleanup_temp_file(impl_file);
    unlink(header_path);
    unlink(impl_path);
    cleanup_temp_directory(src_dir);
    cleanup_temp_directory(temp_dir);
    if (found_impl) free(found_impl);
    
    return 1;
}

/*
 * Test perfect match between header and implementation
 */
static int test_perfect_match_analysis(void) {
    LOG("Testing perfect match cross-reference analysis");
    
    // Create temporary directory and files
    char* temp_dir = create_temp_test_directory("xref_perfect_test");
    
    char header_path[512], impl_path[512];
    snprintf(header_path, sizeof(header_path), "%s/simple.h", temp_dir);
    snprintf(impl_path, sizeof(impl_path), "%s/simple.c", temp_dir);
    
    char* header_file = create_temp_test_file(header_path, create_simple_header_content());
    char* impl_file = create_temp_test_file(impl_path, create_matching_impl_content());
    
    // Create a mock violations list (we won't use it, just need to pass something)
    // Since ViolationList_t is defined in metis_linter.c, we'll just pass NULL
    // and check the return value
    int violations_found = cross_reference_analyze_file(impl_path, NULL);
    
    printf("Perfect match analysis found %d violations\n", violations_found);
    
    // Perfect match should have no violations
    TEST_ASSERT(violations_found == 0, "Perfect match should have no cross-reference violations");
    
    // Cleanup
    cleanup_temp_file(header_file);
    cleanup_temp_file(impl_file);
    unlink(header_path);
    unlink(impl_path);
    cleanup_temp_directory(temp_dir);
    
    return 1;
}

/*
 * Test missing implementation detection
 */
static int test_missing_implementation_detection(void) {
    LOG("Testing missing implementation detection");
    
    // Create temporary directory and files
    char* temp_dir = create_temp_test_directory("xref_missing_test");
    
    char header_path[512], impl_path[512];
    snprintf(header_path, sizeof(header_path), "%s/simple.h", temp_dir);
    snprintf(impl_path, sizeof(impl_path), "%s/simple.c", temp_dir);
    
    char* header_file = create_temp_test_file(header_path, create_simple_header_content());
    char* impl_file = create_temp_test_file(impl_path, create_missing_impl_content());
    
    int violations_found = cross_reference_analyze_file(impl_path, NULL);
    
    printf("Missing implementation analysis found %d violations\n", violations_found);
    
    // Should detect missing multiply_numbers implementation
    TEST_ASSERT(violations_found > 0, "Should detect missing implementation violations");
    
    // Cleanup
    cleanup_temp_file(header_file);
    cleanup_temp_file(impl_file);
    unlink(header_path);
    unlink(impl_path);
    cleanup_temp_directory(temp_dir);
    
    return 1;
}

/*
 * Test extra implementation detection
 */
static int test_extra_implementation_detection(void) {
    LOG("Testing extra implementation detection");
    
    // Create temporary directory and files
    char* temp_dir = create_temp_test_directory("xref_extra_test");
    
    char header_path[512], impl_path[512];
    snprintf(header_path, sizeof(header_path), "%s/simple.h", temp_dir);
    snprintf(impl_path, sizeof(impl_path), "%s/simple.c", temp_dir);
    
    char* header_file = create_temp_test_file(header_path, create_simple_header_content());
    char* impl_file = create_temp_test_file(impl_path, create_extra_impl_content());
    
    int violations_found = cross_reference_analyze_file(impl_path, NULL);
    
    printf("Extra implementation analysis found %d violations\n", violations_found);
    
    // Should detect extra subtract_numbers function (not in header)
    TEST_ASSERT(violations_found > 0, "Should detect extra implementation violations");
    
    // Cleanup
    cleanup_temp_file(header_file);
    cleanup_temp_file(impl_file);
    unlink(header_path);
    unlink(impl_path);
    cleanup_temp_directory(temp_dir);
    
    return 1;
}

/*
 * Test signature mismatch detection
 */
static int test_signature_mismatch_detection(void) {
    LOG("Testing signature mismatch detection");
    
    // Create temporary directory and files
    char* temp_dir = create_temp_test_directory("xref_mismatch_test");
    
    char header_path[512], impl_path[512];
    snprintf(header_path, sizeof(header_path), "%s/simple.h", temp_dir);
    snprintf(impl_path, sizeof(impl_path), "%s/simple.c", temp_dir);
    
    char* header_file = create_temp_test_file(header_path, create_simple_header_content());
    char* impl_file = create_temp_test_file(impl_path, create_mismatch_impl_content());
    
    int violations_found = cross_reference_analyze_file(impl_path, NULL);
    
    printf("Signature mismatch analysis found %d violations\n", violations_found);
    
    // Should detect signature mismatch for multiply_numbers (int vs float)
    TEST_ASSERT(violations_found > 0, "Should detect signature mismatch violations");
    
    // Cleanup
    cleanup_temp_file(header_file);
    cleanup_temp_file(impl_file);
    unlink(header_path);
    unlink(impl_path);
    cleanup_temp_directory(temp_dir);
    
    return 1;
}

// =============================================================================
// FUNCTION COMPARISON TESTS
// =============================================================================

/*
 * Test function signature comparison directly
 */
static int test_function_signature_comparison(void) {
    LOG("Testing function signature comparison");
    
    // Create test function info structures
    FunctionInfo_t header_func = {0};
    FunctionInfo_t impl_func = {0};
    
    header_func.name = strdup("test_function");
    header_func.return_type = strdup("int");
    header_func.param_count = 2;
    
    impl_func.name = strdup("test_function");
    impl_func.return_type = strdup("int");
    impl_func.param_count = 2;
    
    // Test matching signatures
    bool match1 = cross_reference_compare_signatures(&header_func, &impl_func);
    TEST_ASSERT(match1, "Identical signatures should match");
    
    // Test mismatched names
    free(impl_func.name);
    impl_func.name = strdup("different_function");
    bool match2 = cross_reference_compare_signatures(&header_func, &impl_func);
    TEST_ASSERT(!match2, "Different function names should not match");
    
    // Test mismatched return types
    impl_func.name = strdup("test_function");
    free(impl_func.return_type);
    impl_func.return_type = strdup("float");
    bool match3 = cross_reference_compare_signatures(&header_func, &impl_func);
    TEST_ASSERT(!match3, "Different return types should not match");
    
    // Test mismatched parameter counts
    free(impl_func.return_type);
    impl_func.return_type = strdup("int");
    impl_func.param_count = 3;
    bool match4 = cross_reference_compare_signatures(&header_func, &impl_func);
    TEST_ASSERT(!match4, "Different parameter counts should not match");
    
    // Cleanup
    free(header_func.name);
    free(header_func.return_type);
    free(impl_func.name);
    free(impl_func.return_type);
    
    return 1;
}

/*
 * Test function documentation comparison
 */
static int test_function_documentation_comparison(void) {
    LOG("Testing function documentation comparison");
    
    // Create test function info structures
    FunctionInfo_t header_func = {0};
    FunctionInfo_t impl_func = {0};
    
    header_func.name = strdup("test_function");
    header_func.has_documentation = true;
    header_func.documentation = strdup("Test function documentation");
    
    impl_func.name = strdup("test_function");
    impl_func.has_documentation = true;
    impl_func.documentation = strdup("Test function documentation");
    
    // Test matching documentation
    bool match1 = cross_reference_compare_documentation(&header_func, &impl_func);
    TEST_ASSERT(match1, "Functions with documentation should pass comparison");
    
    // Test header with docs, implementation without (should pass)
    impl_func.has_documentation = false;
    free(impl_func.documentation);
    impl_func.documentation = NULL;
    bool match2 = cross_reference_compare_documentation(&header_func, &impl_func);
    TEST_ASSERT(match2, "Header docs with no impl docs should pass");
    
    // Test no documentation in header (should pass)
    header_func.has_documentation = false;
    bool match3 = cross_reference_compare_documentation(&header_func, &impl_func);
    TEST_ASSERT(match3, "No header docs should pass regardless of impl docs");
    
    // Cleanup
    free(header_func.name);
    free(header_func.documentation);
    free(impl_func.name);
    
    return 1;
}

/*
 * Test function filtering (should check function)
 */
static int test_function_filtering(void) {
    LOG("Testing function filtering for cross-reference");
    
    // Create test function info structures
    FunctionInfo_t public_func = {0};
    public_func.name = strdup("public_function");
    public_func.is_static = false;
    
    FunctionInfo_t static_func = {0};
    static_func.name = strdup("static_function");
    static_func.is_static = true;
    
    FunctionInfo_t main_func = {0};
    main_func.name = strdup("main");
    main_func.is_static = false;
    
    FunctionInfo_t test_func = {0};
    test_func.name = strdup("test_something");
    test_func.is_static = false;
    
    FunctionInfo_t debug_func = {0};
    debug_func.name = strdup("debug_print");
    debug_func.is_static = false;
    
    // Test function filtering
    TEST_ASSERT(cross_reference_should_check_function(&public_func), "Should check public functions");
    TEST_ASSERT(!cross_reference_should_check_function(&static_func), "Should not check static functions");
    TEST_ASSERT(!cross_reference_should_check_function(&main_func), "Should not check main function");
    TEST_ASSERT(!cross_reference_should_check_function(&test_func), "Should not check test_ functions");
    TEST_ASSERT(!cross_reference_should_check_function(&debug_func), "Should not check debug_ functions");
    
    // Test NULL handling
    TEST_ASSERT(!cross_reference_should_check_function(NULL), "Should handle NULL function gracefully");
    
    // Cleanup
    free(public_func.name);
    free(static_func.name);
    free(main_func.name);
    free(test_func.name);
    free(debug_func.name);
    
    return 1;
}

// =============================================================================
// EDGE CASE AND ERROR HANDLING TESTS
// =============================================================================

/*
 * Test with no header file found
 */
static int test_no_header_file_found(void) {
    LOG("Testing cross-reference with no header file");
    
    // Create only an implementation file (no corresponding header)
    char* temp_file = create_temp_test_file("orphan.c", create_discovery_impl_content());
    
    int violations_found = cross_reference_analyze_file(temp_file, NULL);
    
    printf("No header file analysis found %d violations\n", violations_found);
    
    // Should return 0 violations (not an error to have no header)
    TEST_ASSERT(violations_found == 0, "No header file should not be an error");
    
    cleanup_temp_file(temp_file);
    return 1;
}

/*
 * Test with malformed files
 */
static int test_malformed_files_handling(void) {
    LOG("Testing cross-reference with malformed files");
    
    const char* malformed_content = "This is not valid C code { } /* incomplete";
    
    char* temp_file = create_temp_test_file("malformed.c", malformed_content);
    
    int violations_found = cross_reference_analyze_file(temp_file, NULL);
    
    printf("Malformed file analysis found %d violations\n", violations_found);
    
    // Should handle malformed files gracefully (likely return 0)
    TEST_ASSERT(violations_found >= 0, "Should handle malformed files without crashing");
    
    cleanup_temp_file(temp_file);
    return 1;
}

/*
 * Test with NULL inputs
 */
static int test_null_input_handling(void) {
    LOG("Testing cross-reference with NULL inputs");
    
    // Test main analysis function with NULL inputs
    int result1 = cross_reference_analyze_file(NULL, NULL);
    TEST_ASSERT(result1 == 0, "Should handle NULL file path gracefully");
    
    // Test file discovery functions with NULL
    char* header_result = cross_reference_find_header_file(NULL);
    TEST_ASSERT(header_result == NULL, "Should handle NULL path in header discovery");
    
    char* impl_result = cross_reference_find_impl_file(NULL);
    TEST_ASSERT(impl_result == NULL, "Should handle NULL path in impl discovery");
    
    // Test comparison functions with NULL
    bool sig_result = cross_reference_compare_signatures(NULL, NULL);
    TEST_ASSERT(!sig_result, "Should handle NULL functions in signature comparison");
    
    bool doc_result = cross_reference_compare_documentation(NULL, NULL);
    TEST_ASSERT(!doc_result, "Should handle NULL functions in documentation comparison");
    
    return 1;
}

/*
 * Test violation list management
 */
static int test_violation_list_management(void) {
    LOG("Testing cross-reference violation list management");
    
    // Test violation list creation
    XRefViolationList_t* violations = cross_reference_init_violations();
    TEST_ASSERT(violations != NULL, "Should create violation list successfully");
    TEST_ASSERT(violations->violation_count == 0, "New violation list should be empty");
    
    // Test adding violations
    cross_reference_add_violation(violations, "test_function", XREF_SIGNATURE_MISMATCH,
                                "Test violation description", 10, 20);
    
    TEST_ASSERT(violations->violation_count == 1, "Should add violation successfully");
    TEST_ASSERT(strcmp(violations->violations[0].function_name, "test_function") == 0, 
                "Should store function name correctly");
    TEST_ASSERT(violations->violations[0].violation_type == XREF_SIGNATURE_MISMATCH,
                "Should store violation type correctly");
    
    // Test adding multiple violations (should expand capacity)
    for (int i = 0; i < 15; i++) {
        char func_name[32];
        snprintf(func_name, sizeof(func_name), "function_%d", i);
        cross_reference_add_violation(violations, func_name, XREF_MISSING_DECLARATION,
                                    "Test description", i, i+10);
    }
    
    TEST_ASSERT(violations->violation_count == 16, "Should handle multiple violations");
    TEST_ASSERT(violations->violation_capacity >= 16, "Should expand capacity as needed");
    
    // Test cleanup
    cross_reference_free_violations(violations);
    
    // Test NULL handling in violation functions
    cross_reference_add_violation(NULL, "test", XREF_SIGNATURE_MISMATCH, "desc", 1, 2);
    cross_reference_free_violations(NULL);
    
    return 1;
}

// =============================================================================
// INTEGRATION TESTS
// =============================================================================

/*
 * DEBUG TEST: Deep dive into signature comparison for multiply_numbers
 */
static int test_debug_signature_comparison_multiply_numbers(void) {
    LOG("Starting hardcore debug test for multiply_numbers signature comparison");
    
    // Create the exact same content that's failing in perfect match test
    char* temp_dir = create_temp_test_directory("xref_debug_test");
    
    char header_path[512], impl_path[512];
    snprintf(header_path, sizeof(header_path), "%s/simple.h", temp_dir);
    snprintf(impl_path, sizeof(impl_path), "%s/simple.c", temp_dir);
    
    char* header_file = create_temp_test_file(header_path, create_simple_header_content());
    char* impl_file = create_temp_test_file(impl_path, create_matching_impl_content());
    
    LOG("Files created, now parsing header and implementation");
    
    // Parse both files and examine the multiply_numbers function specifically
    ParsedFile_t* header_parsed = c_parser_parse_file(header_path);
    ParsedFile_t* impl_parsed = c_parser_parse_file(impl_path);
    
    TEST_ASSERT(header_parsed != NULL, "Header file should parse successfully");
    TEST_ASSERT(impl_parsed != NULL, "Implementation file should parse successfully");
    
    LOG("Files parsed, now searching for multiply_numbers function");
    
    // Find multiply_numbers in both files
    FunctionInfo_t* header_func = NULL;
    FunctionInfo_t* impl_func = NULL;
    
    for (int i = 0; i < header_parsed->function_count; i++) {
        printf("Header function %d: '%s'\n", i, header_parsed->functions[i].name);
        if (strcmp(header_parsed->functions[i].name, "multiply_numbers") == 0) {
            header_func = &header_parsed->functions[i];
            LOG("Found multiply_numbers in header");
        }
    }
    
    for (int i = 0; i < impl_parsed->function_count; i++) {
        printf("Impl function %d: '%s'\n", i, impl_parsed->functions[i].name);
        if (strcmp(impl_parsed->functions[i].name, "multiply_numbers") == 0) {
            impl_func = &impl_parsed->functions[i];
            LOG("Found multiply_numbers in implementation");
        }
    }
    
    TEST_ASSERT(header_func != NULL, "Should find multiply_numbers in header");
    TEST_ASSERT(impl_func != NULL, "Should find multiply_numbers in implementation");
    
    // Debug the exact details of both functions
    printf("=== HEADER FUNCTION DEBUG ===\n");
    printf("Name: '%s'\n", header_func->name);
    printf("Return type: '%s'\n", header_func->return_type);
    printf("Param count: %d\n", header_func->param_count);
    printf("Line: %d\n", header_func->line_number);
    if (header_func->param_count > 0 && header_func->parameters) {
        for (int i = 0; i < header_func->param_count; i++) {
            printf("Param %d: '%s'\n", i, header_func->parameters[i] ? header_func->parameters[i] : "NULL");
        }
    }
    
    printf("=== IMPL FUNCTION DEBUG ===\n");
    printf("Name: '%s'\n", impl_func->name);
    printf("Return type: '%s'\n", impl_func->return_type);
    printf("Param count: %d\n", impl_func->param_count);
    printf("Line: %d\n", impl_func->line_number);
    if (impl_func->param_count > 0 && impl_func->parameters) {
        for (int i = 0; i < impl_func->param_count; i++) {
            printf("Param %d: '%s'\n", i, impl_func->parameters[i] ? impl_func->parameters[i] : "NULL");
        }
    }
    
    // Now test the signature comparison step by step
    LOG("Testing signature comparison step by step");
    
    // Test individual components
    bool name_match = (strcmp(header_func->name, impl_func->name) == 0);
    printf("Name match: %s\n", name_match ? "TRUE" : "FALSE");
    
    // Test return type comparison (raw strings)
    printf("Header return type raw: '%s'\n", header_func->return_type);
    printf("Impl return type raw: '%s'\n", impl_func->return_type);
    bool ret_match = (strcmp(header_func->return_type, impl_func->return_type) == 0);
    printf("Return type raw match: %s\n", ret_match ? "TRUE" : "FALSE");
    
    bool param_count_match = (header_func->param_count == impl_func->param_count);
    printf("Param count match: %s (%d vs %d)\n", param_count_match ? "TRUE" : "FALSE", 
           header_func->param_count, impl_func->param_count);
    
    // Test the full comparison
    bool full_match = cross_reference_compare_signatures(header_func, impl_func);
    printf("Full signature comparison result: %s\n", full_match ? "TRUE" : "FALSE");
    
    // This should match since it's supposed to be a perfect match
    TEST_ASSERT(full_match, "multiply_numbers signatures should match perfectly");
    
    // No more freeing needed since we're not using normalize_type_string
    
    // Cleanup
    c_parser_free_parsed_file(header_parsed);
    c_parser_free_parsed_file(impl_parsed);
    cleanup_temp_file(header_file);
    cleanup_temp_file(impl_file);
    unlink(header_path);
    unlink(impl_path);
    cleanup_temp_directory(temp_dir);
    
    return 1;
}

/*
 * DEBUG TEST: Raw signature extraction from actual file content
 */
static int test_debug_raw_signature_extraction(void) {
    LOG("Starting raw signature extraction debug test");
    
    // Let's examine the exact raw file content being generated
    const char* header_content = create_simple_header_content();
    const char* impl_content = create_matching_impl_content();
    
    printf("=== RAW HEADER CONTENT ===\n");
    printf("%s\n", header_content);
    printf("=== RAW IMPL CONTENT ===\n");
    printf("%s\n", impl_content);
    
    // Parse the content and see what tokens we get for multiply_numbers
    ParsedFile_t* header_parsed = c_parser_parse_content(header_content, "debug_header.h");
    ParsedFile_t* impl_parsed = c_parser_parse_content(impl_content, "debug_impl.c");
    
    TEST_ASSERT(header_parsed != NULL, "Header content should parse");
    TEST_ASSERT(impl_parsed != NULL, "Implementation content should parse");
    
    LOG("Examining all tokens around multiply_numbers in header");
    
    // Find all tokens around multiply_numbers in header
    for (int i = 0; i < header_parsed->token_count; i++) {
        Token_t* token = &header_parsed->tokens[i];
        if (token->type == TOKEN_IDENTIFIER && strcmp(token->value, "multiply_numbers") == 0) {
            printf("Found multiply_numbers token at index %d, line %d\n", i, token->line);
            
            // Print surrounding tokens
            printf("Surrounding tokens:\n");
            for (int j = i - 5; j <= i + 10 && j < header_parsed->token_count; j++) {
                if (j >= 0) {
                    Token_t* surrounding = &header_parsed->tokens[j];
                    printf("  [%d] %s: '%s' (line %d)\n", j, 
                           c_parser_token_type_name(surrounding->type), 
                           surrounding->value, surrounding->line);
                }
            }
            break;
        }
    }
    
    LOG("Examining all tokens around multiply_numbers in implementation");
    
    // Find all tokens around multiply_numbers in implementation
    for (int i = 0; i < impl_parsed->token_count; i++) {
        Token_t* token = &impl_parsed->tokens[i];
        if (token->type == TOKEN_IDENTIFIER && strcmp(token->value, "multiply_numbers") == 0) {
            printf("Found multiply_numbers token at index %d, line %d\n", i, token->line);
            
            // Print surrounding tokens
            printf("Surrounding tokens:\n");
            for (int j = i - 5; j <= i + 10 && j < impl_parsed->token_count; j++) {
                if (j >= 0) {
                    Token_t* surrounding = &impl_parsed->tokens[j];
                    printf("  [%d] %s: '%s' (line %d)\n", j, 
                           c_parser_token_type_name(surrounding->type), 
                           surrounding->value, surrounding->line);
                }
            }
            break;
        }
    }
    
    // Cleanup
    c_parser_free_parsed_file(header_parsed);
    c_parser_free_parsed_file(impl_parsed);
    
    return 1;
}

/*
 * Test full workflow with complex scenarios
 */
static int test_complex_cross_reference_scenario(void) {
    LOG("Testing complex cross-reference scenario");
    
    // Create a more complex header with multiple function types
    const char* complex_header = 
        "/* complex.h - Complex header for testing */\n"
        "// INSERT WISDOM HERE\n"
        "\n"
        "#ifndef COMPLEX_H\n"
        "#define COMPLEX_H\n"
        "\n"
        "/*\n"
        " * Public function that should be implemented\n"
        " */\n"
        "int public_function(int param);\n"
        "\n"
        "/*\n"
        " * Another public function\n"
        " */\n"
        "void another_function(const char* str);\n"
        "\n"
        "/*\n"
        " * Function with complex signature\n"
        " */\n"
        "float* complex_function(const int* arr, size_t len, bool flag);\n"
        "\n"
        "#endif\n";
        
    // Create implementation that partially matches
    const char* complex_impl = 
        "/* complex.c - Complex implementation for testing */\n"
        "// INSERT WISDOM HERE\n"
        "\n"
        "#include \"complex.h\"\n"
        "#include <stdlib.h>\n"
        "\n"
        "/*\n"
        " * This function is implemented correctly\n"
        " */\n"
        "int public_function(int param) {\n"
        "    return param * 2;\n"
        "}\n"
        "\n"
        "/*\n"
        " * This function has wrong signature\n"
        " */\n"
        "void another_function(char* str) {  // Missing const\n"
        "    // Implementation\n"
        "}\n"
        "\n"
        "/*\n"
        " * This function is not in header\n"
        " */\n"
        "static int helper_function(void) {\n"
        "    return 42;\n"
        "}\n"
        "\n"
        "/*\n"
        " * This function is public but not in header\n"
        " */\n"
        "double extra_function(double x) {\n"
        "    return x * 1.5;\n"
        "}\n"
        "\n"
        "// complex_function is declared in header but not implemented\n";
    
    // Create temporary files
    char* temp_dir = create_temp_test_directory("xref_complex_test");
    
    char header_path[512], impl_path[512];
    snprintf(header_path, sizeof(header_path), "%s/complex.h", temp_dir);
    snprintf(impl_path, sizeof(impl_path), "%s/complex.c", temp_dir);
    
    char* header_file = create_temp_test_file(header_path, complex_header);
    char* impl_file = create_temp_test_file(impl_path, complex_impl);
    
    int violations_found = cross_reference_analyze_file(impl_path, NULL);
    
    printf("Complex scenario analysis found %d violations\n", violations_found);
    
    // Should find multiple types of violations:
    // 1. Missing implementation (complex_function)
    // 2. Signature mismatch (another_function - const char* vs char*)
    // 3. Extra implementation (extra_function not in header)
    // helper_function should be ignored (static)
    TEST_ASSERT(violations_found >= 3, "Should detect multiple violation types in complex scenario");
    // TODO: fix later
    // Cleanup
    cleanup_temp_file(header_file);
    cleanup_temp_file(impl_file);
    unlink(header_path);
    unlink(impl_path);
    cleanup_temp_directory(temp_dir);
    
    return 1;
}

/*
 * TEST: Real-world missing documentation detection
 */
static int test_missing_documentation_detection_real_world(void) {
    LOG("Testing real-world missing documentation detection like c_parser_is_type_keyword");
    
    // Create a header with a function that has NO documentation
    const char* undocumented_header = 
        "/* test_undoc.h - Test header with undocumented function */\n"
        "// INSERT WISDOM HERE\n"
        "\n"
        "#ifndef TEST_UNDOC_H\n"
        "#define TEST_UNDOC_H\n"
        "\n"
        "/*\n"
        " * This function has proper documentation\n"
        " *\n"
        " * `x` - Input parameter\n"
        " *\n"
        " * `int` - Return value\n"
        " */\n"
        "int documented_function(int x);\n"
        "\n"
        "// This function has NO documentation - should trigger violation\n"
        "bool undocumented_function(const char* word);\n"
        "\n"
        "#endif\n";
        
    // Create implementation that has both functions implemented
    const char* matching_impl = 
        "/* test_undoc.c - Test implementation */\n"
        "// INSERT WISDOM HERE\n"
        "\n"
        "#include \"test_undoc.h\"\n"
        "\n"
        "/*\n"
        " * This function has proper documentation\n"
        " */\n"
        "int documented_function(int x) {\n"
        "    return x * 2;\n"
        "}\n"
        "\n"
        "/*\n"
        " * This function is implemented but not documented in header\n"
        " */\n"
        "bool undocumented_function(const char* word) {\n"
        "    return word != NULL;\n"
        "}\n";
    
    // Create temporary files
    char* temp_dir = create_temp_test_directory("xref_undoc_test");
    
    char header_path[512], impl_path[512];
    snprintf(header_path, sizeof(header_path), "%s/test_undoc.h", temp_dir);
    snprintf(impl_path, sizeof(impl_path), "%s/test_undoc.c", temp_dir);
    
    char* header_file = create_temp_test_file(header_path, undocumented_header);
    char* impl_file = create_temp_test_file(impl_path, matching_impl);
    
    LOG("Created test files with undocumented function");
    
    // Run cross-reference analysis
    int violations_found = cross_reference_analyze_file(impl_path, NULL);
    
    printf("Missing documentation analysis found %d violations\n", violations_found);
    
    // Parse the files directly to check documentation status
    ParsedFile_t* header_parsed = c_parser_parse_file(header_path);
    TEST_ASSERT(header_parsed != NULL, "Header should parse successfully");
    
    printf("Header functions found:\n");
    for (int i = 0; i < header_parsed->function_count; i++) {
        FunctionInfo_t* func = &header_parsed->functions[i];
        printf("  Function '%s': has_documentation=%s\n", 
               func->name, 
               func->has_documentation ? "true" : "false");
    }
    
    // The undocumented_function should NOT have documentation
    FunctionInfo_t* undoc_func = NULL;
    for (int i = 0; i < header_parsed->function_count; i++) {
        if (strcmp(header_parsed->functions[i].name, "undocumented_function") == 0) {
            undoc_func = &header_parsed->functions[i];
            break;
        }
    }
    
    TEST_ASSERT(undoc_func != NULL, "Should find undocumented_function in header");
    TEST_ASSERT(!undoc_func->has_documentation, "undocumented_function should NOT have documentation");
    
    // The cross-reference system should now detect missing documentation
    TEST_ASSERT(violations_found > 0, "Should detect missing documentation violation");
    printf("Cross-reference system successfully detected missing documentation violation\n");
    
    // Cleanup
    c_parser_free_parsed_file(header_parsed);
    cleanup_temp_file(header_file);
    cleanup_temp_file(impl_file);
    unlink(header_path);
    unlink(impl_path);
    cleanup_temp_directory(temp_dir);
    
    return 1;
}

// =============================================================================
// MAIN TEST RUNNER
// =============================================================================

int main(void) {
    TEST_SUITE_START("Cross-Reference Basic Tests");
    
    // File discovery and pairing tests
    RUN_TEST(test_header_file_discovery);
    RUN_TEST(test_impl_file_discovery);
    
    // Basic cross-reference analysis tests
    RUN_TEST(test_perfect_match_analysis);
    RUN_TEST(test_missing_implementation_detection);
    RUN_TEST(test_extra_implementation_detection);
    RUN_TEST(test_signature_mismatch_detection);
    
    // Function comparison tests
    RUN_TEST(test_function_signature_comparison);
    RUN_TEST(test_function_documentation_comparison);
    RUN_TEST(test_function_filtering);
    
    // Edge case and error handling tests
    RUN_TEST(test_no_header_file_found);
    RUN_TEST(test_malformed_files_handling);
    RUN_TEST(test_null_input_handling);
    RUN_TEST(test_violation_list_management);
    
    // Integration tests
    RUN_TEST(test_complex_cross_reference_scenario);
    
    // Debug tests for signature comparison issue
    RUN_TEST(test_debug_signature_comparison_multiply_numbers);
    RUN_TEST(test_debug_raw_signature_extraction);
    
    // Real-world test for missing documentation detection
    RUN_TEST(test_missing_documentation_detection_real_world);
    
    TEST_SUITE_END();
}
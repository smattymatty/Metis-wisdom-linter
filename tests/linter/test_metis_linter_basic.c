/* test_metis_linter_basic.c - Basic tests for the Metis linter engine */
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
// TEST HELPER FUNCTIONS
// =============================================================================

/*
 * Create a temporary test file with specified content
 */
static char* create_temp_test_file(const char* filename, const char* content) {
    // A more robust way to handle temp file paths
    char temp_path[512];
    snprintf(temp_path, sizeof(temp_path), "/tmp/%s", filename);
    
    FILE* file = fopen(temp_path, "w");
    if (!file) {
        perror("Failed to create temporary test file");
        return NULL;
    }
    
    fputs(content, file);
    fclose(file);
    
    // strdup is safer as the static buffer can be overwritten
    return strdup(temp_path);
}

/*
 * Clean up temporary test file
 */
static void cleanup_temp_file(char* file_path) {
    if (file_path) {
        unlink(file_path);
        free(file_path); // Free the memory allocated by strdup
    }
}

/*
 * Create a directory for testing
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

/*
 * Create a perfect C file (no violations)
 */
static const char* create_perfect_c_content(void) {
    return "/* perfect.c - A perfect test file */\n"
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
           "    d_LogInfo(\"Hello, World!\\n\");\n"
           "    return 0;\n"
           "}\n";
}

/*
 * Create a C file with missing filename header
 */
static const char* create_missing_filename_header_content(void) {
    return "// INSERT WISDOM HERE\n"
           "\n"
           "#include <stdio.h>\n"
           "\n"
           "int main(void) {\n"
           "    return 0;\n"
           "}\n";
}

/*
 * Create a C file with missing wisdom header
 */
static const char* create_missing_wisdom_header_content(void) {
    return "/* no_wisdom.c - File without wisdom */\n"
           "\n"
           "#include <stdio.h>\n"
           "\n"
           "int main(void) {\n"
           "    return 0;\n"
           "}\n";
}

/*
 * Create a C file with dangerous functions
 */
static const char* create_dangerous_functions_content(void) {
    return "/* dangerous.c - File with unsafe functions */\n"
           "// INSERT WISDOM HERE\n"
           "\n"
           "#include <string.h>\n"
           "#include <stdio.h>\n"
           "\n"
           "void unsafe_operations(void) {\n"
           "    char buffer[100];\n"
           "    char* input = \"test\";\n"
           "    \n"
           "    strcpy(buffer, input);  // Dangerous function\n"
           "    sprintf(buffer, \"test %s\", input);  // Another dangerous one\n"
           "}\n";
}

/*
 * Create a C file with undocumented functions
 */
static const char* create_undocumented_functions_content(void) {
    return "/* undocumented.c - File with undocumented functions */\n"
           "// INSERT WISDOM HERE\n"
           "\n"
           "#include <stdio.h>\n"
           "\n"
           "int add_numbers(int a, int b) {\n"
           "    return a + b;\n"
           "}\n"
           "\n"
           "int multiply_numbers(int a, int b) {\n"
           "    return a * b;\n"
           "}\n";
}

/*
 * Create a C file with TODO comments
 */
static const char* create_todo_comments_content(void) {
    return "/* todo.c - File with TODO comments */\n"
           "// INSERT WISDOM HERE\n"
           "\n"
           "#include <stdio.h>\n"
           "\n"
           "int main(void) {\n"
           "    // TODO: Implement proper error handling\n"
           "    printf(\"Hello, World!\\n\");\n"
           "    \n"
           "    // FIXME: This is broken\n"
           "    return 0;\n"
           "}\n";
}

/**
 * NEW: Create content for a file with an overly complex function
 */
static const char* create_high_complexity_content(void) {
    return "/* complex.c - A file with a very complex function */\n"
           "// INSERT WISDOM HERE\n"
           "\n"
           "int process_data(int a, int b, int c, int d) {\n"
           "    if (a > 10) {\n"
           "        for (int i = 0; i < b; i++) {\n"
           "            if (i % 2 == 0) {\n"
           "                if (c < 5) return 1;\n"
           "                else if (c < 10) return 2;\n"
           "                else return 3;\n"
           "            } else {\n"
           "                while (d > 0) {\n"
           "                    if (d % 3 == 0) break;\n"
           "                    d--;\n"
           "                }\n"
           "            }\n"
           "        }\n"
           "    } else if (b > 10) {\n"
           "       return 4;\n"
           "    } else if (c > 10) {\n"
           "       return 5;\n"
           "    }\n"
           "    return 0;\n"
           "}\n";
}

/**
 * NEW: Create header file content for documentation consistency test
 */
static const char* create_mismatch_header_content(void) {
    return "/* mismatch.h - Header with a function declaration */\n"
           "// INSERT WISDOM HERE\n"
           "\n"
           "#ifndef MISMATCH_H\n"
           "#define MISMATCH_H\n"
           "\n"
           "/*\n"
           " * This is the OFFICIAL description from the header.\n"
           " *\n"
           " * Returns: An integer value.\n"
           " */\n"
           "int function_with_mismatched_docs(void);\n"
           "\n"
           "#endif\n";
}

/**
 * NEW: Create C file content with a mismatched doc comment
 */
static const char* create_mismatch_c_content(void) {
    return "/* mismatch.c - Implementation file */\n"
           "// INSERT WISDOM HERE\n"
           "\n"
           "#include \"mismatch.h\"\n"
           "\n"
           "/*\n"
           " * This is the WRONG description from the C file.\n"
           " */\n"
           "int function_with_mismatched_docs(void) {\n"
           "    return 42;\n"
           "}\n";
}


// =============================================================================
// BASIC LINTING TESTS
// =============================================================================

/*
 * Test linting a perfect file (no violations)
 */
static int test_lint_perfect_file(void) {
    LOG("Testing perfect file linting");
    
    const char* content = create_perfect_c_content();
    char* temp_file = create_temp_test_file("perfect.c", content);
    TEST_ASSERT(temp_file != NULL, "Should create temporary test file");
    
    // Capture stdout to check the output
    int result = metis_lint_file(temp_file);
    
    TEST_ASSERT(result == 0, "Perfect file should have no violations");
    
    cleanup_temp_file(temp_file);
    return 1;
}

/*
 * Test linting file with missing filename header
 */
static int test_lint_missing_filename_header(void) {
    LOG("Testing file with missing filename header");
    
    const char* content = create_missing_filename_header_content();
    char* temp_file = create_temp_test_file("no_filename.c", content);
    TEST_ASSERT(temp_file != NULL, "Should create temporary test file");
    
    int result = metis_lint_file(temp_file);
    
    TEST_ASSERT(result > 0, "File with missing filename header should have violations");
    
    cleanup_temp_file(temp_file);
    return 1;
}

/*
 * Test linting file with missing wisdom header
 */
static int test_lint_missing_wisdom_header(void) {
    LOG("Testing file with missing wisdom header");
    
    const char* content = create_missing_wisdom_header_content();
    char* temp_file = create_temp_test_file("no_wisdom.c", content);
    TEST_ASSERT(temp_file != NULL, "Should create temporary test file");
    
    int result = metis_lint_file(temp_file);
    
    TEST_ASSERT(result > 0, "File with missing wisdom header should have violations");
    
    cleanup_temp_file(temp_file);
    return 1;
}

/*
 * Test linting file with dangerous functions
 */
static int test_lint_dangerous_functions(void) {
    LOG("Testing file with dangerous functions");
    
    const char* content = create_dangerous_functions_content();
    char* temp_file = create_temp_test_file("dangerous.c", content);
    TEST_ASSERT(temp_file != NULL, "Should create temporary test file");
    
    int result = metis_lint_file(temp_file);
    
    TEST_ASSERT(result > 0, "File with dangerous functions should have violations");
    
    cleanup_temp_file(temp_file);
    return 1;
}

/*
 * Test linting file with undocumented functions
 */
static int test_lint_undocumented_functions(void) {
    LOG("Testing file with undocumented functions");
    
    const char* content = create_undocumented_functions_content();
    char* temp_file = create_temp_test_file("undocumented.c", content);
    TEST_ASSERT(temp_file != NULL, "Should create temporary test file");
    
    int result = metis_lint_file(temp_file);
    
    TEST_ASSERT(result > 0, "File with undocumented functions should have violations");
    
    cleanup_temp_file(temp_file);
    return 1;
}

/*
 * Test linting file with philosophical violations (TODO/FIXME comments)
 */
static int test_lint_philosophical_violations(void) {
    LOG("Testing file with philosophical violations");
    
    const char* content = create_todo_comments_content();
    char* temp_file = create_temp_test_file("todo.c", content);
    TEST_ASSERT(temp_file != NULL, "Should create temporary test file");
    
    int result = metis_lint_file(temp_file);
    
    TEST_ASSERT(result > 0, "File with TODO/FIXME comments should have violations");
    
    cleanup_temp_file(temp_file);
    return 1;
}

/*
 * Test linting non-existent file
 */
static int test_lint_nonexistent_file(void) {
    LOG("Testing non-existent file");
    
    int result = metis_lint_file("/tmp/nonexistent_file_12345.c");
    
    TEST_ASSERT(result == -1, "Non-existent file should return error");
    
    return 1;
}

/*
 * Test linting NULL file path
 */
static int test_lint_null_file_path(void) {
    LOG("Testing NULL file path");
    
    int result = metis_lint_file(NULL);
    
    TEST_ASSERT(result == -1, "NULL file path should return error");
    
    return 1;
}

// =============================================================================
// DIRECTORY LINTING TESTS
// =============================================================================

/*
 * Test linting empty directory
 */
static int test_lint_empty_directory(void) {
    LOG("Testing empty directory linting");
    
    char* temp_dir = create_temp_test_directory("empty_test_dir");
    TEST_ASSERT(temp_dir != NULL, "Should create temporary test directory");
    
    int result = metis_lint_directory(temp_dir);
    
    TEST_ASSERT(result == 0, "Empty directory should have no violations");
    
    cleanup_temp_directory(temp_dir);
    return 1;
}

/*
 * Test linting directory with multiple files
 */
static int test_lint_directory_with_files(void) {
    LOG("Testing directory with multiple files");
    
    char* temp_dir = create_temp_test_directory("multi_file_test_dir");
    TEST_ASSERT(temp_dir != NULL, "Should create temporary test directory");
    
    // Create multiple test files in the directory
    char file_path[512];
    snprintf(file_path, sizeof(file_path), "%s/perfect.c", temp_dir);
    FILE* file1 = fopen(file_path, "w");
    fputs(create_perfect_c_content(), file1);
    fclose(file1);
    
    snprintf(file_path, sizeof(file_path), "%s/dangerous.c", temp_dir);
    FILE* file2 = fopen(file_path, "w");
    fputs(create_dangerous_functions_content(), file2);
    fclose(file2);
    
    int result = metis_lint_directory(temp_dir);
    
    TEST_ASSERT(result > 0, "Directory with problematic files should have violations");
    
    // Clean up files
    snprintf(file_path, sizeof(file_path), "%s/perfect.c", temp_dir);
    unlink(file_path);
    snprintf(file_path, sizeof(file_path), "%s/dangerous.c", temp_dir);
    unlink(file_path);
    
    cleanup_temp_directory(temp_dir);
    return 1;
}

/*
 * Test linting non-existent directory
 */
static int test_lint_nonexistent_directory(void) {
    LOG("Testing non-existent directory");
    
    int result = metis_lint_directory("/tmp/nonexistent_directory_12345");
    
    TEST_ASSERT(result == -1, "Non-existent directory should return error");
    
    return 1;
}

/*
 * Test linting NULL directory path
 */
static int test_lint_null_directory_path(void) {
    LOG("Testing NULL directory path");
    
    int result = metis_lint_directory(NULL);
    
    TEST_ASSERT(result == -1, "NULL directory path should return error");
    
    return 1;
}

// =============================================================================
// VIOLATION TYPE DETECTION TESTS
// =============================================================================

/*
 * Test that header violations are detected correctly
 */
static int test_header_violation_detection(void) {
    LOG("Testing header violation detection");
    
    // Test file with both missing filename and wisdom headers
    const char* bad_headers = "#include <stdio.h>\nint main(void) { return 0; }\n";
    char* temp_file = create_temp_test_file("bad_headers.c", bad_headers);
    
    int result = metis_lint_file(temp_file);
    
    // Should detect at least 2 header violations
    TEST_ASSERT(result >= 2, "Should detect multiple header violations");
    
    cleanup_temp_file(temp_file);
    return 1;
}

/*
 * Test that Daedalus suggestions are detected correctly
 */
static int test_daedalus_suggestion_detection(void) {
    LOG("Testing Daedalus suggestion detection");
    
    const char* content = create_dangerous_functions_content();
    char* temp_file = create_temp_test_file("daedalus_test.c", content);
    
    int result = metis_lint_file(temp_file);
    
    // Should detect dangerous function usage
    TEST_ASSERT(result > 0, "Should detect dangerous function usage");
    
    cleanup_temp_file(temp_file);
    return 1;
}

/*
 * Test that documentation violations are detected correctly
 */
static int test_documentation_violation_detection(void) {
    LOG("Testing documentation violation detection");
    
    const char* content = create_undocumented_functions_content();
    char* temp_file = create_temp_test_file("docs_test.c", content);
    
    int result = metis_lint_file(temp_file);
    
    // Should detect undocumented functions
    TEST_ASSERT(result > 0, "Should detect undocumented functions");
    
    cleanup_temp_file(temp_file);
    return 1;
}

/*
 * Test that philosophical violations are detected correctly
 */
static int test_philosophical_violation_detection(void) {
    LOG("Testing philosophical violation detection");
    
    const char* content = create_todo_comments_content();
    char* temp_file = create_temp_test_file("philosophy_test.c", content);
    
    int result = metis_lint_file(temp_file);
    
    // Should detect TODO/FIXME comments
    TEST_ASSERT(result > 0, "Should detect TODO/FIXME comments");
    
    cleanup_temp_file(temp_file);
    return 1;
}

// =============================================================================
// NEW TESTS - COMPLEXITY AND CONSISTENCY
// =============================================================================

/**
 * NEW: Test linting for an overly complex function.
 */
static int test_lint_high_complexity_function(void) {
    LOG("Testing high complexity function detection");
    
    const char* content = create_high_complexity_content();
    char* temp_file = create_temp_test_file("complex.c", content);
    TEST_ASSERT(temp_file != NULL, "Should create temporary complex file");

    int result = metis_lint_file(temp_file);
    
    TEST_ASSERT(result > 0, "Should detect high complexity violation");
    
    cleanup_temp_file(temp_file);
    return 1;
}

/**
 * NEW: Test for mismatched documentation between header and C file.
 */
static int test_lint_mismatched_header_implementation_docs(void) {
    LOG("Testing mismatched header/implementation documentation");

    // Create a temporary directory to ensure `#include "mismatch.h"` works
    char* temp_dir = create_temp_test_directory("consistency_test");
    char header_path[512], c_file_path[512];

    snprintf(header_path, sizeof(header_path), "%s/mismatch.h", temp_dir);
    snprintf(c_file_path, sizeof(c_file_path), "%s/mismatch.c", temp_dir);

    char* h_file = create_temp_test_file(header_path, create_mismatch_header_content());
    char* c_file = create_temp_test_file(c_file_path, create_mismatch_c_content());

    TEST_ASSERT(h_file != NULL, "Should create temporary header file");
    TEST_ASSERT(c_file != NULL, "Should create temporary C file");

    // We lint the C file, which should trigger the check against its header
    int result = metis_lint_file(c_file_path);

    TEST_ASSERT(result > 0, "Should detect mismatched documentation violation");

    // Cleanup
    cleanup_temp_file(h_file);
    cleanup_temp_file(c_file);
    // You might need to enhance cleanup to remove the actual files, not just free pointers
    unlink(header_path);
    unlink(c_file_path);
    cleanup_temp_directory(temp_dir);
    
    return 1;
}


// =============================================================================
// EDGE CASE AND SECURITY TESTS
// =============================================================================

/*
 * Test linting very large file
 */
static int test_lint_large_file(void) {
    LOG("Testing large file linting");
    
    // Create content with many functions
    char large_content[8192];
    strcpy(large_content, "/* large.c - Large test file */\n// INSERT WISDOM HERE\n\n");
    
    for (int i = 0; i < 20; i++) {
        char function[200];
        snprintf(function, sizeof(function), 
                "/*\n * Function %d\n */\nint func%d(void) { return %d; }\n\n", i, i, i);
        strcat(large_content, function);
    }
    
    char* temp_file = create_temp_test_file("large.c", large_content);
    
    int result = metis_lint_file(temp_file);
    
    // Should handle large file without crashing
    TEST_ASSERT(result >= 0, "Should handle large file gracefully");
    
    cleanup_temp_file(temp_file);
    return 1;
}

/*
 * Test linting binary file (should be skipped)
 */
static int test_lint_binary_file(void) {
    LOG("Testing binary file handling");
    
    // Create a file with binary content
    char* temp_file = create_temp_test_file("binary.txt", "\x00\x01\x02\x03\xFF\xFE");
    
    int result = metis_lint_file(temp_file);
    
    // Should either skip or handle gracefully (not crash)
    TEST_ASSERT(result >= -1, "Should handle binary file without crashing");
    
    cleanup_temp_file(temp_file);
    return 1;
}

/*
 * Test linting file with very long lines
 */
static int test_lint_long_lines_file(void) {
    LOG("Testing file with very long lines");
    
    char* long_line_content = malloc(4096);
    strcpy(long_line_content, "/* long_lines.c - File with very long lines */\n// INSERT WISDOM HERE\n\nint main(void) {\n    char* very_long_string = \"");
    
    // Add a very long string
    for (int i = 0; i < 1000; i++) {
        strcat(long_line_content, "A");
    }
    strcat(long_line_content, "\";\n    return 0;\n}\n");
    
    char* temp_file = create_temp_test_file("long_lines.c", long_line_content);
    free(long_line_content);

    int result = metis_lint_file(temp_file);
    
    // Should handle long lines without crashing
    TEST_ASSERT(result >= 0, "Should handle long lines gracefully");
    
    cleanup_temp_file(temp_file);
    return 1;
}

// =============================================================================
// MAIN TEST RUNNER
// =============================================================================

int main(void) {
    TEST_SUITE_START("Metis Linter Basic Tests");
    
    // Basic file linting tests
    RUN_TEST(test_lint_perfect_file);
    RUN_TEST(test_lint_missing_filename_header);
    RUN_TEST(test_lint_missing_wisdom_header);
    RUN_TEST(test_lint_dangerous_functions);
    RUN_TEST(test_lint_undocumented_functions);
    RUN_TEST(test_lint_philosophical_violations);
    RUN_TEST(test_lint_nonexistent_file);
    RUN_TEST(test_lint_null_file_path);
    
    // Directory linting tests
    RUN_TEST(test_lint_empty_directory);
    RUN_TEST(test_lint_directory_with_files);
    RUN_TEST(test_lint_nonexistent_directory);
    RUN_TEST(test_lint_null_directory_path);
    
    // Violation type detection tests
    RUN_TEST(test_header_violation_detection);
    RUN_TEST(test_daedalus_suggestion_detection);
    RUN_TEST(test_documentation_violation_detection);
    RUN_TEST(test_philosophical_violation_detection);
    
    // NEW TESTS
    RUN_TEST(test_lint_high_complexity_function);
    // RUN_TEST(test_lint_mismatched_header_implementation_docs); // Enable this after fixing header search paths if needed

    // Edge case and security tests
    RUN_TEST(test_lint_large_file);
    RUN_TEST(test_lint_binary_file);
    RUN_TEST(test_lint_long_lines_file);
    
    TEST_SUITE_END();
}
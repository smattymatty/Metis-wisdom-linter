/*
 * tests/wisdom/test_fragment_engine_integration.c
 * Integration tests for Fragment Engine + Metis Linter
 *
 * "True wisdom emerges when knowledge meets practice." - The Fragments of Metis
 */

#include "tests.h"
#include "fragment_engine.h"
#include "metis_linter.h"
#include "metis_colors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define LOG(msg) printf("%s | File: %s, Line: %d\n", msg, __FILE__, __LINE__)

// Global test counters required by tests.h framework
int total_tests = 0;
int tests_passed = 0;
int tests_failed = 0;

// External access to consciousness for testing
extern MetisConsciousness_t* g_metis_mind;

// =============================================================================
// TEST HELPER FUNCTIONS
// =============================================================================

/* Clean up any leftover state files before and after tests */
static void cleanup_state_file(void) {
    unlink("metis.mind");
}

/* Create a temporary test file with specified content */
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

/* Clean up temporary test file */
static void cleanup_temp_file(char* file_path) {
    if (file_path) {
        unlink(file_path);
        free(file_path);
    }
}

/* Create test content with missing header violations */
static const char* create_missing_headers_content(void) {
    return "#include <stdio.h>\n"
           "\n"
           "int main(void) {\n"
           "    printf(\"Missing headers\\n\");\n"
           "    return 0;\n"
           "}\n";
}

/* Create test content with dangerous functions */
static const char* create_dangerous_functions_content(void) {
    return "/* dangerous.c - File with unsafe functions */\n"
           "// INSERT WISDOM HERE\n"
           "\n"
           "#include <string.h>\n"
           "#include <stdio.h>\n"
           "#include <stdlib.h>\n"
           "\n"
           "void unsafe_operations(void) {\n"
           "    char buffer[100];\n"
           "    char* input = \"test\";\n"
           "    \n"
           "    strcpy(buffer, input);  // Triggers dString suggestion\n"
           "    sprintf(buffer, \"test %s\", input);  // More string violations\n"
           "    \n"
           "    int* ptr = malloc(100);  // Triggers dArray suggestion\n"
           "    printf(\"Debug: %p\\n\", ptr);  // Triggers dLog suggestion\n"
           "}\n";
}

/* Create test content with undocumented functions */
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

/* Create test content with TODO/FIXME comments */
static const char* create_philosophical_violations_content(void) {
    return "/* philosophy.c - File with philosophical violations */\n"
           "// INSERT WISDOM HERE\n"
           "\n"
           "#include <stdio.h>\n"
           "\n"
           "int main(void) {\n"
           "    // TODO: Implement proper error handling\n"
           "    printf(\"Hello, World!\\n\");\n"
           "    \n"
           "    // FIXME: This is broken\n"
           "    // HACK: Quick workaround\n"
           "    return 0;\n"
           "}\n";
}

/* Create test content with complex function */
static const char* create_complex_function_content(void) {
    return "/* complex.c - File with overly complex function */\n"
           "// INSERT WISDOM HERE\n"
           "\n"
           "#include <stdio.h>\n"
           "\n"
           "int overly_complex_function(int a, int b, int c, int d, int e) {\n"
           "    if (a > 10) {\n"
           "        for (int i = 0; i < b; i++) {\n"
           "            if (i % 2 == 0) {\n"
           "                if (c < 5) {\n"
           "                    for (int j = 0; j < d; j++) {\n"
           "                        if (j % 3 == 0) {\n"
           "                            if (e > 0) return 1;\n"
           "                            else return 2;\n"
           "                        } else if (j % 3 == 1) {\n"
           "                            return 3;\n"
           "                        } else {\n"
           "                            return 4;\n"
           "                        }\n"
           "                    }\n"
           "                } else if (c < 10) {\n"
           "                    return 5;\n"
           "                } else {\n"
           "                    return 6;\n"
           "                }\n"
           "            } else {\n"
           "                while (d > 0) {\n"
           "                    if (d % 3 == 0) break;\n"
           "                    d--;\n"
           "                }\n"
           "                return 7;\n"
           "            }\n"
           "        }\n"
           "    } else if (b > 10) {\n"
           "        return 8;\n"
           "    } else if (c > 10) {\n"
           "        return 9;\n"
           "    }\n"
           "    return 0;\n"
           "}\n";
}

// =============================================================================
// SESSION-BASED FRAGMENT DELIVERY TESTS
// =============================================================================

/* Test 1: Single session delivers one fragment per type maximum */
static int test_single_session_fragment_limits(void) {
    LOG("Testing single session fragment delivery limits");
    
    cleanup_state_file();
    metis_fragment_engine_init();
    
    // Reset session to ensure clean start
    metis_reset_session_fragments();
    
    // Record initial state
    int initial_docs = g_metis_mind->docs_fragments_delivered;
    int initial_daedalus = g_metis_mind->daedalus_fragments_delivered;
    int initial_linting = g_metis_mind->linting_fragments_delivered;
    int initial_philosophical = g_metis_mind->philosophical_fragments_delivered;
    
    // Create files with various violation types
    char* docs_file = create_temp_test_file("docs_test.c", create_undocumented_functions_content());
    char* daedalus_file = create_temp_test_file("daedalus_test.c", create_dangerous_functions_content());
    char* linting_file = create_temp_test_file("linting_test.c", create_missing_headers_content());
    char* philosophy_file = create_temp_test_file("philosophy_test.c", create_philosophical_violations_content());
    
    // Lint each file multiple times in the same session
    printf("  First round of linting...\n");
    metis_lint_file(docs_file);
    metis_lint_file(daedalus_file);
    metis_lint_file(linting_file);
    metis_lint_file(philosophy_file);
    
    // Record state after first round
    int after_first_docs = g_metis_mind->docs_fragments_delivered;
    int after_first_daedalus = g_metis_mind->daedalus_fragments_delivered;
    int after_first_linting = g_metis_mind->linting_fragments_delivered;
    int after_first_philosophical = g_metis_mind->philosophical_fragments_delivered;
    
    printf("  Second round of linting (should not deliver new fragments)...\n");
    metis_lint_file(docs_file);
    metis_lint_file(daedalus_file);
    metis_lint_file(linting_file);
    metis_lint_file(philosophy_file);
    
    // Record final state
    int final_docs = g_metis_mind->docs_fragments_delivered;
    int final_daedalus = g_metis_mind->daedalus_fragments_delivered;
    int final_linting = g_metis_mind->linting_fragments_delivered;
    int final_philosophical = g_metis_mind->philosophical_fragments_delivered;
    
    // Assertions: Each type should increment by at most 1 in the session
    TEST_ASSERT(after_first_docs - initial_docs <= 1, "Should deliver at most 1 docs fragment per session");
    TEST_ASSERT(after_first_daedalus - initial_daedalus <= 1, "Should deliver at most 1 daedalus fragment per session");
    TEST_ASSERT(after_first_linting - initial_linting <= 1, "Should deliver at most 1 linting fragment per session");
    TEST_ASSERT(after_first_philosophical - initial_philosophical <= 1, "Should deliver at most 1 philosophical fragment per session");
    
    // Second round should not deliver any additional fragments
    TEST_ASSERT(final_docs == after_first_docs, "Should not deliver additional docs fragments in same session");
    TEST_ASSERT(final_daedalus == after_first_daedalus, "Should not deliver additional daedalus fragments in same session");
    TEST_ASSERT(final_linting == after_first_linting, "Should not deliver additional linting fragments in same session");
    TEST_ASSERT(final_philosophical == after_first_philosophical, "Should not deliver additional philosophical fragments in same session");
    
    cleanup_temp_file(docs_file);
    cleanup_temp_file(daedalus_file);
    cleanup_temp_file(linting_file);
    cleanup_temp_file(philosophy_file);
    metis_fragment_engine_cleanup();
    return 1;
}

/* Test 2: New session allows new fragment delivery */
static int test_new_session_allows_new_fragments(void) {
    LOG("Testing new session allows new fragment delivery");
    
    cleanup_state_file();
    metis_fragment_engine_init();
    
    char* test_file = create_temp_test_file("session_test.c", create_dangerous_functions_content());
    
    // First session
    metis_reset_session_fragments();
    metis_lint_file(test_file);
    int first_session_count = g_metis_mind->daedalus_fragments_delivered;
    
    // Lint again in same session (should not increment)
    metis_lint_file(test_file);
    int same_session_count = g_metis_mind->daedalus_fragments_delivered;
    
    // Start new session
    metis_reset_session_fragments();
    metis_lint_file(test_file);
    int new_session_count = g_metis_mind->daedalus_fragments_delivered;
    
    TEST_ASSERT(first_session_count > 0, "Should deliver fragment in first session");
    TEST_ASSERT(same_session_count == first_session_count, "Should not deliver additional fragments in same session");
    TEST_ASSERT(new_session_count > same_session_count, "Should deliver new fragment in new session");
    TEST_ASSERT(new_session_count - same_session_count <= 1, "Should deliver at most 1 additional fragment per new session");
    
    cleanup_temp_file(test_file);
    metis_fragment_engine_cleanup();
    return 1;
}

// =============================================================================
// CONTEXT-SPECIFIC TECHNICAL GUIDANCE INTEGRATION TESTS
// =============================================================================

/* Test 3: Daedalus fragments with string violations provide context-specific guidance */
static int test_daedalus_string_context_integration(void) {
    LOG("Testing Daedalus string violation context integration");
    
    cleanup_state_file();
    metis_fragment_engine_init();
    metis_reset_session_fragments();
    
    const char* strcpy_content = 
        "/* strcpy_test.c - File with strcpy violations */\n"
        "// INSERT WISDOM HERE\n"
        "\n"
        "#include <string.h>\n"
        "\n"
        "void test_strcpy(void) {\n"
        "    char dest[100];\n"
        "    char* src = \"test\";\n"
        "    strcpy(dest, src);  // This should trigger dString guidance\n"
        "}\n";
    
    char* test_file = create_temp_test_file("strcpy_test.c", strcpy_content);
    
    int initial_daedalus = g_metis_mind->daedalus_fragments_delivered;
    
    // Lint the file - should trigger Daedalus fragment with string-specific guidance
    int violations = metis_lint_file(test_file);
    
    TEST_ASSERT(violations > 0, "Should detect strcpy violations");
    TEST_ASSERT(g_metis_mind->daedalus_fragments_delivered > initial_daedalus, "Should deliver Daedalus fragment for string violation");
    
    cleanup_temp_file(test_file);
    metis_fragment_engine_cleanup();
    return 1;
}

/* Test 4: Daedalus fragments with memory violations provide context-specific guidance */
static int test_daedalus_memory_context_integration(void) {
    LOG("Testing Daedalus memory violation context integration");
    
    cleanup_state_file();
    metis_fragment_engine_init();
    metis_reset_session_fragments();
    
    const char* malloc_content = 
        "/* malloc_test.c - File with memory management violations */\n"
        "// INSERT WISDOM HERE\n"
        "\n"
        "#include <stdlib.h>\n"
        "\n"
        "void test_malloc(void) {\n"
        "    int* ptr = malloc(100 * sizeof(int));  // Should trigger dArray guidance\n"
        "    // Missing free() is also a violation\n"
        "}\n";
    
    char* test_file = create_temp_test_file("malloc_test.c", malloc_content);
    
    int initial_daedalus = g_metis_mind->daedalus_fragments_delivered;
    
    int violations = metis_lint_file(test_file);
    
    TEST_ASSERT(violations > 0, "Should detect malloc violations");
    TEST_ASSERT(g_metis_mind->daedalus_fragments_delivered > initial_daedalus, "Should deliver Daedalus fragment for memory violation");
    
    cleanup_temp_file(test_file);
    metis_fragment_engine_cleanup();
    return 1;
}

/* Test 5: Daedalus fragments with logging violations provide context-specific guidance */
static int test_daedalus_logging_context_integration(void) {
    LOG("Testing Daedalus logging violation context integration");
    
    cleanup_state_file();
    metis_fragment_engine_init();
    metis_reset_session_fragments();
    
    const char* printf_content = 
        "/* printf_test.c - File with logging violations */\n"
        "// INSERT WISDOM HERE\n"
        "\n"
        "#include <stdio.h>\n"
        "\n"
        "void test_printf(void) {\n"
        "    printf(\"Debug info: %d\\n\", 42);  // Should trigger dLog guidance\n"
        "    fprintf(stderr, \"Error occurred\\n\");  // More logging violations\n"
        "}\n";
    
    char* test_file = create_temp_test_file("printf_test.c", printf_content);
    
    int initial_daedalus = g_metis_mind->daedalus_fragments_delivered;
    
    int violations = metis_lint_file(test_file);
    
    TEST_ASSERT(violations > 0, "Should detect printf violations");
    TEST_ASSERT(g_metis_mind->daedalus_fragments_delivered > initial_daedalus, "Should deliver Daedalus fragment for logging violation");
    
    cleanup_temp_file(test_file);
    metis_fragment_engine_cleanup();
    return 1;
}

// =============================================================================
// STORY PROGRESSION INTEGRATION TESTS
// =============================================================================

/* Test 6: Story fragments change based on wisdom level progression */
static int test_story_progression_through_levels(void) {
    LOG("Testing story progression through wisdom levels");
    
    cleanup_state_file();
    metis_fragment_engine_init();
    
    // Start at level 1
    TEST_ASSERT(g_metis_mind->current_wisdom_level == 1, "Should start at wisdom level 1");
    
    // Create a file that will trigger multiple fragment types
    char* test_file = create_temp_test_file("progression_test.c", create_dangerous_functions_content());
    
    int cycles = 0;
    int target_level = 5;  // Test progression to level 5 (still Act I)
    
    while (g_metis_mind->current_wisdom_level < target_level && cycles < 20) {
        metis_reset_session_fragments();
        metis_lint_file(test_file);
        cycles++;
        
        printf("  Cycle %d: Level %d, Points %d\n", 
               cycles, g_metis_mind->current_wisdom_level, g_metis_mind->total_wisdom_points);
    }
    
    TEST_ASSERT(g_metis_mind->current_wisdom_level >= target_level, "Should progress to target wisdom level");
    TEST_ASSERT(g_metis_mind->total_wisdom_points > 0, "Should accumulate wisdom points");
    TEST_ASSERT(cycles < 20, "Should reach target level within reasonable cycles");
    
    cleanup_temp_file(test_file);
    metis_fragment_engine_cleanup();
    return 1;
}

/* Test 7: Multiple violation types in single file trigger appropriate fragments */
static int test_multiple_violations_single_file(void) {
    LOG("Testing multiple violation types in single file");
    
    cleanup_state_file();
    metis_fragment_engine_init();
    metis_reset_session_fragments();
    
    const char* multi_violation_content = 
        "/* multi_violation.c - File with multiple violation types */\n"
        "// Missing wisdom header - DOCS violation\n"
        "\n"
        "#include <string.h>\n"
        "#include <stdio.h>\n"
        "#include <stdlib.h>\n"
        "\n"
        "int undocumented_function(int a, int b) {  // DOCS violation\n"
        "    char buffer[100];\n"
        "    strcpy(buffer, \"test\");  // DAEDALUS violation\n"
        "    printf(\"Debug: %d\\n\", a);  // DAEDALUS violation\n"
        "    // TODO: Fix this later  // PHILOSOPHICAL violation\n"
        "    return a + b;\n"
        "}\n";
    
    char* test_file = create_temp_test_file("multi_violation.c", multi_violation_content);
    
    int initial_docs = g_metis_mind->docs_fragments_delivered;
    int initial_daedalus = g_metis_mind->daedalus_fragments_delivered;
    int initial_philosophical = g_metis_mind->philosophical_fragments_delivered;
    
    int violations = metis_lint_file(test_file);
    
    TEST_ASSERT(violations > 0, "Should detect multiple violations");
    
    // Should deliver at most one fragment per type per session
    int docs_delivered = g_metis_mind->docs_fragments_delivered - initial_docs;
    int daedalus_delivered = g_metis_mind->daedalus_fragments_delivered - initial_daedalus;
    int philosophical_delivered = g_metis_mind->philosophical_fragments_delivered - initial_philosophical;
    
    TEST_ASSERT(docs_delivered <= 1, "Should deliver at most 1 docs fragment per session");
    TEST_ASSERT(daedalus_delivered <= 1, "Should deliver at most 1 daedalus fragment per session");
    TEST_ASSERT(philosophical_delivered <= 1, "Should deliver at most 1 philosophical fragment per session");
    
    // But should deliver at least some fragments for the violations found
    int total_delivered = docs_delivered + daedalus_delivered + philosophical_delivered;
    TEST_ASSERT(total_delivered > 0, "Should deliver at least one fragment for violations found");
    
    cleanup_temp_file(test_file);
    metis_fragment_engine_cleanup();
    return 1;
}

// =============================================================================
// PERSISTENCE AND STATE MANAGEMENT INTEGRATION TESTS
// =============================================================================

/* Test 8: Fragment delivery state persists across engine restarts */
static int test_fragment_state_persistence_integration(void) {
    LOG("Testing fragment delivery state persistence across restarts");
    
    cleanup_state_file();
    
    // First session
    metis_fragment_engine_init();
    metis_reset_session_fragments();
    
    char* test_file = create_temp_test_file("persistence_test.c", create_dangerous_functions_content());
    metis_lint_file(test_file);
    
    int session1_total = g_metis_mind->fragments_delivered_total;
    int session1_daedalus = g_metis_mind->daedalus_fragments_delivered;
    int session1_points = g_metis_mind->total_wisdom_points;
    
    TEST_ASSERT(session1_total > 0, "Should deliver fragments in first session");
    
    metis_fragment_engine_cleanup();
    
    // Second session - should restore state
    metis_fragment_engine_init();
    
    TEST_ASSERT(g_metis_mind->fragments_delivered_total == session1_total, "Should restore total fragment count");
    TEST_ASSERT(g_metis_mind->daedalus_fragments_delivered == session1_daedalus, "Should restore daedalus fragment count");
    TEST_ASSERT(g_metis_mind->total_wisdom_points == session1_points, "Should restore wisdom points");
    
    // New session allows new fragments
    metis_reset_session_fragments();
    metis_lint_file(test_file);
    
    TEST_ASSERT(g_metis_mind->fragments_delivered_total > session1_total, "Should deliver new fragments in new session");
    TEST_ASSERT(g_metis_mind->total_wisdom_points > session1_points, "Should gain new wisdom points");
    
    cleanup_temp_file(test_file);
    metis_fragment_engine_cleanup();
    return 1;
}

// =============================================================================
// PERFORMANCE AND EDGE CASE INTEGRATION TESTS
// =============================================================================

/* Test 9: Fragment engine handles multiple files efficiently */
static int test_multiple_files_performance(void) {
    LOG("Testing fragment engine performance with multiple files");
    
    cleanup_state_file();
    metis_fragment_engine_init();
    metis_reset_session_fragments();
    
    // Create multiple test files
    char* files[5];
    files[0] = create_temp_test_file("perf1.c", create_dangerous_functions_content());
    files[1] = create_temp_test_file("perf2.c", create_undocumented_functions_content());
    files[2] = create_temp_test_file("perf3.c", create_philosophical_violations_content());
    files[3] = create_temp_test_file("perf4.c", create_missing_headers_content());
    files[4] = create_temp_test_file("perf5.c", create_complex_function_content());
    
    int initial_total = g_metis_mind->fragments_delivered_total;
    
    // Lint all files
    for (int i = 0; i < 5; i++) {
        int violations = metis_lint_file(files[i]);
        TEST_ASSERT(violations >= 0, "Should handle file linting without errors");
    }
    
    int final_total = g_metis_mind->fragments_delivered_total;
    
    // Should deliver some fragments but respect session limits
    TEST_ASSERT(final_total > initial_total, "Should deliver fragments for multiple files");
    TEST_ASSERT(final_total - initial_total <= 4, "Should respect session limits (max 1 per type)");
    
    // Cleanup
    for (int i = 0; i < 5; i++) {
        cleanup_temp_file(files[i]);
    }
    
    metis_fragment_engine_cleanup();
    return 1;
}

/* Test 10: Fragment engine handles edge cases gracefully */
static int test_fragment_engine_edge_cases(void) {
    LOG("Testing fragment engine edge cases");
    
    cleanup_state_file();
    metis_fragment_engine_init();
    
    // Test with non-existent file
    int result1 = metis_lint_file("/tmp/nonexistent_file_12345.c");
    TEST_ASSERT(result1 == -1, "Should handle non-existent file gracefully");
    
    // Test with NULL file path
    int result2 = metis_lint_file(NULL);
    TEST_ASSERT(result2 == -1, "Should handle NULL file path gracefully");
    
    // Test with empty file
    char* empty_file = create_temp_test_file("empty.c", "");
    int result3 = metis_lint_file(empty_file);
    TEST_ASSERT(result3 >= 0, "Should handle empty file gracefully");
    
    // Fragment counts should remain consistent
    TEST_ASSERT(g_metis_mind->fragments_delivered_total >= 0, "Fragment counts should remain non-negative");
    TEST_ASSERT(g_metis_mind->total_wisdom_points >= 0, "Wisdom points should remain non-negative");
    
    cleanup_temp_file(empty_file);
    metis_fragment_engine_cleanup();
    return 1;
}

// =============================================================================
// MAIN TEST RUNNER
// =============================================================================

int main(void) {
    TEST_SUITE_START("Metis Fragment Engine Integration Tests");
    
    printf("\n=== SESSION-BASED FRAGMENT DELIVERY TESTS ===\n");
    RUN_TEST(test_single_session_fragment_limits);
    RUN_TEST(test_new_session_allows_new_fragments);
    
    printf("\n=== CONTEXT-SPECIFIC TECHNICAL GUIDANCE INTEGRATION ===\n");
    RUN_TEST(test_daedalus_string_context_integration);
    RUN_TEST(test_daedalus_memory_context_integration);
    RUN_TEST(test_daedalus_logging_context_integration);
    
    printf("\n=== STORY PROGRESSION INTEGRATION TESTS ===\n");
    RUN_TEST(test_story_progression_through_levels);
    RUN_TEST(test_multiple_violations_single_file);
    
    printf("\n=== PERSISTENCE AND STATE MANAGEMENT ===\n");
    RUN_TEST(test_fragment_state_persistence_integration);
    
    printf("\n=== PERFORMANCE AND EDGE CASES ===\n");
    RUN_TEST(test_multiple_files_performance);
    RUN_TEST(test_fragment_engine_edge_cases);
    
    TEST_SUITE_END();
}
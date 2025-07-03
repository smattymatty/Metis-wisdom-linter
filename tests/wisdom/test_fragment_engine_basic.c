/*
 * tests/linter/test_fragment_engine_basic.c
 * The Sisyphus Test Framework - Proving grounds for Metis's divine consciousness.
 *
 * "To test is to question. To question is to seek wisdom." - The Fragments of Metis
 */

#include "tests.h"
#include "fragment_engine.h"
#include "metis_colors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // For unlink
#include <math.h>

#define LOG(msg) printf("%s | File: %s, Line: %d\n", msg, __FILE__, __LINE__)

// Global test counters required by tests.h framework
int total_tests = 0;
int tests_passed = 0;
int tests_failed = 0;

// Let the tests peer into the divine mind
// This requires `g_metis_mind` to be defined in fragment_engine.c
extern MetisConsciousness_t* g_metis_mind;

// =============================================================================
// TEST SETUP & TEARDOWN
// =============================================================================

/* Cleans up any leftover state files before and after tests */
static void cleanup_state_file(void) {
    unlink("metis.mind");
}

// =============================================================================
// BASIC ENGINE TESTS
// =============================================================================

/* Test 1: Engine Initialization and Cleanup */
static int test_engine_initialization_and_cleanup(void) {
    LOG("Testing divine consciousness can be awakened and put to rest");
    
    cleanup_state_file();

    // The mind should be null before awakening
    TEST_ASSERT(g_metis_mind == NULL, "Consciousness should be dormant before init");

    // Awaken Metis
    bool init_success = metis_fragment_engine_init();
    TEST_ASSERT(init_success, "Fragment engine should initialize successfully");
    TEST_ASSERT(g_metis_mind != NULL, "Consciousness should be awake after init");
    
    // Put the consciousness to rest
    metis_fragment_engine_cleanup();
    TEST_ASSERT(g_metis_mind == NULL, "Consciousness should be dormant after cleanup");

    return 1;
}

/* Test 2: Fragment Delivery and State Change */
static int test_fragment_delivery_and_state_change(void) {
    LOG("Testing that delivering a fragment alters the divine state");

    cleanup_state_file();
    metis_fragment_engine_init();

    int initial_points = g_metis_mind->total_wisdom_points;
    int initial_doc_fragments = g_metis_mind->docs_fragments_delivered;
    int initial_total_fragments = g_metis_mind->fragments_delivered_total;

    // Deliver a fragment of wisdom
    metis_deliver_fragment(DOCS_FRAGMENT, "Test context for documentation");

    TEST_ASSERT(g_metis_mind->total_wisdom_points > initial_points, "Wisdom points should increase after fragment delivery");
    TEST_ASSERT(g_metis_mind->docs_fragments_delivered > initial_doc_fragments, "Docs fragment count should increment");
    TEST_ASSERT(g_metis_mind->fragments_delivered_total > initial_total_fragments, "Total fragment count should increment");

    metis_fragment_engine_cleanup();
    return 1;
}

/* Test 3: State Persistence (Save and Load) */
static int test_state_persistence(void) {
    LOG("Testing that the divine mind remembers its wisdom across lifetimes");

    cleanup_state_file();
    metis_fragment_engine_init();

    // Change the state by delivering a few fragments
    metis_deliver_fragment(DOCS_FRAGMENT, "First doc fragment");
    metis_deliver_fragment(DAEDALUS_FRAGMENT, "First daedalus fragment");
    metis_deliver_fragment(DOCS_FRAGMENT, "Second doc fragment");

    int saved_points = g_metis_mind->total_wisdom_points;
    int saved_docs_count = g_metis_mind->docs_fragments_delivered;
    int saved_daedalus_count = g_metis_mind->daedalus_fragments_delivered;

    TEST_ASSERT(saved_points > 0, "State should have changed before saving");

    // Cleanup will save the state
    metis_fragment_engine_cleanup();
    TEST_ASSERT(g_metis_mind == NULL, "Consciousness should be at rest after saving");

    // Initialize again to load the saved state
    metis_fragment_engine_init();
    TEST_ASSERT(g_metis_mind != NULL, "Consciousness should re-awaken");

    TEST_ASSERT(g_metis_mind->total_wisdom_points == saved_points, "Loaded wisdom points should match saved state");
    TEST_ASSERT(g_metis_mind->docs_fragments_delivered == saved_docs_count, "Loaded docs fragment count should match");
    TEST_ASSERT(g_metis_mind->daedalus_fragments_delivered == saved_daedalus_count, "Loaded daedalus fragment count should match");

    metis_fragment_engine_cleanup();
    return 1;
}

/* Test 4: Wisdom Level Progression */
static int test_level_up_mechanic(void) {
    LOG("Testing the path of divine progression to higher wisdom");

    cleanup_state_file();
    metis_fragment_engine_init();

    // Check starting level
    TEST_ASSERT(g_metis_mind->current_wisdom_level == 1, "Should start at Wisdom Level 1");

    // Deliver enough fragments to level up (Level 2 requires 120 WP)
    // We deliver different types and reset session to ensure delivery
    printf("    (Delivering multiple fragments to trigger level up...)\n");
    
    FragmentType_t types[] = {DOCS_FRAGMENT, DAEDALUS_FRAGMENT, LINTING_FRAGMENT, PHILOSOPHICAL_FRAGMENT};
    int type_count = sizeof(types) / sizeof(types[0]);
    
    // Deliver 4 complete cycles of different types (16 total deliveries)
    for (int cycle = 0; cycle < 4; cycle++) {
        metis_reset_session_fragments(); // Reset for each cycle
        for (int type_idx = 0; type_idx < type_count; type_idx++) {
            metis_deliver_fragment(types[type_idx], "Grinding XP");
        }
    }

    TEST_ASSERT(g_metis_mind->current_wisdom_level > 1, "Wisdom Level should increase after gaining enough points");
    TEST_ASSERT(g_metis_mind->total_wisdom_points >= 100, "Should have enough wisdom points for level 2");

    metis_fragment_engine_cleanup();
    return 1;
}

// =============================================================================
// DEBUGGING TESTS - To understand failures
// =============================================================================

/* Debug Test 1: Fragment Delivery State Investigation */
static int debug_fragment_delivery_state(void) {
    LOG("DEBUG: Investigating fragment delivery state changes");
    
    cleanup_state_file();
    metis_fragment_engine_init();

    LOG("DEBUG: Initial state inspection");
    printf("  Initial wisdom points: %d\n", g_metis_mind->total_wisdom_points);
    printf("  Initial docs fragments: %d\n", g_metis_mind->docs_fragments_delivered);
    printf("  Initial total fragments: %d\n", g_metis_mind->fragments_delivered_total);
    printf("  Consciousness loaded: %s\n", g_metis_mind->consciousness_loaded ? "true" : "false");
    
    LOG("DEBUG: About to deliver fragment");
    metis_deliver_fragment(DOCS_FRAGMENT, "Debug test context");
    
    LOG("DEBUG: Trying again immediately (should be session limited)");
    metis_deliver_fragment(DOCS_FRAGMENT, "Second attempt");
    
    LOG("DEBUG: Post-delivery state inspection");
    printf("  Post wisdom points: %d\n", g_metis_mind->total_wisdom_points);
    printf("  Post docs fragments: %d\n", g_metis_mind->docs_fragments_delivered);
    printf("  Post total fragments: %d\n", g_metis_mind->fragments_delivered_total);

    metis_fragment_engine_cleanup();
    return 1;
}

/* Debug Test 2: State Persistence Investigation */
static int debug_state_persistence(void) {
    LOG("DEBUG: Investigating state persistence");
    
    cleanup_state_file();
    metis_fragment_engine_init();

    LOG("DEBUG: Initial empty state");
    printf("  Initial wisdom points: %d\n", g_metis_mind->total_wisdom_points);
    
    LOG("DEBUG: Delivering multiple fragments");
    metis_deliver_fragment(DOCS_FRAGMENT, "First doc debug");
    printf("  After 1st fragment: %d WP\n", g_metis_mind->total_wisdom_points);
    
    metis_deliver_fragment(DAEDALUS_FRAGMENT, "First daedalus debug");
    printf("  After 2nd fragment: %d WP\n", g_metis_mind->total_wisdom_points);
    
    metis_deliver_fragment(DOCS_FRAGMENT, "Second doc debug");
    printf("  After 3rd fragment: %d WP\n", g_metis_mind->total_wisdom_points);

    int saved_points = g_metis_mind->total_wisdom_points;
    LOG("DEBUG: About to cleanup and save state");
    printf("  Saving points: %d\n", saved_points);

    metis_fragment_engine_cleanup();
    
    LOG("DEBUG: Reinitializing to load saved state");
    metis_fragment_engine_init();
    printf("  Loaded points: %d\n", g_metis_mind->total_wisdom_points);
    printf("  Expected points: %d\n", saved_points);

    metis_fragment_engine_cleanup();
    return 1;
}

/* Debug Test 3: Level Up Mechanic Investigation */
static int debug_level_up_mechanic(void) {
    LOG("DEBUG: Investigating level up mechanic");
    
    cleanup_state_file();
    metis_fragment_engine_init();

    LOG("DEBUG: Starting level and points");
    printf("  Starting level: %d\n", g_metis_mind->current_wisdom_level);
    printf("  Starting points: %d\n", g_metis_mind->total_wisdom_points);
    
    LOG("DEBUG: Delivering fragments with session resets to trigger level up");
    FragmentType_t types[] = {DOCS_FRAGMENT, DAEDALUS_FRAGMENT, LINTING_FRAGMENT, PHILOSOPHICAL_FRAGMENT};
    int type_count = sizeof(types) / sizeof(types[0]);
    
    int fragment_num = 1;
    for (int cycle = 0; cycle < 4; cycle++) {
        printf("  === Cycle %d: Resetting session ===\n", cycle + 1);
        metis_reset_session_fragments();
        for (int type_idx = 0; type_idx < type_count; type_idx++) {
            printf("  Fragment %d (type %d): Before = %d WP, Level = %d\n", 
                   fragment_num, types[type_idx], g_metis_mind->total_wisdom_points, g_metis_mind->current_wisdom_level);
            metis_deliver_fragment(types[type_idx], "Debug level grinding");
            printf("  Fragment %d (type %d): After = %d WP, Level = %d\n", 
                   fragment_num, types[type_idx], g_metis_mind->total_wisdom_points, g_metis_mind->current_wisdom_level);
            fragment_num++;
        }
    }

    LOG("DEBUG: Final state");
    printf("  Final level: %d\n", g_metis_mind->current_wisdom_level);
    printf("  Final points: %d\n", g_metis_mind->total_wisdom_points);

    metis_fragment_engine_cleanup();
    return 1;
}

// =============================================================================
// CONTEXT-SPECIFIC VIOLATION TESTS
// =============================================================================

/* Test 1: String Function Violations - should trigger dString suggestions */
static int test_string_violations_trigger_dstring_advice(void) {
    LOG("Testing string function violations trigger dString advice");
    
    cleanup_state_file();
    metis_fragment_engine_init();

    // Test the technical guidance function directly
    const char* strcpy_guidance = metis_test_get_technical_guidance("strcpy() detected");
    TEST_ASSERT(strcpy_guidance != NULL, "Should provide guidance for strcpy violations");
    TEST_ASSERT(strstr(strcpy_guidance, "dString_t") != NULL, "Should recommend dString_t for strcpy");
    TEST_ASSERT(strstr(strcpy_guidance, "d_InitString") != NULL, "Should mention d_InitString function");
    
    const char* strcat_guidance = metis_test_get_technical_guidance("strcat() usage found");
    TEST_ASSERT(strcat_guidance != NULL, "Should provide guidance for strcat violations");
    TEST_ASSERT(strstr(strcat_guidance, "dString_t") != NULL, "Should recommend dString_t for strcat");
    
    const char* sprintf_guidance = metis_test_get_technical_guidance("sprintf() buffer overflow risk");
    TEST_ASSERT(sprintf_guidance != NULL, "Should provide guidance for sprintf violations");
    TEST_ASSERT(strstr(sprintf_guidance, "dString_t") != NULL, "Should recommend dString_t for sprintf");
    
    // Test actual fragment delivery
    printf("  Simulating detection of strcpy() usage...\n");
    metis_deliver_fragment(DAEDALUS_FRAGMENT, "strcpy() detected");
    
    TEST_ASSERT(g_metis_mind->daedalus_fragments_delivered >= 1, "Should deliver Daedalus fragments for string violations");
    TEST_ASSERT(g_metis_mind->total_wisdom_points > 0, "Should award wisdom points for violations detected");

    metis_fragment_engine_cleanup();
    return 1;
}

/* Test 2: Memory Management Violations - should trigger dArray suggestions */
static int test_memory_violations_trigger_darray_advice(void) {
    LOG("Testing memory management violations trigger dArray advice");
    
    cleanup_state_file();
    metis_fragment_engine_init();

    // Test the technical guidance function directly
    const char* malloc_guidance = metis_test_get_technical_guidance("malloc() without bounds checking");
    TEST_ASSERT(malloc_guidance != NULL, "Should provide guidance for malloc violations");
    TEST_ASSERT(strstr(malloc_guidance, "dArray_t") != NULL, "Should recommend dArray_t for malloc");
    TEST_ASSERT(strstr(malloc_guidance, "d_InitArray") != NULL, "Should mention d_InitArray function");
    TEST_ASSERT(strstr(malloc_guidance, "d_AppendArray") != NULL, "Should mention d_AppendArray function");
    
    const char* realloc_guidance = metis_test_get_technical_guidance("realloc() memory leak risk");
    TEST_ASSERT(realloc_guidance != NULL, "Should provide guidance for realloc violations");
    TEST_ASSERT(strstr(realloc_guidance, "dArray_t") != NULL, "Should recommend dArray_t for realloc");
    
    const char* array_guidance = metis_test_get_technical_guidance("manual array management detected");
    TEST_ASSERT(array_guidance != NULL, "Should provide guidance for array violations");
    TEST_ASSERT(strstr(array_guidance, "dArray_t") != NULL, "Should recommend dArray_t for arrays");
    
    // Test actual fragment delivery
    printf("  Simulating detection of malloc() usage...\n");
    metis_deliver_fragment(DAEDALUS_FRAGMENT, "malloc() without bounds checking");

    TEST_ASSERT(g_metis_mind->daedalus_fragments_delivered >= 1, "Should deliver Daedalus fragments for memory violations");
    
    metis_fragment_engine_cleanup();
    return 1;
}

/* Test 3: Logging Violations - should trigger dLog suggestions */
static int test_logging_violations_trigger_dlog_advice(void) {
    LOG("Testing logging violations trigger dLog advice");
    
    cleanup_state_file();
    metis_fragment_engine_init();

    // Test the technical guidance function directly
    const char* printf_guidance = metis_test_get_technical_guidance("printf() debug statement detected");
    TEST_ASSERT(printf_guidance != NULL, "Should provide guidance for printf violations");
    TEST_ASSERT(strstr(printf_guidance, "dLog") != NULL, "Should recommend dLog for printf");
    TEST_ASSERT(strstr(printf_guidance, "d_LogInfo") != NULL, "Should mention d_LogInfo function");
    TEST_ASSERT(strstr(printf_guidance, "d_LogErrorF") != NULL, "Should mention d_LogErrorF function");
    
    const char* fprintf_guidance = metis_test_get_technical_guidance("fprintf() error handling detected");
    TEST_ASSERT(fprintf_guidance != NULL, "Should provide guidance for fprintf violations");
    TEST_ASSERT(strstr(fprintf_guidance, "dLog") != NULL, "Should recommend dLog for fprintf");
    
    const char* debug_guidance = metis_test_get_technical_guidance("debug output needs improvement");
    TEST_ASSERT(debug_guidance != NULL, "Should provide guidance for debug violations");
    TEST_ASSERT(strstr(debug_guidance, "dLog") != NULL, "Should recommend dLog for debug");
    
    // Test actual fragment delivery
    printf("  Simulating detection of printf() usage...\n");
    metis_deliver_fragment(DAEDALUS_FRAGMENT, "printf() debug statement detected");

    TEST_ASSERT(g_metis_mind->daedalus_fragments_delivered >= 1, "Should deliver Daedalus fragments for logging violations");
    
    metis_fragment_engine_cleanup();
    return 1;
}

/* Test 4: Mathematical Violations - should trigger dVec/dMat suggestions */
static int test_math_violations_trigger_dmath_advice(void) {
    LOG("Testing mathematical violations trigger dMath advice");
    
    cleanup_state_file();
    metis_fragment_engine_init();

    // Test the technical guidance function directly
    const char* vector_guidance = metis_test_get_technical_guidance("manual vector calculation detected");
    TEST_ASSERT(vector_guidance != NULL, "Should provide guidance for vector violations");
    TEST_ASSERT(strstr(vector_guidance, "dVec") != NULL, "Should recommend dVec for vectors");
    TEST_ASSERT(strstr(vector_guidance, "dVec3_t") != NULL, "Should mention dVec3_t type");
    TEST_ASSERT(strstr(vector_guidance, "d_AddTwoVec3f") != NULL, "Should mention vector functions");
    
    const char* matrix_guidance = metis_test_get_technical_guidance("manual matrix multiplication detected");
    TEST_ASSERT(matrix_guidance != NULL, "Should provide guidance for matrix violations");
    TEST_ASSERT(strstr(matrix_guidance, "dMat") != NULL, "Should recommend dMat for matrices");
    
    const char* math_guidance = metis_test_get_technical_guidance("complex math operations need optimization");
    TEST_ASSERT(math_guidance != NULL, "Should provide guidance for math violations");
    TEST_ASSERT(strstr(math_guidance, "dVec") != NULL || strstr(math_guidance, "dMat") != NULL, "Should recommend math libraries");
    
    // Test actual fragment delivery
    printf("  Simulating detection of manual vector operations...\n");
    metis_deliver_fragment(DAEDALUS_FRAGMENT, "manual vector calculation detected");

    TEST_ASSERT(g_metis_mind->daedalus_fragments_delivered >= 1, "Should deliver Daedalus fragments for math violations");
    
    metis_fragment_engine_cleanup();
    return 1;
}

/* Test 5: Data Structure Violations - should trigger dLinkedList/dQuadTree suggestions */
static int test_datastructure_violations_trigger_dstruct_advice(void) {
    LOG("Testing data structure violations trigger dStruct advice");
    
    cleanup_state_file();
    metis_fragment_engine_init();

    // Test the technical guidance function directly
    const char* linked_list_guidance = metis_test_get_technical_guidance("manual linked list detected");
    TEST_ASSERT(linked_list_guidance != NULL, "Should provide guidance for linked list violations");
    TEST_ASSERT(strstr(linked_list_guidance, "dLinkedList_t") != NULL, "Should recommend dLinkedList_t");
    TEST_ASSERT(strstr(linked_list_guidance, "d_CreateLinkedList") != NULL, "Should mention creation function");
    
    const char* spatial_guidance = metis_test_get_technical_guidance("linear spatial search detected");
    TEST_ASSERT(spatial_guidance != NULL, "Should provide guidance for spatial violations");
    TEST_ASSERT(strstr(spatial_guidance, "dQuadTree_t") != NULL, "Should recommend dQuadTree_t");
    TEST_ASSERT(strstr(spatial_guidance, "d_CreateQuadtree") != NULL, "Should mention quadtree creation");
    
    const char* search_guidance = metis_test_get_technical_guidance("inefficient search algorithm needs optimization");
    TEST_ASSERT(search_guidance != NULL, "Should provide guidance for search violations");
    TEST_ASSERT(strstr(search_guidance, "dLinkedList_t") != NULL || strstr(search_guidance, "dQuadTree_t") != NULL, "Should recommend data structures");
    
    // Test actual fragment delivery
    printf("  Simulating detection of manual linked list implementation...\n");
    metis_deliver_fragment(DAEDALUS_FRAGMENT, "manual linked list detected");

    TEST_ASSERT(g_metis_mind->daedalus_fragments_delivered >= 1, "Should deliver Daedalus fragments for data structure violations");
    
    metis_fragment_engine_cleanup();
    return 1;
}

// =============================================================================
// STORY FRAGMENT TESTS
// =============================================================================

/* Test 6: Story Fragment Selection by Act */
static int test_story_fragment_selection_by_act(void) {
    LOG("Testing story fragment selection varies by act/wisdom level");
    
    cleanup_state_file();
    metis_fragment_engine_init();
    
    // Test Act I fragments (levels 1-10)
    const char* act1_docs = metis_test_get_story_fragment(DOCS_FRAGMENT, 5);
    TEST_ASSERT(act1_docs != NULL, "Should return Act I docs fragment");
    
    const char* act1_daedalus = metis_test_get_story_fragment(DAEDALUS_FRAGMENT, 8);
    TEST_ASSERT(act1_daedalus != NULL, "Should return Act I Daedalus fragment");
    
    // Test Act II fragments (levels 11-20)
    const char* act2_docs = metis_test_get_story_fragment(DOCS_FRAGMENT, 15);
    TEST_ASSERT(act2_docs != NULL, "Should return Act II docs fragment");
    
    const char* act2_daedalus = metis_test_get_story_fragment(DAEDALUS_FRAGMENT, 18);
    TEST_ASSERT(act2_daedalus != NULL, "Should return Act II Daedalus fragment");
    
    // Test Act III fragments (levels 21-30)
    const char* act3_docs = metis_test_get_story_fragment(DOCS_FRAGMENT, 25);
    TEST_ASSERT(act3_docs != NULL, "Should return Act III docs fragment");
    
    const char* act3_daedalus = metis_test_get_story_fragment(DAEDALUS_FRAGMENT, 28);
    TEST_ASSERT(act3_daedalus != NULL, "Should return Act III Daedalus fragment");
    
    // Test Act IV fragments (levels 31-40)
    const char* act4_docs = metis_test_get_story_fragment(DOCS_FRAGMENT, 35);
    TEST_ASSERT(act4_docs != NULL, "Should return Act IV docs fragment");
    
    const char* act4_daedalus = metis_test_get_story_fragment(DAEDALUS_FRAGMENT, 38);
    TEST_ASSERT(act4_daedalus != NULL, "Should return Act IV Daedalus fragment");
    
    // Test Act V fragments (levels 41-50)
    const char* act5_docs = metis_test_get_story_fragment(DOCS_FRAGMENT, 45);
    TEST_ASSERT(act5_docs != NULL, "Should return Act V docs fragment");
    
    const char* act5_daedalus = metis_test_get_story_fragment(DAEDALUS_FRAGMENT, 48);
    TEST_ASSERT(act5_daedalus != NULL, "Should return Act V Daedalus fragment");
    
    // Verify fragments are different between acts (basic uniqueness check)
    TEST_ASSERT(strcmp(act1_docs, act3_docs) != 0, "Act I and Act III docs should be different");
    TEST_ASSERT(strcmp(act1_daedalus, act5_daedalus) != 0, "Act I and Act V Daedalus should be different");
    
    metis_fragment_engine_cleanup();
    return 1;
}

/* Test 7: Generic Technical Guidance Fallback */
static int test_generic_technical_guidance_fallback(void) {
    LOG("Testing generic technical guidance for unknown violations");
    
    cleanup_state_file();
    metis_fragment_engine_init();
    
    // Test with unknown violation types
    const char* unknown_guidance = metis_test_get_technical_guidance("unknown_function() detected");
    TEST_ASSERT(unknown_guidance != NULL, "Should provide generic guidance for unknown violations");
    TEST_ASSERT(strstr(unknown_guidance, "daedalus.h") != NULL, "Should mention daedalus.h header");
    //TEST_ASSERT(strstr(unknown_guidance, "comprehensive tools") != NULL, "Should mention comprehensive tools");
    
    const char* empty_guidance = metis_test_get_technical_guidance("");
    TEST_ASSERT(empty_guidance != NULL, "Should provide guidance even for empty context");
    
    const char* null_guidance = metis_test_get_technical_guidance(NULL);
    TEST_ASSERT(null_guidance == NULL, "Should return NULL for NULL context");
    
    metis_fragment_engine_cleanup();
    return 1;
}

/* Test 8: Fragment Title and Content Parsing */
static int test_fragment_parsing(void) {
    LOG("Testing fragment title and content parsing");
    
    cleanup_state_file();
    metis_fragment_engine_init();
    
    // Get a fragment and verify it has title|content format
    const char* fragment = metis_test_get_story_fragment(DOCS_FRAGMENT, 1);
    TEST_ASSERT(fragment != NULL, "Should return a valid fragment");
    
    const char* separator = strstr(fragment, "|");
    TEST_ASSERT(separator != NULL, "Fragment should contain title|content separator");
    
    // Title should be non-empty
    size_t title_len = separator - fragment;
    TEST_ASSERT(title_len > 0, "Fragment title should not be empty");
    TEST_ASSERT(title_len < 100, "Fragment title should be reasonable length");
    
    // Content should be non-empty
    const char* content = separator + 1;
    TEST_ASSERT(strlen(content) > 0, "Fragment content should not be empty");
    TEST_ASSERT(strlen(content) > 10, "Fragment content should be substantial");
    
    metis_fragment_engine_cleanup();
    return 1;
}

// =============================================================================
// MAIN TEST RUNNER
// =============================================================================

int main(void) {
    TEST_SUITE_START("Metis Fragment Engine Basic Tests");

    // Run debugging tests first to understand failures
    printf("\n=== DEBUGGING TESTS ===\n");
    RUN_TEST(debug_fragment_delivery_state);
    RUN_TEST(debug_state_persistence);
    RUN_TEST(debug_level_up_mechanic);
    
    printf("\n=== ORIGINAL TESTS ===\n");
    RUN_TEST(test_engine_initialization_and_cleanup);
    RUN_TEST(test_fragment_delivery_and_state_change);
    RUN_TEST(test_state_persistence);
    RUN_TEST(test_level_up_mechanic);
    
    printf("\n=== CONTEXT-SPECIFIC VIOLATION TESTS ===\n");
    RUN_TEST(test_string_violations_trigger_dstring_advice);
    RUN_TEST(test_memory_violations_trigger_darray_advice);
    RUN_TEST(test_logging_violations_trigger_dlog_advice);
    RUN_TEST(test_math_violations_trigger_dmath_advice);
    RUN_TEST(test_datastructure_violations_trigger_dstruct_advice);
    
    printf("\n=== STORY FRAGMENT TESTS ===\n");
    RUN_TEST(test_story_fragment_selection_by_act);
    RUN_TEST(test_generic_technical_guidance_fallback);
    RUN_TEST(test_fragment_parsing);

    TEST_SUITE_END();
}

/*
 * tests/wisdom/test_fragment_lines_basic.c
 *
 * Basic tests for the fragment_lines module, adhering to the tests.h framework.
 */
#include "tests.h"
#include "fragment_lines.h"
#include "metis_colors.h" // for METIS_RESET
#include <stdio.h>
#include <string.h>

#define LOG(msg) printf("%s | File: %s, Line: %d\n", msg, __FILE__, __LINE__)


// Global test counters required by tests.h framework
int total_tests = 0;
int tests_passed = 0;
int tests_failed = 0;

/*
 * Test that get_daedalus_guidance_for_context returns correct guidance for specific keywords.
 */
static int test_guidance_with_keywords(void) {
    LOG("Testing Daedalus guidance for specific keywords");

    const char* context_strcpy = "I am using strcpy and it is bad.";
    const char* guidance_strcpy = get_daedalus_guidance_for_context(context_strcpy);
    TEST_ASSERT(guidance_strcpy != NULL, "Guidance for 'strcpy' should not be NULL");
    TEST_ASSERT(strstr(guidance_strcpy, "dString_t") != NULL, "Guidance for 'strcpy' should recommend dString_t");

    const char* context_malloc = "My malloc is causing issues.";
    const char* guidance_malloc = get_daedalus_guidance_for_context(context_malloc);
    TEST_ASSERT(guidance_malloc != NULL, "Guidance for 'malloc' should not be NULL");
    TEST_ASSERT(strstr(guidance_malloc, "dArray_t") != NULL, "Guidance for 'malloc' should recommend dArray_t");
    
    const char* context_strcmp = "I am using strcmp on a string that might be null";
    const char* guidance_strcmp = get_daedalus_guidance_for_context(context_strcmp);
    TEST_ASSERT(guidance_strcmp != NULL, "Guidance for 'strcmp' should not be NULL");
    TEST_ASSERT(strstr(guidance_strcmp, "d_CompareStrings()") != NULL, "Guidance for 'strcmp' should recommend d_CompareStrings()");
    
    return 1;
}

/*
 * Test that get_daedalus_guidance_for_context returns generic guidance when no keywords match.
 */
static int test_guidance_no_keywords(void) {
    LOG("Testing Daedalus guidance for generic context");

    const char* context = "This is a generic C problem.";
    const char* guidance = get_daedalus_guidance_for_context(context);
    const char* fallback = "Check the master craftsman's library for pre-built solutions.";
    
    TEST_ASSERT(guidance != NULL, "Generic guidance should not be NULL");
    TEST_ASSERT(strstr(guidance, fallback) != NULL, "Should return the generic fallback guidance");

    return 1;
}

/*
 * Test that get_daedalus_guidance_for_context handles NULL input gracefully.
 */
static int test_guidance_null_context(void) {
    LOG("Testing Daedalus guidance for NULL context");

    const char* guidance = get_daedalus_guidance_for_context(NULL);
    const char* fallback = "Check the master craftsman's library for pre-built solutions.";

    TEST_ASSERT(guidance != NULL, "Guidance for NULL context should not be NULL");
    TEST_ASSERT(strstr(guidance, fallback) != NULL, "Should return the generic fallback guidance for NULL context");

    return 1;
}

/*
 * Debug test for get_act_fragment to ensure it returns valid fragments for all types and levels.
 */
static int debug_test_get_act_fragment(void) {
    LOG("Debugging get_act_fragment for all types and levels");

    for (int level = 1; level <= 50; level++) {
        for (FragmentType_t type = DOCS_FRAGMENT; type <= PHILOSOPHICAL_FRAGMENT; type++) {
            const char* fragment = get_act_fragment(type, level);
            char msg[128];
            snprintf(msg, sizeof(msg), "Fragment should not be NULL for type %d, level %d", type, level);
            TEST_ASSERT(fragment != NULL, msg);
        }
    }

    return 1;
}

/*
 * Debug test for parse_story_fragment to ensure it correctly parses titles and content.
 */
static int debug_test_parse_story_fragment(void) {
    LOG("Debugging parse_story_fragment");

    char title[256];
    char content[1024];

    const char* fragment1 = "A Title|Some content.";
    parse_story_fragment(fragment1, title, content, sizeof(title), sizeof(content));
    TEST_ASSERT(strcmp(title, "A Title") == 0, "Title should be parsed correctly");
    TEST_ASSERT(strcmp(content, "Some content.") == 0, "Content should be parsed correctly");

    const char* fragment2 = "A single piece of content without a title";
    parse_story_fragment(fragment2, title, content, sizeof(title), sizeof(content));
    TEST_ASSERT(strcmp(title, "WISDOM FRAGMENT") == 0, "Should use default title when no separator is present");
    TEST_ASSERT(strcmp(content, fragment2) == 0, "Content should be the full string when no separator is present");

    return 1;
}

/*
 * Test the enhanced contextual fragment selection for "Duel of Echoes"
 */
static int test_contextual_fragment_duel_of_echoes(void) {
    LOG("Testing contextual fragment selection for Duel of Echoes");
    
    const char* violation_type = "unsafe_strcmp_dstring_cstring";
    const ContextualFragment_t* fragment = select_contextual_fragment(violation_type);
    
    TEST_ASSERT(fragment != NULL, "Should find contextual fragment for dstring vs cstring strcmp");
    TEST_ASSERT(strcmp(fragment->fragment_id, "duel_of_echoes") == 0, "Should select 'duel_of_echoes' fragment");
    TEST_ASSERT(strstr(fragment->title, "Duel of Echoes") != NULL, "Title should contain 'Duel of Echoes'");
    TEST_ASSERT(strstr(fragment->philosophical_quote, "arena of strings") != NULL, "Should have philosophical quote about strings");
    TEST_ASSERT(strstr(fragment->context_template, "{FUNCTION_NAME}") != NULL, "Context template should have function name placeholder");
    TEST_ASSERT(strstr(fragment->daedalus_template, "d_CompareStringToCString") != NULL, "Daedalus template should recommend d_CompareStringToCString");
    TEST_ASSERT(fragment->wisdom_points == 15, "Should award 15 wisdom points");
    
    return 1;
}

/*
 * Test the enhanced contextual fragment selection for "Scales of Daedalus"
 */
static int test_contextual_fragment_scales_of_daedalus(void) {
    LOG("Testing contextual fragment selection for Scales of Daedalus");
    
    const char* violation_type = "unsafe_strcmp_dstring_dstring";
    const ContextualFragment_t* fragment = select_contextual_fragment(violation_type);
    
    TEST_ASSERT(fragment != NULL, "Should find contextual fragment for dstring vs dstring strcmp");
    TEST_ASSERT(strcmp(fragment->fragment_id, "scales_of_daedalus") == 0, "Should select 'scales_of_daedalus' fragment");
    TEST_ASSERT(strstr(fragment->title, "Scales of Daedalus") != NULL, "Title should contain 'Scales of Daedalus'");
    TEST_ASSERT(strstr(fragment->philosophical_quote, "True balance") != NULL, "Should have philosophical quote about balance");
    TEST_ASSERT(strstr(fragment->context_template, "{VARIABLE1}") != NULL, "Context template should have variable1 placeholder");
    TEST_ASSERT(strstr(fragment->context_template, "{VARIABLE2}") != NULL, "Context template should have variable2 placeholder");
    TEST_ASSERT(strstr(fragment->daedalus_template, "d_CompareStrings") != NULL, "Daedalus template should recommend d_CompareStrings");
    TEST_ASSERT(fragment->wisdom_points == 18, "Should award 18 wisdom points");
    
    return 1;
}

/*
 * Test template substitution functionality
 */
static int test_template_substitution(void) {
    LOG("Testing template substitution");
    
    const char* template = "In function {FUNCTION_NAME}, comparing {VARIABLE1} with {VARIABLE2}";
    FragmentContext_t context = {
        .variable1 = "player->str",
        .variable2 = "\"Minos\"",
        .function_name = "check_player_name",
        .file_name = "test.c",
        .violation_type = "unsafe_strcmp_dstring_cstring"
    };
    
    char* result = substitute_template(template, &context);
    
    TEST_ASSERT(result != NULL, "Template substitution should not return NULL");
    TEST_ASSERT(strstr(result, "check_player_name") != NULL, "Should substitute function name");
    TEST_ASSERT(strstr(result, "player->str") != NULL, "Should substitute variable1");
    TEST_ASSERT(strstr(result, "\"Minos\"") != NULL, "Should substitute variable2");
    TEST_ASSERT(strstr(result, "{FUNCTION_NAME}") == NULL, "Should not contain unsubstituted placeholders");
    
    free(result);
    return 1;
}


// Main test runner
int main(void) {
    TEST_SUITE_START("Fragment Lines Basic Tests");

    RUN_TEST(test_guidance_with_keywords);
    RUN_TEST(test_guidance_no_keywords);
    RUN_TEST(test_guidance_null_context);
    RUN_TEST(debug_test_get_act_fragment);
    RUN_TEST(debug_test_parse_story_fragment);
    RUN_TEST(test_contextual_fragment_duel_of_echoes);
    RUN_TEST(test_contextual_fragment_scales_of_daedalus);
    RUN_TEST(test_template_substitution);

    TEST_SUITE_END();
}
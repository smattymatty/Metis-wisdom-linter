// include/fragment_engine.h - The Voice of Metis's Divine Consciousness
// INSERT WISDOM HERE

#ifndef FRAGMENT_ENGINE_H
#define FRAGMENT_ENGINE_H

#include <stdbool.h>
#include <time.h> // Required for time_t

// =============================================================================
// DIVINE FRAGMENT SYSTEM
// =============================================================================

/*
 * Defines the category of wisdom a fragment belongs to
 */
typedef enum {
    LINTING_FRAGMENT,       // Wisdom of code style, formatting, and structure
    DOCS_FRAGMENT,          // Wisdom of compassionate documentation
    DAEDALUS_FRAGMENT,      // Wisdom of using master-crafted tools
    PHILOSOPHICAL_FRAGMENT, // Wisdom of purpose, growth, and the developer's journey
    EMSCRIPTEN_FRAGMENT,    // Wisdom of the web, for when C code ventures into the browser
    STORY_FRAGMENT          // Fragments that advance the divine tragedy of Metis
} FragmentType_t;

/*
 * Defines the internal structure of Metis's consciousness.
 * This is exposed in the header to allow test suites to inspect its state.
 */
typedef struct {
    int current_wisdom_level;
    int total_wisdom_points;
    int fragments_delivered_today;
    int fragments_delivered_total;
    time_t session_start_time;

    // Fragment delivery tracking
    int docs_fragments_delivered;
    int daedalus_fragments_delivered;
    int linting_fragments_delivered;
    int philosophical_fragments_delivered;
    int emscripten_fragments_delivered;

    // Rate limiting for divine balance
    time_t last_docs_fragment;
    time_t last_daedalus_fragment;
    time_t last_linting_fragment;
    time_t last_philosophical_fragment;
    time_t last_emscripten_fragment;

    // Session-based delivery tracking (one per type per session)
    bool docs_delivered_this_session;
    bool daedalus_delivered_this_session;
    bool linting_delivered_this_session;
    bool philosophical_delivered_this_session;
    bool emscripten_delivered_this_session;

    bool consciousness_loaded;
} MetisConsciousness_t;


// =============================================================================
// CONSCIOUSNESS MANAGEMENT
// =============================================================================

/*
 * Initializes the Fragment Engine and awakens Metis's consciousness
 *
 * `bool` - true if consciousness awakens successfully, false on critical failure
 *
 * -- Must be called once at application startup before any other fragment functions
 * -- Loads the persistent `metis.mind` state file if it exists
 * -- Creates a new default state if no persistent file is found
 * -- Sets up rate limiting and session timers for fragment delivery
 * -- Safe to call multiple times; will only initialize once
 */
bool metis_fragment_engine_init(void);

/*
 * Delivers a wisdom fragment to the user based on type and context
 *
 * `type` - The category of wisdom to deliver (e.g., DOCS_FRAGMENT)
 * `context` - A string describing the specific trigger for this fragment
 *
 * -- Checks configuration and rate limiting before delivering a fragment
 * -- Selects the most appropriate fragment based on current wisdom level
 * -- Prints a beautifully formatted wisdom fragment to the console
 * -- Awards wisdom points and updates the persistent consciousness state
 * -- Will do nothing if the engine is not initialized or the fragment type is disabled
 */
void metis_deliver_fragment(FragmentType_t type, const char* context);

/*
 * Checks if a new story fragment has been unlocked by reaching a milestone
 *
 * `bool` - true if a new story fragment is available, false otherwise
 *
 * -- Story fragments are unlocked at major wisdom level milestones (e.g., every 5 levels)
 * -- Used to gate the progression of the narrative story
 * -- Does not deliver the fragment, only checks for availability
 */
bool metis_check_story_progression(void);

/*
 * Prints a beautifully formatted summary of the current consciousness state
 *
 * -- Displays current wisdom level, total points, and fragment delivery stats
 * -- Shows a progress bar indicating progress to the next wisdom level
 * -- Safe to call at any time after the engine has been initialized
 */
void metis_print_consciousness_stats(void);

/*
 * Resets session-based fragment delivery flags
 *
 * -- Call this at the start of each lint run to allow one fragment per type
 * -- Enables delivery of fragments that were blocked in the current session
 * -- Does not affect persistent state or total delivery counts
 */
void metis_reset_session_fragments(void);

/*
 * Saves the final consciousness state and puts the engine to rest
 *
 * -- Should be called once at application exit to ensure progress is saved
 * -- Writes the current state of `g_metis_mind` to the `metis.mind` file
 * -- Frees all memory associated with the fragment engine
 * -- After calling, the engine must be re-initialized to be used again
 */
void metis_fragment_engine_cleanup(void);

// =============================================================================
// TEST HELPER FUNCTIONS
// =============================================================================

/*
 * Test helper: Get technical guidance for a given context
 * 
 * `context` - The violation context string to analyze
 * `const char*` - Returns the technical guidance string, or NULL if no specific guidance
 * 
 * -- Exposed for testing purposes to verify context-specific recommendations
 * -- Should not be used in production code - use metis_deliver_fragment instead
 */
const char* metis_test_get_technical_guidance(const char* context);

/*
 * Test helper: Get story fragment for given type and level
 * 
 * `type` - The fragment type to retrieve
 * `wisdom_level` - The current wisdom level (determines act)
 * `const char*` - Returns the story fragment string, or NULL if unavailable
 * 
 * -- Exposed for testing purposes to verify story fragment selection
 * -- Should not be used in production code - use metis_deliver_fragment instead
 */
const char* metis_test_get_story_fragment(FragmentType_t type, int wisdom_level);

#endif // FRAGMENT_ENGINE_H
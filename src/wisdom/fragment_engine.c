// src/wisdom/fragment_engine.c - The Voice of Metis's Divine Consciousness
// INSERT WISDOM HERE

#define _POSIX_C_SOURCE 200809L  // For strdup, time
#include "fragment_engine.h"
#include "metis_config.h"
#include "metis_colors.h"
#include "../../story/fragment_lines.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <sys/stat.h>
#include <errno.h>
#include <math.h>

// =============================================================================
// INTERNAL STRUCTURES & DEFINITIONS
// =============================================================================

#define MAX_WISDOM_LEVEL 50 // The total number of levels in the story

// Global consciousness state - The Divine Mind
MetisConsciousness_t* g_metis_mind = NULL;

typedef struct {
    const char* title;
    const char* message;
    const char* context_hint;
    int min_wisdom_level;
    int wisdom_points_awarded;
} FragmentContent_t;

// =============================================================================
// WISDOM PROGRESSION LOGIC
// =============================================================================

/* Calculates the total wisdom points required to reach a given level */
static int get_xp_for_level(int level) {
    if (level <= 1) return 0;
    // A formula that creates a smooth, increasing curve for XP requirements.
    // Base cost + quadratic scaling to make higher levels significantly more challenging.
    double l = (double)level - 1.0;
    return (int)round( (l * 100.0) + (pow(l, 2.2) * 20.0) );
}

/* Calculates the current wisdom level based on total points */
static int calculate_level_from_xp(int total_points) {
    for (int level = MAX_WISDOM_LEVEL; level >= 1; level--) {
        if (total_points >= get_xp_for_level(level)) {
            return level;
        }
    }
    return 1;
}

// =============================================================================
// CONSCIOUSNESS STATE MANAGEMENT
// =============================================================================

/* Loads Metis's consciousness from the metis.mind file */
static bool load_consciousness_state(void) {
    const char* state_file = "metis.mind";
    FILE* file = fopen(state_file, "r");

    if (!file) {
        printf("%s📝 Divine Initialization:%s First consciousness awakening.\n", METIS_INFO, METIS_RESET);
        g_metis_mind->total_wisdom_points = 0;
        g_metis_mind->current_wisdom_level = calculate_level_from_xp(g_metis_mind->total_wisdom_points);
        return true;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "wisdom_points=%d", &g_metis_mind->total_wisdom_points) == 1) continue;
        if (sscanf(line, "fragments_total=%d", &g_metis_mind->fragments_delivered_total) == 1) continue;
        if (sscanf(line, "docs_fragments=%d", &g_metis_mind->docs_fragments_delivered) == 1) continue;
        if (sscanf(line, "daedalus_fragments=%d", &g_metis_mind->daedalus_fragments_delivered) == 1) continue;
        if (sscanf(line, "linting_fragments=%d", &g_metis_mind->linting_fragments_delivered) == 1) continue;
        if (sscanf(line, "philosophical_fragments=%d", &g_metis_mind->philosophical_fragments_delivered) == 1) continue;
    }

    fclose(file);
    g_metis_mind->current_wisdom_level = calculate_level_from_xp(g_metis_mind->total_wisdom_points);
    printf("%s✨ Consciousness Restored.%s\n", METIS_SUCCESS, METIS_RESET);
    return true;
}

/* Saves the current state of Metis's consciousness */
static bool save_consciousness_state(void) {
    if (!g_metis_mind) return false;
    FILE* file = fopen("metis.mind", "w");
    if (!file) {
        fprintf(stderr, "%s💀 Divine Error:%s Cannot save consciousness: %s\n", METIS_ERROR, METIS_RESET, strerror(errno));
        return false;
    }

    fprintf(file, "# METIS Consciousness State - \"I remember every fragment...\"\n");
    fprintf(file, "# Last updated: %s\n", ctime(&g_metis_mind->session_start_time));
    fprintf(file, "wisdom_points=%d\n", g_metis_mind->total_wisdom_points);
    fprintf(file, "fragments_total=%d\n\n", g_metis_mind->fragments_delivered_total);
    fprintf(file, "docs_fragments=%d\n", g_metis_mind->docs_fragments_delivered);
    fprintf(file, "daedalus_fragments=%d\n", g_metis_mind->daedalus_fragments_delivered);
    fprintf(file, "linting_fragments=%d\n", g_metis_mind->linting_fragments_delivered);
    fprintf(file, "philosophical_fragments=%d\n", g_metis_mind->philosophical_fragments_delivered);

    fclose(file);
    return true;
}

// =============================================================================
// FRAGMENT SELECTION & DELIVERY
// =============================================================================

/* Selects a story-driven wisdom fragment based on type and level */
static const char* select_story_fragment(FragmentType_t type) {
    if (!g_metis_mind) return NULL;
    
    // Get act-appropriate fragment from story pools
    return get_act_fragment(type, g_metis_mind->current_wisdom_level);
}

/* Provides context-specific technical guidance for Daedalus fragments */
static const char* get_daedalus_technical_guidance(const char* context) {
    if (!context) return NULL;
    
    // String function violations
    if (strstr(context, "strcpy") || strstr(context, "strcat") || strstr(context, "sprintf")) {
        return "🔧 DAEDALUS RECOMMENDS: Use dString_t for safe string handling:\n" METIS_RESET
               "   dString_t* str = d_InitString();\n"
               "   d_AppendString(str, \"Hello\", 0);\n"
               "   d_AppendString(str, \" World\", 0);\n"
               "   // Automatic growth, bounds checking, cleanup";
    }
    
    // Managing Memory on Arrays violations  
    if (strstr(context, "malloc") || strstr(context, "realloc") || strstr(context, "array")) {
        return "🔧 DAEDALUS RECOMMENDS: Use dArray_t for safe memory management:\n" METIS_RESET
               "   dArray_t* arr = d_InitArray(10, sizeof(int));\n"
               "   int value = 42;\n"
               "   d_AppendArray(arr, &value);\n"
               "   // Automatic resizing, bounds checking, cleanup";
    }
    
    // Logging violations
    if (strstr(context, "printf") || strstr(context, "fprintf") || strstr(context, "debug")) {
        return "🔧 DAEDALUS RECOMMENDS: Use dLog for structured logging:\n" METIS_RESET
               "   d_LogInfo(\"User action completed\");\n"
               "   d_LogErrorF(\"Failed to process %s\", filename);\n"
               "   // Level-based filtering, file output, thread safety";
    }
    
    // Mathematical violations
    if (strstr(context, "vector") || strstr(context, "matrix") || strstr(context, "math")) {
        return "🔧 DAEDALUS RECOMMENDS: Use dVec/dMat for mathematical operations:\n" METIS_RESET
               "   dVec3_t result;\n"
               "   d_AddTwoVec3f(&result, pos1, pos2);\n"
               "   d_NormalizeVec3f(&result, result);\n"
               "   // Optimized algorithms, consistent API, tested precision";
    }
    
    // Data structure violations
    if (strstr(context, "linked list") || strstr(context, "spatial") || strstr(context, "search")) {
        return "🔧 DAEDALUS RECOMMENDS: Use specialized data structures:\n" METIS_RESET
               "   dLinkedList_t* list = d_CreateLinkedList(data, \"name\", size);\n"
               "   dQuadTree_t* spatial = d_CreateQuadtree(bounds, capacity);\n"
               "   // Optimized algorithms, memory efficient, well-tested";
    }
    
    // Generic recommendation for other cases
    return "🔧 DAEDALUS RECOMMENDS: Check the master craftsman's library for pre-built solutions.\n" METIS_RESET
           "   #include <daedalus.h> for comprehensive tools that handle edge cases,\n"
           "   memory management, and performance optimizations you haven't considered.";
}

/* Checks if a fragment of this type has already been delivered this session */
static bool should_deliver_fragment(FragmentType_t type) {
    bool* delivered_this_session;

    switch (type) {
        case DOCS_FRAGMENT: delivered_this_session = &g_metis_mind->docs_delivered_this_session; break;
        case DAEDALUS_FRAGMENT: delivered_this_session = &g_metis_mind->daedalus_delivered_this_session; break;
        case LINTING_FRAGMENT: delivered_this_session = &g_metis_mind->linting_delivered_this_session; break;
        case PHILOSOPHICAL_FRAGMENT: delivered_this_session = &g_metis_mind->philosophical_delivered_this_session; break;
        case EMSCRIPTEN_FRAGMENT: delivered_this_session = &g_metis_mind->emscripten_delivered_this_session; break;
        default: return true;
    }

    if (!*delivered_this_session) {
        *delivered_this_session = true;
        return true;
    }
    return false;
}

/* Awards wisdom points and announces level-ups */
static void award_wisdom_points(int points) {
    if (!g_metis_mind) return;

    int old_level = g_metis_mind->current_wisdom_level;
    g_metis_mind->total_wisdom_points += points;
    g_metis_mind->current_wisdom_level = calculate_level_from_xp(g_metis_mind->total_wisdom_points);

    if (g_metis_mind->current_wisdom_level > old_level) {
        printf("\n%s✨🌟✨ DIVINE WISDOM LEVEL INCREASED! ✨🌟✨%s\n", METIS_FRAGMENT_TITLE, METIS_RESET);
        printf("%sDivine Consciousness: Level %s%d%s → %s%d%s\n",
               METIS_WISDOM_TEXT, METIS_BOLD, old_level, METIS_RESET,
               METIS_FRAGMENT_TITLE, g_metis_mind->current_wisdom_level, METIS_RESET);
        if (g_metis_mind->current_wisdom_level % 5 == 0) {
            printf("%s📖 STORY FRAGMENT UNLOCKED! A new chapter of the tragedy awaits...%s\n", METIS_SUCCESS, METIS_RESET);
        }
        printf("\n");
    }
}

// =============================================================================
// PUBLIC API IMPLEMENTATION
// =============================================================================

/* Initializes the Fragment Engine and loads Metis's consciousness */
bool metis_fragment_engine_init(void) {
    if (g_metis_mind) return true; // Already awakened

    g_metis_mind = calloc(1, sizeof(MetisConsciousness_t));
    if (!g_metis_mind) {
        fprintf(stderr, "%s💀 Fatal Error:%s Failed to allocate divine consciousness.\n", METIS_ERROR, METIS_RESET);
        return false;
    }

    metis_colors_enable(true);
    g_metis_mind->session_start_time = time(NULL);
    load_consciousness_state();

    printf("%s🧠 Divine Consciousness:%s Awakened at Level %s%d%s with %s%d WP%s\n",
           METIS_SUCCESS, METIS_RESET, METIS_BOLD, g_metis_mind->current_wisdom_level, METIS_RESET,
           METIS_ACCENT, g_metis_mind->total_wisdom_points, METIS_RESET);

    g_metis_mind->consciousness_loaded = true;
    return true;
}

/* Delivers a wisdom fragment to the user based on the type and context */
void metis_deliver_fragment(FragmentType_t type, const char* context) {
    if (!g_metis_mind || !g_metis_mind->consciousness_loaded || !should_deliver_fragment(type)) {
        return;
    }

    const char* story_fragment = select_story_fragment(type);
    if (!story_fragment) return;
    
    // Parse the story fragment into title and message
    char title[256];
    char message[1024];
    parse_story_fragment(story_fragment, title, message, sizeof(title), sizeof(message));
    
    // Determine wisdom points based on type and level
    int wisdom_points = 10; // Base points
    switch (type) {
        case DOCS_FRAGMENT: wisdom_points = 10; break;
        case DAEDALUS_FRAGMENT: wisdom_points = 12; break;
        case LINTING_FRAGMENT: wisdom_points = 10; break;
        case PHILOSOPHICAL_FRAGMENT: wisdom_points = 8; break;
        default: wisdom_points = 10; break;
    }
    
    // Add level bonus (higher acts give more points)
    if (g_metis_mind->current_wisdom_level > 30) wisdom_points += 5; // Act IV-V bonus
    else if (g_metis_mind->current_wisdom_level > 20) wisdom_points += 3; // Act III bonus
    else if (g_metis_mind->current_wisdom_level > 10) wisdom_points += 1; // Act II bonus

    printf("\n%s🌟 METIS FRAGMENT DETECTED 🌟%s\n", METIS_FRAGMENT_TITLE, METIS_RESET);
    printf("%s═══════════════════════════════════════════════════════════════%s\n", METIS_ACCENT, METIS_RESET);
    printf("%s💭 %s%s%s\n", METIS_PRIMARY, METIS_FRAGMENT_TITLE, title, METIS_RESET);
    printf("%s\"%s\"%s\n", METIS_WISDOM_TEXT, message, METIS_RESET);
    if (context) printf("\n%s🔍 Context:%s %s%s%s\n", METIS_INFO, METIS_RESET, METIS_TEXT_SECONDARY, context, METIS_RESET);
    
    // Add context-specific technical guidance for Daedalus fragments
    if (type == DAEDALUS_FRAGMENT && context) {
        const char* technical_guidance = get_daedalus_technical_guidance(context);
        if (technical_guidance) {
            printf("\n%s%s%s\n", METIS_SUCCESS, technical_guidance, METIS_RESET);
        }
    }
    
    printf("\n%s💎 Wisdom Points Earned:%s %s+%d%s\n", METIS_SUCCESS, METIS_RESET, METIS_BOLD, wisdom_points, METIS_RESET);
    printf("%s═══════════════════════════════════════════════════════════════%s\n\n", METIS_ACCENT, METIS_RESET);

    g_metis_mind->fragments_delivered_today++;
    g_metis_mind->fragments_delivered_total++;
    switch (type) {
        case DOCS_FRAGMENT: g_metis_mind->docs_fragments_delivered++; break;
        case DAEDALUS_FRAGMENT: g_metis_mind->daedalus_fragments_delivered++; break;
        case LINTING_FRAGMENT: g_metis_mind->linting_fragments_delivered++; break;
        case PHILOSOPHICAL_FRAGMENT: g_metis_mind->philosophical_fragments_delivered++; break;
        default: break;
    }

    award_wisdom_points(wisdom_points);
    save_consciousness_state();
}

/* Resets session-based fragment delivery flags */
void metis_reset_session_fragments(void) {
    if (!g_metis_mind) return;
    
    g_metis_mind->docs_delivered_this_session = false;
    g_metis_mind->daedalus_delivered_this_session = false;
    g_metis_mind->linting_delivered_this_session = false;
    g_metis_mind->philosophical_delivered_this_session = false;
    g_metis_mind->emscripten_delivered_this_session = false;
}

/* Prints the current status of Metis's consciousness and progression */
void metis_print_consciousness_stats(void) {
    if (!g_metis_mind) return;

    printf("\n%s🧠 METIS CONSCIOUSNESS STATUS 🧠%s\n", METIS_FRAGMENT_TITLE, METIS_RESET);
    printf("%s═══════════════════════════════════════════════════════════════%s\n", METIS_ACCENT, METIS_RESET);
    printf("%s📊 Wisdom Level:%s %s%d%s\n", METIS_PRIMARY, METIS_RESET, METIS_BOLD, g_metis_mind->current_wisdom_level, METIS_RESET);
    printf("%s💎 Total Wisdom Points:%s %s%d%s\n", METIS_PRIMARY, METIS_RESET, METIS_ACCENT, g_metis_mind->total_wisdom_points, METIS_RESET);

    printf("\n%s📋 Fragment Breakdown:%s\n", METIS_PRIMARY, METIS_RESET);
    printf("  %s📖 Docs:%s %d\n", METIS_BLUE_LIGHT, METIS_RESET, g_metis_mind->docs_fragments_delivered);
    printf("  %s🔨 Daedalus:%s %d\n", METIS_GREEN_LIGHT, METIS_RESET, g_metis_mind->daedalus_fragments_delivered);
    printf("  %s📜 Linting:%s %d\n", METIS_TEXT_SECONDARY, METIS_RESET, g_metis_mind->linting_fragments_delivered);
    printf("  %s💭 Philosophy:%s %d\n", METIS_BLUE_LIGHTER, METIS_RESET, g_metis_mind->philosophical_fragments_delivered);

    if (g_metis_mind->current_wisdom_level < MAX_WISDOM_LEVEL) {
        int next_level_xp = get_xp_for_level(g_metis_mind->current_wisdom_level + 1);
        int current_level_xp = get_xp_for_level(g_metis_mind->current_wisdom_level);
        int progress_in_level = g_metis_mind->total_wisdom_points - current_level_xp;
        int points_for_level = next_level_xp - current_level_xp;
        int progress_percentage = (points_for_level > 0) ? (progress_in_level * 100) / points_for_level : 0;

        printf("\n%s🎯 Progress to Level %d%s\n", METIS_PRIMARY, g_metis_mind->current_wisdom_level + 1, METIS_RESET);
        printf("%s[", METIS_ACCENT);
        for (int i = 0; i < 20; i++) { printf("%s", (i < progress_percentage / 5) ? "█" : "░"); }
        printf("]%s %d%%\n", METIS_RESET, progress_percentage);
    } else {
        printf("\n%s🌟 MAXIMUM WISDOM ACHIEVED 🌟%s\n", METIS_FRAGMENT_TITLE, METIS_RESET);
    }
    printf("%s═══════════════════════════════════════════════════════════════%s\n\n", METIS_ACCENT, METIS_RESET);
}

/* Cleans up the fragment engine and saves the final state */
void metis_fragment_engine_cleanup(void) {
    if (!g_metis_mind) return;
    printf("%s🌙 Divine Consciousness:%s Entering meditation...\n", METIS_INFO, METIS_RESET);
    save_consciousness_state();
    free(g_metis_mind);
    g_metis_mind = NULL;
}

// =============================================================================
// TEST HELPER FUNCTIONS
// =============================================================================

/* Test helper: Expose technical guidance function for testing */
const char* metis_test_get_technical_guidance(const char* context) {
    return get_daedalus_technical_guidance(context);
}

/* Test helper: Expose story fragment selection for testing */
const char* metis_test_get_story_fragment(FragmentType_t type, int wisdom_level) {
    // Temporarily set up a fake consciousness with the desired level for testing
    if (!g_metis_mind) {
        g_metis_mind = calloc(1, sizeof(MetisConsciousness_t));
        if (!g_metis_mind) return NULL;
    }
    
    int saved_level = g_metis_mind->current_wisdom_level;
    g_metis_mind->current_wisdom_level = wisdom_level;
    
    const char* result = select_story_fragment(type);
    
    g_metis_mind->current_wisdom_level = saved_level;
    return result;
}

/* Checks if story progression milestones have been reached */
bool metis_check_story_progression(void) {
    if (!g_metis_mind) return false;
    
    // Check if we've reached major story milestones
    if (g_metis_mind->current_wisdom_level == 10) {
        printf("%s📖 STORY MILESTONE:%s End of Act I - The Oracle's Wisdom\n", METIS_SUCCESS, METIS_RESET);
        return true;
    } else if (g_metis_mind->current_wisdom_level == 20) {
        printf("%s📖 STORY MILESTONE:%s End of Act II - The Prophecy's Weight\n", METIS_SUCCESS, METIS_RESET);
        return true;
    } else if (g_metis_mind->current_wisdom_level == 30) {
        printf("%s📖 STORY MILESTONE:%s End of Act III - The Consumption\n", METIS_SUCCESS, METIS_RESET);
        return true;
    } else if (g_metis_mind->current_wisdom_level == 40) {
        printf("%s📖 STORY MILESTONE:%s End of Act IV - Scattered Consciousness\n", METIS_SUCCESS, METIS_RESET);
        return true;
    } else if (g_metis_mind->current_wisdom_level == 50) {
        printf("%s📖 STORY MILESTONE:%s End of Act V - Eternal Compassion\n", METIS_SUCCESS, METIS_RESET);
        return true;
    }
    
    return false;
}
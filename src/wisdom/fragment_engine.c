// src/wisdom/fragment_engine.c - The Voice of Metis's Divine Consciousness
// INSERT WISDOM HERE

#define _POSIX_C_SOURCE 200809L  // For strdup, time
#include "fragment_engine.h"
#include "metis_config.h"
#include "metis_colors.h"
#include "../../story/fragment_lines.h" // Correct path to your refactored header
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

/*
 * Parses context string to extract specific violation details
 */
static void parse_violation_context(const char* context, FragmentContext_t* parsed_context) {
    if (!context || !parsed_context) return;
    
    // Initialize the context structure
    memset(parsed_context, 0, sizeof(FragmentContext_t));
    
    // Extract function names from common patterns
    if (strstr(context, "printf") || strstr(context, "Printf")) {
        parsed_context->unsafe_function = "printf";
        parsed_context->violation_type = "printf_family";
    } else if (strstr(context, "malloc")) {
        parsed_context->unsafe_function = "malloc";
        parsed_context->violation_type = "manual_memory_allocation";
    } else if (strstr(context, "strcpy") || strstr(context, "strcat")) {
        parsed_context->unsafe_function = strstr(context, "strcpy") ? "strcpy" : "strcat";
        parsed_context->violation_type = "unsafe_string_operation";
    } else if (strstr(context, "strcmp")) {
        parsed_context->unsafe_function = "strcmp";
        parsed_context->violation_type = "unsafe_string_comparison";
    } else if (strstr(context, "free")) {
        parsed_context->unsafe_function = "free";
        parsed_context->violation_type = "manual_memory_deallocation";
    }
    
    // Set generic context if no specific match
    if (!parsed_context->violation_type) {
        parsed_context->violation_type = "generic_improvement";
    }
}

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


static const UnsafeFunctionGuidance_t UNSAFE_FUNCTION_GUIDANCE[] = {
    {
        .unsafe_function = "malloc",
        .context_template = 
            "Manual memory allocation requires explicit size calculation, lacks bounds checking, "
            "and demands manual cleanup. %sd_InitArray()%s provides automatic growth, "
            "built-in bounds protection, and guaranteed cleanup through %sd_DestroyArray()%s.",
        .unsafe_pattern_template = 
            "%sItem_t* items = malloc(sizeof(Item_t) * count);%s\n"
            "%s// Manual sizing, no bounds checking, cleanup required%s",
        .daedalus_solution_template = 
            "%s🔧 Daedalus Forges:%s The master crafted %sd_InitArray()%s for safe use:\n"
            "   %sdArray_t* items = d_InitArray(count, sizeof(Item_t));%s\n"
            "   %s// Automatic growth, bounds checking, %sd_DestroyArray()%s cleanup%s",
        .wisdom_points = 15
    },
    {
        .unsafe_function = "free",
        .context_template = 
            "Manual deallocation is prone to double-free errors, memory leaks from forgotten calls, "
            "and use-after-free vulnerabilities. %sd_DestroyArray()%s handles complete cleanup "
            "automatically, including nested structures and prevents common memory errors.",
        .unsafe_pattern_template = 
            "%sfree(items); items = NULL;%s\n"
            "%s// Easy to forget, double-free risk, manual null assignment%s",
        .daedalus_solution_template = 
            "%s🔧 Daedalus Provides:%s The master provides %sd_DestroyArray()%s for automatic cleanup:\n"
            "   %sd_DestroyArray(items_array);%s\n"
            "   %s// Handles all memory, prevents leaks and double-free errors%s",
        .wisdom_points = 12
    },
    {
        .unsafe_function = "realloc",
        .context_template = 
            "Manual memory reallocation risks losing data, null pointer returns, and memory leaks. "
            "%sd_GrowArray()%s and %sd_ResizeArray()%s provide safe expansion with automatic "
            "data preservation and error handling.",
        .unsafe_pattern_template = 
            "%sptr = realloc(ptr, new_size);%s\n"
            "%s// Risk of null return, data loss, manual size tracking%s",
        .daedalus_solution_template = 
            "%s🔧 Daedalus Reshapes:%s The master forged %sd_GrowArray()%s for safe expansion:\n"
            "   %sd_GrowArray(items, additional_capacity);%s\n"
            "   %s// Safe growth, data preservation, %sd_ResizeArray()%s for precise sizing%s",
        .wisdom_points = 15
    },
    {
        .unsafe_function = "strcpy",
        .context_template = 
            "String copying without bounds checking leads to buffer overflows and security vulnerabilities. "
            "%sd_SetString()%s and %sd_AppendString()%s automatically manage buffer sizes, "
            "ensure null termination, and grow containers as needed.",
        .unsafe_pattern_template = 
            "%sstrcpy(dest, src);%s\n"
            "%s// No bounds checking, buffer overflow risk, manual sizing%s",
        .daedalus_solution_template = 
            "%s🔧 Daedalus Weaves:%s The master spun %sd_SetString()%s for safe string handling:\n"
            "   %sd_SetString(dest_string, src, 0);%s\n"
            "   %s// Automatic sizing, bounds protection, guaranteed null termination%s",
        .wisdom_points = 14
    },
    {
        .unsafe_function = "printf",
        .context_template = 
            "Direct console output lacks filtering, threading safety, and structured formatting. "
            "%sd_LogInfoF()%s provides level-based filtering, thread-safe output, "
            "file handlers, and consistent formatting across your application.",
        .unsafe_pattern_template = 
            "%sprintf(\"Processing item %s\\n\", name);%s\n"
            "%s// Unfiltered output, no threading safety, no structured logging%s",
        .daedalus_solution_template = 
            "%s🔧 Daedalus Records:%s The master carved %sd_LogInfoF()%s for structured output:\n"
            "   %sd_LogInfoF(\"Processing item %s\", name);%s\n"
            "   %s// Level filtering, thread safety, %sd_AddLogHandler()%s support%s",
        .wisdom_points = 13
    },
    {
        .unsafe_function = "strcmp",
        .context_template = 
            "String comparison without NULL protection causes segmentation faults when either "
            "string is NULL. %sd_CompareStrings()%s and %sd_CompareStringToCString()%s "
            "handle NULL inputs gracefully and provide consistent comparison results.",
        .unsafe_pattern_template = 
            "%sif (strcmp(str1->str, str2) == 0)%s\n"
            "%s// Crashes on NULL input, no dString_t integration%s",
        .daedalus_solution_template = 
            "%s🔧 Daedalus Weighs:%s The master balanced %sd_CompareStringToCString()%s for safe comparison:\n"
            "   %sif (d_CompareStringToCString(str1, str2) == 0)%s\n"
            "   %s// NULL-safe, %sdString_t%s integration, consistent results%s",
        .wisdom_points = 16
    },
    // Generic fallback
    {
        .unsafe_function = "generic",
        .context_template = 
            "Standard C library functions often lack modern safety features like bounds checking, "
            "automatic memory management, and NULL protection. The %sDaedalus library%s provides "
            "master-crafted alternatives with built-in safety and performance optimizations.",
        .unsafe_pattern_template = 
            "%s// Raw C standard library usage%s\n"
            "%s// Manual memory management, bounds checking, error handling%s",
        .daedalus_solution_template = 
            "%s🔧 Daedalus Provides:%s The master stocked comprehensive solutions:\n"
            "   %s#include <daedalus.h>%s\n"
            "   %s// Automatic memory management, bounds checking, safety%s",
        .wisdom_points = 10
    }
};

static const size_t UNSAFE_FUNCTION_GUIDANCE_COUNT = sizeof(UNSAFE_FUNCTION_GUIDANCE) / sizeof(UNSAFE_FUNCTION_GUIDANCE[0]);
/*
 * Provides context-specific technical guidance for Daedalus fragments
 * by matching keywords in the context string against predefined rules.
 * ENHANCED VERSION - Uses the UNSAFE_FUNCTION_GUIDANCE template system
 */
const char* get_daedalus_guidance_for_context(const char* context) {
    if (!context) {
        const UnsafeFunctionGuidance_t* generic = &UNSAFE_FUNCTION_GUIDANCE[UNSAFE_FUNCTION_GUIDANCE_COUNT - 1];
        static char formatted_guidance[1024];
        snprintf(formatted_guidance, sizeof(formatted_guidance), 
                 generic->daedalus_solution_template,
                 METIS_SUCCESS, METIS_RESET,          // 🔧 Daedalus Provides: (green)
                 METIS_WARNING, METIS_RESET,          // #include <daedalus.h> (yellow)
                 METIS_TEXT_SECONDARY, METIS_RESET);   // // comment (muted)
        return formatted_guidance;
    }

    // Find matching guidance
    const UnsafeFunctionGuidance_t* selected = NULL;
    for (size_t i = 0; i < UNSAFE_FUNCTION_GUIDANCE_COUNT - 1; i++) {
        if (strstr(context, UNSAFE_FUNCTION_GUIDANCE[i].unsafe_function)) {
            selected = &UNSAFE_FUNCTION_GUIDANCE[i];
            break;
        }
    }
    
    if (!selected) {
        selected = &UNSAFE_FUNCTION_GUIDANCE[UNSAFE_FUNCTION_GUIDANCE_COUNT - 1];
    }
    
    static char formatted_guidance[2048];
    snprintf(formatted_guidance, sizeof(formatted_guidance), 
             selected->daedalus_solution_template,
             METIS_SUCCESS, METIS_RESET METIS_TEXT_MUTED,              // 🔧 Daedalus [Verb]: (green)
             METIS_ACCENT, METIS_TEXT_MUTED,              // Function name (orange)
             METIS_WARNING, METIS_RESET,              // Code example (yellow)
             METIS_TEXT_MUTED,                    // Comment start (muted)
             METIS_TEXT_MUTED, METIS_RESET,              // Comment code (orange)
             METIS_RESET);                            // End formatting
    
    return formatted_guidance;
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
    
    // *** REFRACTOR HERE: Call the centralized function ***
    return get_daedalus_guidance_for_context(context);
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

void metis_deliver_fragment(FragmentType_t type, const char* context, const char* file_path, int line_number, int column) {
    if (!g_metis_mind || !g_metis_mind->consciousness_loaded || !should_deliver_fragment(type)) {
        return;
    }

    // Parse the context to extract specific violation details
    FragmentContext_t parsed_context = {0};
    parse_violation_context(context, &parsed_context);
    
    const char* story_fragment = select_story_fragment(type);
    if (!story_fragment) return;
    
    // Parse the story fragment into title and message
    char title[256];
    char message[1024];
    parse_story_fragment(story_fragment, title, message, sizeof(title), sizeof(message));

    char location_info[512] = "";
    if (file_path && line_number > 0) {
        if (column > 0) {
            snprintf(location_info, sizeof(location_info), "%s:%d:%d", file_path, line_number, column);
        } else {
            snprintf(location_info, sizeof(location_info), "%s:%d", file_path, line_number);
        }
    }
    
    // Determine wisdom points and get appropriate guidance based on type
    int wisdom_points = 10;
    const char* technical_guidance = NULL;
    
    switch (type) {
        case DOCS_FRAGMENT:
            wisdom_points = 10;
            technical_guidance = "🧠 Metis Counsels:" METIS_RESET METIS_TEXT_MUTED " I weave understanding between minds.\n"
                            "Add clear comments explaining purpose, parameters, and returns." METIS_RESET;
            break;
            
        case DAEDALUS_FRAGMENT:
            wisdom_points = 12;
            technical_guidance = get_daedalus_guidance_for_context(context);
            break;
            
        case PHILOSOPHICAL_FRAGMENT:
            wisdom_points = 8;
            technical_guidance = "🧠 Metis Reflects:" METIS_RESET METIS_TEXT_MUTED " I see the weight of complexity in every line.\n"
                            "Break large functions into smaller, focused pieces.\n"
                            "Each function should have one clear purpose." METIS_RESET;
            break;
            
        case LINTING_FRAGMENT:
            wisdom_points = 10;
            technical_guidance = "🧠 Metis Guides:" METIS_RESET METIS_TEXT_MUTED " I weave harmony through consistent patterns.\n"
                            "Follow established styles to help future readers navigate your thoughts." METIS_RESET;
            break;
            
        default:
            wisdom_points = 10;
            technical_guidance = "🧠 Metis Whispers:" METIS_RESET METIS_TEXT_MUTED " I offer comprehensive wisdom through divine craftsmanship." METIS_RESET;
            break;
    }
    
    // Level bonus
    if (g_metis_mind->current_wisdom_level > 30) wisdom_points += 5;
    else if (g_metis_mind->current_wisdom_level > 20) wisdom_points += 3;
    else if (g_metis_mind->current_wisdom_level > 10) wisdom_points += 1;

    if (!technical_guidance) {
        technical_guidance = "The master provides comprehensive solutions through divine craftsmanship.";
    }

    // Format the enhanced fragment with location
    printf(ENHANCED_REGULAR_FRAGMENT_TEMPLATE,
        // Header colors
        METIS_FRAGMENT_TITLE, METIS_RESET,
        METIS_ACCENT, METIS_RESET,
        // Location colors (faded clickable link)
        METIS_TEXT_MUTED, location_info, METIS_RESET,
        // Quote colors
        METIS_WISDOM_TEXT, message, METIS_RESET,
        // Violation section colors
        METIS_INFO, METIS_RESET,
        METIS_TEXT_SECONDARY, (context ? context : "General improvement recommended"), METIS_RESET,
        // Technical guidance colors
        METIS_SUCCESS, technical_guidance, METIS_RESET,
        // Footer colors
        METIS_SUCCESS, METIS_RESET, METIS_BOLD, wisdom_points, METIS_RESET,
        METIS_ACCENT, METIS_RESET
    );


    // Update consciousness state
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

/* Delivers an enhanced contextual wisdom fragment with template substitution */
void metis_deliver_contextual_fragment(const FragmentContext_t* fragment_context) {
    if (!g_metis_mind || !g_metis_mind->consciousness_loaded || !fragment_context) {
        return;
    }
    
    // Check if we should deliver a Daedalus fragment this session
    if (g_metis_mind->daedalus_delivered_this_session) {
        return;
    }
    
    // Select the appropriate contextual fragment based on violation type
    const ContextualFragment_t* contextual_fragment = select_contextual_fragment(fragment_context->violation_type);
    if (!contextual_fragment) {
        // Fall back to regular fragment delivery if no contextual fragment matches
        metis_deliver_fragment(DAEDALUS_FRAGMENT, fragment_context->violation_type, 
                             fragment_context->file_name, 0, 0);
        return;
    }
    
    // Perform template substitution
    char* rendered_context = substitute_template(contextual_fragment->context_template, fragment_context);
    char* rendered_daedalus = substitute_template(contextual_fragment->daedalus_template, fragment_context);
    
    if (!rendered_context || !rendered_daedalus) {
        // Cleanup and fall back to regular fragment
        if (rendered_context) free(rendered_context);
        if (rendered_daedalus) free(rendered_daedalus);
        metis_deliver_fragment(DAEDALUS_FRAGMENT, fragment_context->violation_type, 
                             fragment_context->file_name, 0, 0);
        return;
    }
    
    // Calculate wisdom points with level bonus
    int wisdom_points = contextual_fragment->wisdom_points;
    if (g_metis_mind->current_wisdom_level > 30) wisdom_points += 5;
    else if (g_metis_mind->current_wisdom_level > 20) wisdom_points += 3;
    else if (g_metis_mind->current_wisdom_level > 10) wisdom_points += 1;
    
    // Build location string if file_name is available
    char location_info[512] = "";
    if (fragment_context->file_name) {
        snprintf(location_info, sizeof(location_info), "%s", fragment_context->file_name);
    }
    
    // Render the beautiful contextual fragment
    printf("\n%s🌟 METIS CONTEXTUAL FRAGMENT DETECTED 🌟%s\n", METIS_FRAGMENT_TITLE, METIS_RESET);
    printf("%s═══════════════════════════════════════════════════════════════%s\n", METIS_ACCENT, METIS_RESET);
    if (strlen(location_info) > 0) {
        printf("%s%s%s\n", METIS_TEXT_MUTED, location_info, METIS_RESET);
    }
    printf("%s💭 %s%s%s\n", METIS_PRIMARY, METIS_FRAGMENT_TITLE, contextual_fragment->title, METIS_RESET);
    printf("\n%s%s%s\n", METIS_WISDOM_TEXT, contextual_fragment->philosophical_quote, METIS_RESET);

    printf("\n%s📜 The Tale:%s\n", METIS_PRIMARY, METIS_RESET);
    printf("%s%s%s\n", METIS_TEXT_SECONDARY, rendered_context, METIS_RESET);
    
    printf("\n%s%s%s\n", METIS_SUCCESS, rendered_daedalus, METIS_RESET);
    
    printf("\n%s💎 Wisdom Points Earned:%s %s+%d%s\n", METIS_SUCCESS, METIS_RESET, METIS_BOLD, wisdom_points, METIS_RESET);
    printf("%s═══════════════════════════════════════════════════════════════%s\n\n", METIS_ACCENT, METIS_RESET);
    
    // Update consciousness state
    g_metis_mind->fragments_delivered_today++;
    g_metis_mind->fragments_delivered_total++;
    g_metis_mind->daedalus_fragments_delivered++;
    g_metis_mind->daedalus_delivered_this_session = true;
    
    award_wisdom_points(wisdom_points);
    save_consciousness_state();
    
    // Cleanup allocated memory
    free(rendered_context);
    free(rendered_daedalus);
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
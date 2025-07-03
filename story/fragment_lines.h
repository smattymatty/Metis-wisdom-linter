// story/fragment_lines.h - Technical Wisdom Fragments Organized by Divine Acts

#ifndef FRAGMENT_LINES_H
#define FRAGMENT_LINES_H

// Core C standard library includes needed for the declarations below
#include <stdio.h>    // For size_t
#include <stdlib.h>   // For NULL
#include <string.h>   // For strstr, strncpy (if struct members hint at string usage)
#include <stdbool.h>  // Good practice to include if you use bool types

// Include your custom colors header if Metis colors are used directly in recommendations,
// otherwise, it might only be needed in the .c file.
#include "metis_colors.h"

// Enhanced contextual fragment system structures
typedef struct {
    const char* fragment_id;          // Unique identifier for this fragment (e.g., "duel_of_echoes")
    const char* title;                // Display title for the fragment
    const char* philosophical_quote;  // A philosophical quote relevant to the violation
    const char* context_template;     // Template with placeholders like {VARIABLE1}, {VARIABLE2}
    const char* daedalus_template;    // Technical recommendation template with placeholders
    int wisdom_points;                // Points awarded for this specific fragment
    const char** keywords;            // Keywords that trigger this fragment (NULL-terminated array)
} ContextualFragment_t;

typedef struct {
    const char* variable1;      // First variable name (e.g., "player->name")
    const char* variable2;      // Second variable name (e.g., "\"Minos\"")
    const char* function_name;  // Function name where violation occurred
    const char* file_name;      // File name where violation occurred
    const char* violation_type; // Type of violation (e.g., "unsafe_strcmp_dstring_cstring")
    const char* unsafe_function; // The specific unsafe function detected (e.g., "printf", "malloc")
    int line_number;             // Line number where violation occurred
    int column;                  // Column number where violation occurred
} FragmentContext_t;

// Define FragmentType_t enum if it's not exclusively defined elsewhere
// The #ifndef FRAGMENT_ENGINE_H ... #endif block is good for preventing redefinition
// if fragment_engine.h *also* defines it, ensuring consistency.
#ifndef FRAGMENT_ENGINE_H
typedef enum {
    LINTING_FRAGMENT,
    DOCS_FRAGMENT,
    DAEDALUS_FRAGMENT,
    PHILOSOPHICAL_FRAGMENT,
    EMSCRIPTEN_FRAGMENT,
    STORY_FRAGMENT
} FragmentType_t;
#endif

// =============================================================================
// UNSAFE FUNCTION GUIDANCE SYSTEM
// =============================================================================

typedef struct {
    const char* unsafe_function;          // The dangerous function name
    const char* context_template;         // Technical explanation of the problem
    const char* unsafe_pattern_template;  // Template showing dangerous usage
    const char* daedalus_solution_template; // Template showing Daedalus solution
    int wisdom_points;                    // Points awarded
} UnsafeFunctionGuidance_t;

typedef struct {
    const char* placeholder;              // e.g., "{UNSAFE_FUNCTION}"
    const char* replacement;              // The actual value to substitute
} TemplateSubstitution_t;

// Provides context-specific technical guidance for Daedalus fragments
const char* get_daedalus_guidance_for_context(const char* context);

// Function to get act-appropriate fragment based on wisdom level and type
const char* get_act_fragment(FragmentType_t type, int wisdom_level);

// Function to parse fragment into title and content
void parse_story_fragment(const char* fragment, char* title, char* content, size_t title_size, size_t content_size);

// Enhanced contextual fragment functions
const ContextualFragment_t* select_contextual_fragment(const char* violation_type);
char* substitute_template(const char* template, const FragmentContext_t* context);

static const char* ENHANCED_REGULAR_FRAGMENT_TEMPLATE = 
    "%s🌟 METIS FRAGMENT DETECTED 🌟%s\n"
    "%s═══════════════════════════════════════════════════════════════%s\n"
    "%s%s%s\n"
    "\n%s\"%s\"%s\n"
    "\n%s📜 The Violation:%s\n"
    "%s%s%s\n"
    "\n%s%s%s\n"
    "\n%s💎 Wisdom Points Earned:%s %s+%d%s\n"
    "%s═══════════════════════════════════════════════════════════════%s\n\n";

#endif // FRAGMENT_LINES_H
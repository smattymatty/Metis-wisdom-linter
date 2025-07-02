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

// Structure for Daedalus technical guidance
// This struct needs to be visible to any file that calls get_daedalus_guidance_for_context
// or otherwise interacts with DaedalusGuidance_t.
typedef struct {
    const char** keywords;       // Array of keywords to search for in context
    const char* recommendation;  // The recommendation message
} DaedalusGuidance_t;

// --- Function Declarations (Prototypes) ---
// These functions are implemented in wisdom/fragment_lines.c
// but are declared here so other files can call them.

// Provides context-specific technical guidance for Daedalus fragments
const char* get_daedalus_guidance_for_context(const char* context);

// Function to get act-appropriate fragment based on wisdom level and type
const char* get_act_fragment(FragmentType_t type, int wisdom_level);

// Function to parse fragment into title and content
void parse_story_fragment(const char* fragment, char* title, char* content, size_t title_size, size_t content_size);

#endif // FRAGMENT_LINES_H
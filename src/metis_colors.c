/*
 * METIS WISDOM LINTER - Divine Color System Implementation
 */

#define _POSIX_C_SOURCE 200809L

#include "metis_colors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static bool g_colors_enabled = true;
static bool g_colors_checked = false;
static bool g_colors_supported = false;

bool metis_colors_supported(void) {
    if (g_colors_checked) {
        return g_colors_supported;
    }

    g_colors_checked = true;

    // Check if we're outputting to a terminal
    if (!isatty(STDOUT_FILENO)) {
        g_colors_supported = false;
        return false;
    }

    // Check TERM environment variable
    const char* term = getenv("TERM");
    if (!term) {
        g_colors_supported = false;
        return false;
    }

    // Basic terminal capability check
    if (strstr(term, "color") ||
        strstr(term, "xterm") ||
        strstr(term, "screen") ||
        strstr(term, "tmux") ||
        strcmp(term, "linux") == 0) {
        g_colors_supported = true;
        return true;
    }

    g_colors_supported = false;
    return false;
}

void metis_colors_enable(bool enable) {
    g_colors_enabled = enable && metis_colors_supported();
}

const char* metis_color_for_severity(int severity) {
    if (!g_colors_enabled) return "";

    switch (severity) {
        case 0: return METIS_VIOLATION_INFO;    // INFO
        case 1: return METIS_VIOLATION_WARN;    // WARNING
        case 2: return METIS_VIOLATION_ERROR;   // ERROR
        default: return METIS_TEXT_PRIMARY;
    }
}

const char* metis_color_for_type(int type) {
    if (!g_colors_enabled) return "";

    switch (type) {
        case 0: return METIS_RED_LIGHTER;       // MEMORY_VIOLATION
        case 1: return METIS_BLUE_LIGHT;        // DOCS_VIOLATION
        case 2: return METIS_GRAY_LIGHT;        // STYLE_VIOLATION
        case 3: return METIS_ORANGE_LIGHT;      // PHILOSOPHICAL_VIOLATION
        case 4: return METIS_GREEN_LIGHT;       // DAEDALUS_SUGGESTION
        default: return METIS_TEXT_PRIMARY;
    }
}

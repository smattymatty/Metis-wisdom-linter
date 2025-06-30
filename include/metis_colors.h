/*
 * METIS WISDOM LINTER - Divine Color System
 *
 */

#ifndef METIS_COLORS_H
#define METIS_COLORS_H

#include <stdbool.h>

// ===== BLUES - Cool Ocean Tones =====
#define METIS_BLUE_DARKEST    "\033[38;2;23;32;56m"     // #172038
#define METIS_BLUE_DARK       "\033[38;2;37;58;94m"     // #253a5e
#define METIS_BLUE_MEDIUM     "\033[38;2;60;94;139m"    // #3c5e8b
#define METIS_BLUE_LIGHT      "\033[38;2;79;143;186m"   // #4f8fba
#define METIS_BLUE_LIGHTER    "\033[38;2;115;190;211m"  // #73bed3
#define METIS_BLUE_LIGHTEST   "\033[38;2;164;221;219m"  // #a4dddb

// ===== GREENS - Nature Earth Tones =====
#define METIS_GREEN_DARKEST   "\033[38;2;25;51;45m"     // #19332d
#define METIS_GREEN_DARK      "\033[38;2;37;86;46m"     // #25562e
#define METIS_GREEN_MEDIUM    "\033[38;2;70;130;50m"    // #468232
#define METIS_GREEN_LIGHT     "\033[38;2;117;167;67m"   // #75a743
#define METIS_GREEN_LIGHTER   "\033[38;2;168;202;88m"   // #a8ca58
#define METIS_GREEN_LIGHTEST  "\033[38;2;208;218;145m"  // #d0da91

// ===== BROWNS - Warm Earth Tones =====
#define METIS_BROWN_DARKEST   "\033[38;2;77;43;50m"     // #4d2b32
#define METIS_BROWN_DARK      "\033[38;2;122;72;65m"    // #7a4841
#define METIS_BROWN_MEDIUM    "\033[38;2;173;119;87m"   // #ad7757
#define METIS_BROWN_LIGHT     "\033[38;2;192;148;115m"  // #c09473
#define METIS_BROWN_LIGHTER   "\033[38;2;215;181;148m"  // #d7b594
#define METIS_BROWN_LIGHTEST  "\033[38;2;231;213;179m"  // #e7d5b3

// ===== ORANGES - Fire Warm Tones =====
#define METIS_ORANGE_DARKEST  "\033[38;2;52;28;39m"     // #341c27
#define METIS_ORANGE_DARK     "\033[38;2;96;44;44m"     // #602c2c
#define METIS_ORANGE_MEDIUM   "\033[38;2;136;75;43m"    // #884b2b
#define METIS_ORANGE_LIGHT    "\033[38;2;190;119;43m"   // #be772b
#define METIS_ORANGE_LIGHTER  "\033[38;2;222;158;65m"   // #de9e41
#define METIS_ORANGE_LIGHTEST "\033[38;2;232;193;112m"  // #e8c170

// ===== REDS - Bold Vibrant Tones =====
#define METIS_RED_DARKEST     "\033[38;2;36;21;39m"     // #241527
#define METIS_RED_DARK        "\033[38;2;65;29;49m"     // #411d31
#define METIS_RED_MEDIUM      "\033[38;2;117;36;56m"    // #752438
#define METIS_RED_LIGHT       "\033[38;2;165;48;48m"    // #a53030
#define METIS_RED_LIGHTER     "\033[38;2;207;87;60m"    // #cf573c
#define METIS_RED_LIGHTEST    "\033[38;2;218;134;62m"   // #da863e

// ===== GRAYS - Neutral Balanced Tones =====
#define METIS_GRAY_DARKEST    "\033[38;2;9;10;20m"      // #090a14
#define METIS_GRAY_DARK       "\033[38;2;21;29;40m"     // #151d28
#define METIS_GRAY_MEDIUM     "\033[38;2;57;74;80m"     // #394a50
#define METIS_GRAY_LIGHT      "\033[38;2;129;151;150m"  // #819796
#define METIS_GRAY_LIGHTER    "\033[38;2;168;181;178m"  // #a8b5b2
#define METIS_GRAY_LIGHTEST   "\033[38;2;199;207;204m"  // #c7cfcc
#define METIS_GRAY_WHITE      "\033[38;2;235;237;233m"  // #ebede9

// ===== SEMANTIC COLOR MAPPINGS =====
#define METIS_PRIMARY         METIS_BLUE_DARK
#define METIS_ACCENT          METIS_ORANGE_LIGHT
#define METIS_SUCCESS         METIS_GREEN_MEDIUM
#define METIS_WARNING         METIS_ORANGE_LIGHTER
#define METIS_ERROR           METIS_RED_LIGHT
#define METIS_INFO            METIS_BLUE_LIGHT

// ===== TEXT COLORS =====
#define METIS_TEXT_PRIMARY    METIS_GRAY_WHITE
#define METIS_TEXT_SECONDARY  METIS_GRAY_LIGHTER
#define METIS_TEXT_MUTED      METIS_GRAY_LIGHT

// ===== SPECIAL EFFECTS =====
#define METIS_BOLD            "\033[1m"
#define METIS_DIM             "\033[2m"
#define METIS_UNDERLINE       "\033[4m"
#define METIS_BLINK           "\033[5m"
#define METIS_REVERSE         "\033[7m"
#define METIS_RESET           "\033[0m"

// ===== DIVINE COMBINATIONS =====
#define METIS_FRAGMENT_TITLE  METIS_BOLD METIS_ORANGE_LIGHT
#define METIS_WISDOM_TEXT     METIS_BLUE_LIGHTER
#define METIS_VIOLATION_ERROR METIS_BOLD METIS_RED_LIGHT
#define METIS_VIOLATION_WARN  METIS_BOLD METIS_ORANGE_LIGHTER
#define METIS_VIOLATION_INFO  METIS_BLUE_LIGHT
#define METIS_CLICKABLE_LINK  METIS_UNDERLINE METIS_BLUE_LIGHT

// Functions for color management
bool metis_colors_supported(void);
void metis_colors_enable(bool enable);
const char* metis_color_for_severity(int severity);
const char* metis_color_for_type(int type);

#endif // METIS_COLORS_H

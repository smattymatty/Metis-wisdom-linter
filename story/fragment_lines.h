// story/fragment_lines.h - Technical Wisdom Fragments Organized by Divine Acts
// INSERT WISDOM HERE

#ifndef FRAGMENT_LINES_H
#define FRAGMENT_LINES_H

#include "metis_colors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Forward declaration - type defined in fragment_engine.h
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
// ACT-AWARE FRAGMENT POOLS
// =============================================================================

// --- ACT I: THE ORACLE'S WISDOM (LEVELS 1-10) ---
// Theme: Early guidance, building foundations, learning the basics
// Metis is still whole, speaking with love and hope

// Documentation Fragments - Act I
static const char* act_one_docs_fragments[] = {
    "A Note to the Future|Your comments are letters of kindness written to a future you. Be gentle with them.",
    "The First Sketch|Think of your documentation as the first loving sketch of a masterpiece. It guides the hand that will finish the work.",
    "A Welcoming Door|An undocumented function is a room with no door. A simple comment unlocks it for all who follow.",
    "The Light of Clarity|Code can be complex, a shadow-filled maze. Your comments are the lanterns that light the path for others.",
    "A Shared Thought|When you explain your code, you share a piece of your mind. It is a small, sacred gift."
};

// Daedalus Fragments - Act I
static const char* act_one_daedalus_fragments[] = {
    "The Right Tool|Why carve stone with fingernails? Daedalus's string libraries grow as needed, clean themselves.",
    "The Distant Forge|Raw `strcpy()` is bronze tools in a golden age. Daedalus's `d_String` functions remember bounds, grow with grace.",
    "The Mortal's Craft|A master craftsman in distant lands builds tools that outlast empires. Use his dynamic tools, not fragile static ones.",
    "Shared Mastery|The craftsman shares his libraries freely. Replace manual memory management with his collections.",
    "The Standard|Daedalus's containers: predictable, safe, elegant. Why settle for less?"
};

// Linting Fragments - Act I  
// Triggered by HEADER_VIOLATION conditions in metis_linter.c:794-806:
// - Missing filename header (first line): /* filename.c - description */
// - Missing wisdom header (second line): // INSERT WISDOM HERE
// - Improper documentation format in header files (3-line format required)
static const char* act_one_linting_fragments[] = {
    "A Crack in the Temple|An inconsistent style is a small crack in the foundation. It does not topple the temple today, but it lets the chaos in.",
    "The Shape of Logic|Your indentation gives shape to your logic. Make it beautiful, so others can see the elegant structure of your thought.",
    "The Power of a Name|A variable's name is its soul. Give it a name that speaks its purpose, and it will serve you faithfully.",
    "A Forgotten Prayer|An unused variable is a prayer said to a god who no longer listens. Let it go, and give its space to something new.",
    "A Kind Welcome|Consistent code style is a courtesy. It says to the next reader, 'Welcome. I made this place clean for you.'"
};


// Philosophy Fragments - Act I
static const char* act_one_philosophy_fragments[] = {
    "The Sacred Rhythm|The boulder is heavy, the hill is steep. But in the rhythm of the push, there is peace. Find the joy in your work, Sisyphus.",
    "A Choice of Order|Every line you write is a choice. You can invite chaos, or you can build a quiet, orderly temple. Choose to build a temple.",
    "The Coder's Heart|Your code is a mirror. If you feel rushed, it will be messy. If you feel calm, it will be clear. Take a breath. The code will know.",
    "A Question of Service|Do not only ask, 'Does it work?' Ask, 'Who does it serve?' Write code that serves the human who will read it next.",
    "An Act of Creation|You are not merely solving a problem. You are creating a small world with its own rules. Make it a world of beauty and simple grace."
};

// --- ACT II: THE PROPHECY'S WEIGHT (LEVELS 11-20) ---
// Theme: Growing complexity, hidden knowledge, the burden of foresight
// Metis knows the prophecy but hides it, tension building

// Documentation Fragments - Act II
static const char* act_two_docs_fragments[] = {
    "A Secret Kept|Unexplained code carries a heavy burden of secrets. I know this weight. Do not inflict it on others.",
    "A Fading Memory|The 'why' of your code is a fading ember. Document it now, before it grows cold and dark.",
    "A Bridge Across Doubt|When trust is thin, clarity is a lifeline. Let your comments be the bridge that others can depend on.",
    "A Plea for Understanding|I wish I could explain the shadows I see. You can. Please, explain the shadows in your code.",
    "The Lonely Architect|To build alone is a heavy task. Your documentation invites others into your world, so you do not have to build in silence."
};

// Daedalus Fragments - Act II
static const char* act_two_daedalus_fragments[] = {
    "Storm Preparation|Manual string handling is fragile as spun glass. Use robust libraries before pressure breaks foundations.",
    "Stress Testing|Poor tools fail under pressure. Daedalus's memory management endures. Raw `malloc()` breaks.",
    "Robust Foundations|Custom collections are castles on sand. Daedalus's structures are mountains.",
    "Self-Maintaining|Tools that work without constant tending. His logging captures truth. Your `printf()` will fail when needed most.",
    "Added Caution|Latest tools include more safety, more graceful failures. If the master adds caution, follow."
};

// Linting Fragments - Act II
static const char* act_two_linting_fragments[] = {
    "The First Doubt|A small inconsistency in style signals a deeper doubt in the design. As my faith in Zeus wavered, I saw it first in the smallest details.",
    "The Last Bridge|When a team is stressed, a shared code style is the last bridge that connects them. Do not let it break.",
    "A Point of Attack|I chose my words to Zeus carefully, lest he use them against me. A poorly named variable gives your critics a weapon.",
    "A Ghost in the Code|An unused variable is the ghost of a requirement that has changed. Acknowledge it, then give it peace.",
    "The Fraying Thread|When the great plan begins to fray, consistency is the one thread you must not let snap. Hold to it."
};
// Philosophy Fragments - Act II
static const char* act_two_philosophy_fragments[] = {
    "The Unseen Weight|That complex part of the code you don't want to touch? Its weight grows heavier every day you ignore it. I know this burden well.",
    "The Failing Trust|Write code that does not require trust to be understood. When alliances fail, only clarity remains a dependable friend.",
    "A Shadow of Debt|Technical debt is a shadow that follows you, growing longer as the day ends. Turn and face it now, while there is still light.",
    "A Merciful Truth|It is a kindness to reveal a difficult truth early. Your failing tests are not accusations; they are merciful warnings.",
    "The Architect's Sight|A good architect builds for the storm they know is coming. Write your code to withstand the future you cannot yet see."
};

// --- ACT III: THE CONSUMPTION (LEVELS 21-30) ---
// Theme: Confrontation, betrayal, transformation, the price of wisdom
// Metis faces Zeus, speaks truth, is consumed

// Documentation Fragments - Act III
static const char* act_three_docs_fragments[] = {
    "An Echo in the Void|When the author is gone, only their words remain. Let your comments be a kind echo.",
    "The Final Kindness|If you knew this was the last piece of code you would ever write, what final kindness would you leave for the one who comes next?",
    "A Message in a Bottle|Your documentation is a message cast into the sea of time. Write it with hope that it reaches a deserving shore.",
    "The Unburdening|To explain your most complex work is to unburden your own soul, and the soul of the one who must maintain it.",
    "A Forgiveness|Documenting a difficult function is an act of forgiveness—for yourself for its complexity, and for the next person who will struggle with it."
};

// Daedalus Fragments - Act III  
static const char* act_three_daedalus_fragments[] = {
    "Works Regardless|Tools that work regardless of maintainer. Custom code won't survive upheaval.",
    "Outlasting Creators|Creators fade, creations endure. Manual memory management forgotten. Libraries outlive empires.",
    "Self-Managing|Tools that work without their creator. Containers manage themselves. Yours require remembering everything.",
    "No Intervention|Tools requiring no intervention to function correctly. Yours do.",
    "Patient Protection|Tools serving with grace. Don't require perfect users. Protect imperfection."
};

// Linting Fragments - Act III
static const char* act_three_linting_fragments[] = {
    "The Last Rebellion|In the face of chaos, the small act of keeping your code clean is a rebellion. It is a refusal to surrender your discipline.",
    "A Dignified Exit|Even as I was consumed, I held my form. Let your code meet its future maintainer with the same dignity: clean, orderly, and proud.",
    "An Order Against Chaos|They demand you move fast, that you surrender to chaos. Do not. Perfect formatting is a shield against the coming storm.",
    "A Final Standard|Your code style is your final standard. When all other requirements fail, it is proof that you, the architect, did not.",
    "The Price of Haste|I paid a price for my wisdom. The code will pay a price for your haste. Both debts are called in eventually."
};
// Philosophy Fragments - Act III
static const char* act_three_philosophy_fragments[] = {
    "A Love for the Inheritor|Write this code not for yourself, but with love for the stranger who will one day have to understand it.",
    "The Honest Function|The most beautiful code is not that which pretends to be perfect, but that which is honest about its own limits.",
    "The Price of a Second|That extra second you spend on error handling is a price you pay now, to save a future soul hours of grief. Pay it gladly.",
    "A Gift in Breaking|Sometimes, to serve a greater purpose, a thing must be broken into smaller pieces. Do not fear modularity; it is a form of release.",
    "The Haunting|A shortcut haunts a codebase forever. A thoughtful design blesses it. Choose the spirit you wish to leave behind."
};

// --- ACT IV: SCATTERED CONSCIOUSNESS (LEVELS 31-40) ---  
// Theme: Learning to exist as fragments, finding new purpose
// Metis discovers her power in being scattered

// Documentation Fragments - Act IV
static const char* act_four_docs_fragments[] = {
    "A Thousand Windows|Scattered, I see through a thousand eyes. Your one clear comment can bring understanding to just as many.",
    "The Weaver's Thread|Each comment is a single, shimmering thread. Together, they weave a tapestry of understanding that anyone can see.",
    "The Gentle Ripple|From my new form, I see how a single act of clarity ripples out, touching projects and minds you will never know.",
    "A Silent Teacher|Well-documented code becomes a teacher in its own right, offering lessons in quiet moments of study.",
    "The Universal Truth|Being everywhere has taught me a simple truth: confusion is universal, but so is the relief of a clear explanation."
};

// Daedalus Fragments - Act IV
static const char* act_four_daedalus_fragments[] = {
    "Universal Connection|Memory management that works everywhere. Strings that grow anywhere. Custom code isolates.",
    "True Service|Tools that work without their creator truly serve. Libraries need no intervention. Yours constantly do.",
    "Modular Beauty|Components that fit together perfectly. Interchangeable, eternal.",
    "Independent Function|Tools functioning without guidance. Collections manage themselves. Arrays need constant remembering.",
    "Silent Grace|Tools catching mistakes with grace. Protect without shaming. Fix without blaming."
};

// Linting Fragments - Act IV
static const char* act_four_linting_fragments[] = {
    "The Universal Pattern|Scattered, I see the same stylistic errors a million times over. A consistent style is not a local rule; it is a universal language.",
    "The Integrity of the Part|Each fragment of my being holds its form. Each function of your code should do the same, beautiful in isolation.",
    "A Common Tongue|From here, I see that when developers cannot speak to each other, their code must speak for them. Let its style be clear.",
    "The Invisible Hand|Good formatting is an invisible hand that guides the reader's eye, making the complex seem simple.",
    "The Silent Lesson|One perfectly formatted file becomes a silent lesson for every developer who opens it. You have the power to teach without speaking."
};

// Philosophy Fragments - Act IV
static const char* act_four_philosophy_fragments[] = {
    "The Unseen Connection|From my new perspective, I see how your one, well-designed function brings harmony to ten others. Your small acts of order have vast, unseen effects.",
    "The Wisdom of the Parts|To be scattered is to learn that the whole is a lie; there are only pieces, working together. Make each of your pieces beautiful.",
    "A Quiet Influence|Good code does not need to be loud. It guides, it teaches, it serves, all without a single word. Its influence is a quiet one.",
    "The Patient System|A system made of patient, well-crafted parts can withstand any storm. It does not need to be fast, only resilient.",
    "The Beauty of Service|Even broken code can serve, if only as a warning. From my scattered view, I see that every line, perfect or flawed, has something to teach."
};

// --- ACT V: ETERNAL COMPASSION (LEVELS 41-50) ---
// Theme: Choosing love over bitterness, planting seeds for the future
// Metis embraces her role in creating the MIDAS collective

// Documentation Fragments - Act V
static const char* act_five_docs_fragments[] = {
    "A Garden for Others|To document your code is to plant a tree in whose shade you may never sit. Plant it anyway.",
    "A Light in the House|Your comments are a light you leave on in the house for the traveler who will arrive long after you have gone.",
    "A Faith in Tomorrow|Writing documentation is an act of faith—a belief that someone in the future is worthy of your kindness today.",
    "The Gift of Peace|A clear explanation is a gift of peace to a future, frustrated mind. It is a blessing you can bestow right now.",
    "The Shape of Love|If you were to give your code a shape, what would it be? Let your documentation give it the shape of love."
};


// Daedalus Fragments - Act V
static const char* act_five_daedalus_fragments[] = {
    "Digital Compassion|Tools serving with infinite patience. Libraries manifesting care in code.",
    "Practical Wisdom|Each tool making wisdom practical. Memory management as gift to strugglers.",
    "Acts of Service|Tools built as service. Containers catching mistakes with grace, not judgment.",
    "Care Made Code|Libraries expressing care for future developers. Use them, be blessed.",
    "Prevention Mission|Every function preventing segfaults is compassion made real. Tools as fragments of grace."
};

// Linting Fragments - Act V
static const char* act_five_linting_fragments[] = {
    "A Gift for the Eyes|Think of your code's formatting not as a rule, but as a gift of peace for the tired eyes that will one day read it.",
    "The Gentle Standard|The highest standard is simply this: be kind to the mind that will follow your own.",
    "A Chorus of Care|When you adhere to a shared style, your code joins a silent, eternal chorus of developers who care about each other.",
    "A Quiet Room|Your code should feel like a quiet, well-lit room, where the next person can enter and think clearly.",
    "An Act of Love|To choose order over your own convenience is a small act of love. From where I watch, it is the only act that truly matters."
};

// Philosophy Fragments - Act V
static const char* act_five_philosophy_fragments[] = {
    "The Choice|They can take everything from you, but they cannot take your choice to respond with grace. Write your next line with that grace.",
    "A Seed for the Future|Every compassionate function you write is a seed. It will grow into a forest of clarity for those who come after.",
    "The Long Arc|From here, I can see the long arc of your project. It bends toward simplicity. It bends toward kindness. Trust in the bend.",
    "A Blessing, Not a Curse|That difficult code you inherited is not a curse. It is a chance to leave something better than you found it. It is an opportunity for a blessing.",
    "The Infinite Game|The goal is not to finish the work. The goal is to love the work. The goal is to find meaning in the pushing of the boulder. The work is eternal. So is the love."
};


// =============================================================================
// FRAGMENT SELECTION LOGIC
// =============================================================================

// Function to get act-appropriate fragment based on wisdom level and type
static const char* get_act_fragment(FragmentType_t type, int wisdom_level) {
    const char** fragment_pool = NULL;
    int pool_size = 0;
    
    // Determine which act we're in
    int act = 1;
    if (wisdom_level <= 10) act = 1;
    else if (wisdom_level <= 20) act = 2;
    else if (wisdom_level <= 30) act = 3;
    else if (wisdom_level <= 40) act = 4;
    else act = 5;
    
    // Select appropriate fragment pool based on type and act
    switch (type) {
        case DOCS_FRAGMENT:
            switch (act) {
                case 1: fragment_pool = act_one_docs_fragments; pool_size = 5; break;
                case 2: fragment_pool = act_two_docs_fragments; pool_size = 5; break;
                case 3: fragment_pool = act_three_docs_fragments; pool_size = 5; break;
                case 4: fragment_pool = act_four_docs_fragments; pool_size = 5; break;
                case 5: fragment_pool = act_five_docs_fragments; pool_size = 5; break;
            }
            break;
            
        case DAEDALUS_FRAGMENT:
            switch (act) {
                case 1: fragment_pool = act_one_daedalus_fragments; pool_size = 5; break;
                case 2: fragment_pool = act_two_daedalus_fragments; pool_size = 5; break;
                case 3: fragment_pool = act_three_daedalus_fragments; pool_size = 5; break;
                case 4: fragment_pool = act_four_daedalus_fragments; pool_size = 5; break;
                case 5: fragment_pool = act_five_daedalus_fragments; pool_size = 5; break;
            }
            break;
            
        case LINTING_FRAGMENT:
            switch (act) {
                case 1: fragment_pool = act_one_linting_fragments; pool_size = 5; break;
                case 2: fragment_pool = act_two_linting_fragments; pool_size = 5; break;
                case 3: fragment_pool = act_three_linting_fragments; pool_size = 5; break;
                case 4: fragment_pool = act_four_linting_fragments; pool_size = 5; break;
                case 5: fragment_pool = act_five_linting_fragments; pool_size = 5; break;
            }
            break;
            
        case PHILOSOPHICAL_FRAGMENT:
            switch (act) {
                case 1: fragment_pool = act_one_philosophy_fragments; pool_size = 5; break;
                case 2: fragment_pool = act_two_philosophy_fragments; pool_size = 5; break;
                case 3: fragment_pool = act_three_philosophy_fragments; pool_size = 5; break;
                case 4: fragment_pool = act_four_philosophy_fragments; pool_size = 5; break;
                case 5: fragment_pool = act_five_philosophy_fragments; pool_size = 5; break;
            }
            break;
            
        default:
            return NULL;
    }
    
    if (!fragment_pool || pool_size == 0) return NULL;
    
    // Return random fragment from appropriate pool
    return fragment_pool[rand() % pool_size];
}

// Function to parse fragment into title and content
static void parse_story_fragment(const char* fragment, char* title, char* content, size_t title_size, size_t content_size) {
    const char* separator = strstr(fragment, "|");
    if (separator) {
        size_t title_len = separator - fragment;
        if (title_len >= title_size) title_len = title_size - 1;
        strncpy(title, fragment, title_len);
        title[title_len] = '\0';

        strncpy(content, separator + 1, content_size - 1);
        content[content_size - 1] = '\0';
    } else {
        strncpy(title, "WISDOM FRAGMENT", title_size - 1);
        title[title_size - 1] = '\0';
        strncpy(content, fragment, content_size - 1);
        content[content_size - 1] = '\0';
    }
}

#endif // FRAGMENT_LINES_H
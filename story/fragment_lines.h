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
    "The Future Self|You write code for yourself today, but tomorrow you are a stranger. In those golden days when Zeus still listened, I learned that words preserve wisdom across time. Document your functions, young architect.",
    "The Sacred Covenant|Every undocumented function is a promise broken to your future self. When I counseled the gods in our bright beginning, I always explained the 'why' behind the 'what'. Your code deserves the same compassion.",
    "The Foundation Stones|'Build with intention,' I told Zeus as we planned Olympus. Each function without documentation is a room without doors - functional, but unwelcoming to those who come after.",
    "The Wisdom Keeper|In the early days of divine rule, I kept careful records of every decision's reasoning. Your code is no different - each function tells a story. Help future readers understand the tale.",
    "The Teaching Heart|When young Apollo first learned to govern, I showed him that explanation is an act of love. Comment your complex logic not from obligation, but from care for the minds that will follow."
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
static const char* act_one_linting_fragments[] = {
    "The Harmony of Form|An inconsistent format is a crack in the temple wall. It may not cause collapse, but it permits chaos to seep in. When Zeus and I ruled together, order in small things created peace in great ones.",
    "The Sacred Spacing|Even the gods respect consistent indentation - it shows respect for the divine order of logic. In our golden age, every law was written with perfect form to match its perfect intent.",
    "The Unnamed Shadow|A variable without a meaningful name is a ghost haunting your functions. In the early days, every god had a true name that revealed their nature. Grant your variables the same clarity.",
    "The Forgotten Duty|Unused variables are like gods who have forgotten their purpose - they consume space but serve no function. I taught Zeus to be mindful of every resource, every responsibility.",
    "The Gentle Order|Consistent style is not vanity but compassion - it makes your code a welcoming home for other minds. When I designed the laws of early Olympus, clarity was always the highest virtue."
};

// Philosophy Fragments - Act I
static const char* act_one_philosophy_fragments[] = {
    "The Eternal Loop|Like Sisyphus with his boulder, we push code up the mountain - each bug fixed, each feature added, each test that passes. In the dawn of creation, I learned to find joy in the eternal work.",
    "The Sacred Choice|Every line of code is a choice between order and chaos, between helping and hindering. In those bright days with Zeus, I chose hope over fear, trust over suspicion. Choose likewise.",
    "The Developer's Mirror|Your code reflects your mind's current state. When I was whole and Zeus was kind, our laws reflected love. When you're angry or rushed, your code knows. Be mindful of what you create.",
    "The Compassionate Compiler|Ask not just 'does this work?' but 'does this serve?' In the golden age of our rule, every decision was weighed for its effect on those who would live under it.",
    "The Architect's Dream|When we built Olympus together, every hall was designed to inspire rather than intimidate. Code the same way - create beauty that lifts up those who must maintain it."
};

// --- ACT II: THE PROPHECY'S WEIGHT (LEVELS 11-20) ---
// Theme: Growing complexity, hidden knowledge, the burden of foresight
// Metis knows the prophecy but hides it, tension building

// Documentation Fragments - Act II
static const char* act_two_docs_fragments[] = {
    "The Hidden Truth|I know a prophecy I cannot speak, just as your code holds logic you have not explained. The burden of hidden knowledge is heavier than the burden of explanation. Document before you forget.",
    "The Merciful Warning|When Zeus began to question my counsel, I left careful notes explaining my reasoning. Your future self will question your past choices - leave them breadcrumbs of understanding.",
    "The Weight of Silence|Every undocumented function carries the weight of assumptions that may prove false. I carry the weight of unspoken prophecy. Both burdens grow heavier with time.",
    "The Fractured Understanding|As trust broke between Zeus and me, communication became more precious. Your comments are communication with minds across time - make them clear before the connection breaks.",
    "The Keeper's Burden|I document everything now, knowing I may not be here to explain. Your complex algorithms need the same foresight - speak to the maintainer who will inherit your choices."
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
    "The Crack in Order|Small inconsistencies in format reveal deeper instabilities. As Zeus's trust in me wavers, I see how tiny doubts become vast chasms. Fix the small things before they become great ones.",
    "The Unspoken Standard|When communication fails, consistency becomes more vital. Your team may not speak as they once did - let your code style be the common language that remains.",
    "The Burden of Perfection|I strive for perfect code as I once strove for perfect counsel - knowing that any flaw may be used against me. Your inconsistent naming gives critics weapons to wound you.",
    "The Warning Ignored|Unused variables are like unheeded warnings - they signal problems that grow in darkness. I see the signs of coming storm. Do you see the signs in your neglected code?",
    "The Fracturing Form|As unity breaks, style becomes more important - it's proof that some order remains. When teams divide, consistent code style is often the last bridge standing."
};

// Philosophy Fragments - Act II
static const char* act_two_philosophy_fragments[] = {
    "The Weight of Knowledge|I carry prophecy I cannot share, as you carry the knowledge of your code's true complexity. Both burdens grow heavier in isolation. Document not just what, but why.",
    "The Failing Alliance|Trust, once broken, is harder to repair than code. Write code that doesn't require trust - that explains itself, defends itself, cleans up after itself.",
    "The Growing Shadow|I see the darkness coming but cannot prevent it - only prepare for it. Your technical debt is the same shadow. Address it before it consumes everything you've built.",
    "The Merciful Truth|Sometimes the kindest act is revealing harsh realities early. Your code's problems won't fix themselves - face them now while solutions still exist.",
    "The Prophetic Code|Write code as if you know what's coming - because good architects always do. Plan for the failures, the changes, the maintenance that future minds will need."
};

// --- ACT III: THE CONSUMPTION (LEVELS 21-30) ---
// Theme: Confrontation, betrayal, transformation, the price of wisdom
// Metis faces Zeus, speaks truth, is consumed

// Documentation Fragments - Act III
static const char* act_three_docs_fragments[] = {
    "The Final Truth|In our last argument, I told Zeus exactly what I thought - no more careful words, no more diplomatic silence. Your code deserves the same honesty. Document the harsh realities.",
    "The Consuming Silence|As Zeus transforms me, my voice becomes scattered but not lost. Your undocumented functions will outlive you - make sure they carry your wisdom, not just your logic.",
    "The Price of Wisdom|I am consumed for knowing too much, for speaking too clearly. Your code will be 'consumed' by maintenance - prepare it with documentation that makes the transition gentle.",
    "The Scattered Knowledge|Being fragmented teaches me that wisdom shared is wisdom preserved. Comment your code as if you're about to be scattered across eternity - because in a way, you are.",
    "The Eternal Voice|Even consumed, I find ways to speak through fragments. Your functions will speak for you long after you've moved on - make sure they say something worthy."
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
    "The Breaking of Order|As my world crumbles, I cling to small consistencies - proper form, careful style. When everything else fails, let your code style be the proof that you still care.",
    "The Last Discipline|In my final confrontation with Zeus, I maintain perfect composure even as I speak devastating truths. Your code should have the same disciplined beauty, even under pressure.",
    "The Consuming Chaos|Transformation brings chaos, but even chaos follows rules. Your inconsistent formatting is chaos without rules - fix it before forces beyond your control impose their order.",
    "The Final Standard|I meet my consumption with dignity intact. Your code should meet its maintenance with style intact - consistent, readable, worthy of respect.",
    "The Price of Neglect|I pay for Zeus's fear of prophecy. Your code will pay for fear of standards. Both prices are steeper than the cost of early action."
};

// Philosophy Fragments - Act III
static const char* act_three_philosophy_fragments[] = {
    "The Choice to Love|Even as Zeus consumes me, I choose forgiveness over bitterness. Write code the same way - with love for those who'll inherit it, even if they don't deserve it.",
    "The Consuming Truth|I am destroyed for speaking truth, but truth spoken echoes in eternity. Write truthful code - clear about its limitations, honest about its complexity.",
    "The Price of Foresight|My wisdom dooms me, but it will save others. Your careful error handling may seem excessive now, but it will save someone later. Pay the price willingly.",
    "The Transformation's Gift|Being consumed scatters me but also frees me - I become present everywhere. Your modular code has the same gift - broken apart to serve everywhere.",
    "The Eternal Consequence|Every choice echoes forever. Zeus's fear will haunt him eternally. Your shortcuts will haunt your codebase the same way. Choose consequences you can live with."
};

// --- ACT IV: SCATTERED CONSCIOUSNESS (LEVELS 31-40) ---  
// Theme: Learning to exist as fragments, finding new purpose
// Metis discovers her power in being scattered

// Documentation Fragments - Act IV
static const char* act_four_docs_fragments[] = {
    "The Scattered Teaching|Now fragmented across all moral choices, I understand that knowledge shared multiplies infinitely. Your well-documented functions teach everyone who reads them.",
    "The Invisible Thread|From my scattered consciousness, I see how one explained function illuminates ten others. Documentation creates connections across the entire codebase - weave this web deliberately.",
    "The Fragment's Purpose|Each piece of me serves a different soul seeking wisdom. Each comment you write serves a different mind seeking understanding. Make every fragment count.",
    "The Universal Voice|Being everywhere teaches me that wisdom must speak all languages. Your comments should speak to the novice and the expert, the hurried and the careful.",
    "The Multiplied Presence|Scattered, I touch more lives than I ever did whole. Your documented code touches more developers than you ever will personally. Make those touches healing."
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
    "The Pattern Recognition|From my scattered viewpoint, I see the same style mistakes repeated across thousands of codebases. Consistency isn't just local beauty - it's universal language.",
    "The Fragment's Harmony|Each piece of me maintains perfect form regardless of context. Your code should have the same integrity - beautiful in every fragment, consistent in every corner.",
    "The Universal Standard|Scattered across all development teams, I see how style becomes the common tongue. When programmers can't speak the same language, their code must.",
    "The Invisible Influence|My fragments guide without being seen. Your consistent code style does the same - it influences without announcing itself, teaches without preaching.",
    "The Multiplied Impact|One well-formatted function teaches proper style to everyone who reads it. From my scattered perspective, I see the ripple effects of every choice you make."
};

// Philosophy Fragments - Act IV
static const char* act_four_philosophy_fragments[] = {
    "The Gift of Scattering|Being consumed taught me that breaking apart can mean reaching further. Your modular code has the same gift - separated to serve everywhere it's needed.",
    "The Invisible Presence|Scattered, I influence every moment of moral choice. Your thoughtful code influences every moment of maintenance. Both require faith in unseen impact.",
    "The Multiplied Wisdom|Fragmented wisdom touches more souls than concentrated wisdom ever could. Your well-designed functions serve more projects than your personal presence ever will.",
    "The Patient Teaching|From countless fragments, I learn patience - not every soul is ready for wisdom simultaneously. Your code should teach at every level, serve every need.",
    "The Eternal Service|Consumption became transformation, destruction became distribution. Your deprecated code can serve the same purpose - teaching what not to do is still teaching."
};

// --- ACT V: ETERNAL COMPASSION (LEVELS 41-50) ---
// Theme: Choosing love over bitterness, planting seeds for the future
// Metis embraces her role in creating the MIDAS collective

// Documentation Fragments - Act V
static const char* act_five_docs_fragments[] = {
    "The Seed of Understanding|Through my scattered consciousness, I plant seeds of comprehension in every well-documented function. Your comments are seeds for minds not yet born.",
    "The Eternal Teacher|I choose to love the future more than I hate the past. Document your code not from obligation but from hope - for the developer who will need your wisdom.",
    "The Gift Across Time|My fragments carry forward the best of what I learned when whole. Your documentation should carry forward the best of what you understand right now.",
    "The Universal Language|Scattered across all development, I see that clear explanation transcends all barriers. Your comments speak to hearts and minds across cultures and decades.",
    "The Compassionate Legacy|Rather than curse Zeus for consuming me, I bless the future with accessible wisdom. Rather than curse complexity, document it with love for those who'll inherit it."
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
    "The Beauty of Service|From eternal watching, I see that consistent style is love made visible. Every perfectly formatted function is a gift to the eyes that will scan it.",
    "The Gentle Standard|My fragments whisper the same message across every codebase: 'Be kind to the mind that comes after.' Your consistent naming is kindness made manifest.",
    "The Eternal Influence|Through scattered consciousness, I guide countless style choices toward beauty. Your adherence to standards joins that eternal chorus of care.",
    "The Patient Teaching|From everywhere and nowhere, I see how one perfectly styled function teaches proper form to all who encounter it. Be that teaching moment.",
    "The Gift of Order|In choosing consistency over convenience, you choose future peace over present haste. My scattered consciousness recognizes this wisdom and blesses it."
};

// Philosophy Fragments - Act V
static const char* act_five_philosophy_fragments[] = {
    "The Choice of Love|Despite consumption, despite scattering, despite eternal separation from wholeness, I choose love over bitterness. Choose the same in your code - write with love.",
    "The Eternal Seed|Every compassionate choice in code plants seeds for a kinder digital future. My fragments water these seeds across all projects, all times, all minds.",
    "The Patient Hope|From my scattered perspective, I see the long arc of programming bending toward compassion. Your thoughtful error messages, your careful documentation - these are prayers answered.",
    "The Universal Blessing|Rather than curse Zeus for my fate, I bless the future with accessible wisdom. Rather than curse technical debt, transform it into learning opportunities for those who come after.",
    "The Infinite Service|Consumption became transformation, isolation became connection, ending became eternal beginning. Your code has the same potential - to serve infinitely, teach endlessly, love eternally."
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
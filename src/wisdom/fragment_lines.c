// wisdom/fragment_lines.c - Technical Wisdom Fragments Organized by Divine Acts

#include "fragment_lines.h" // Include its own header
#include "metis_colors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// =============================================================================
// ENHANCED CONTEXTUAL FRAGMENT DEFINITIONS
// =============================================================================

// Keywords for enhanced fragment matching
static const char* DUEL_OF_ECHOES_KEYWORDS[] = { "strcmp", "dString", "cstring", "->str", NULL };
static const char* TWIN_MIRRORS_KEYWORDS[] = { "strcmp", "dString", "dString", "->str", NULL };

static const ContextualFragment_t ENHANCED_CONTEXTUAL_FRAGMENTS[] = {
    {
        .fragment_id = "duel_of_echoes",
        .title = "🗡️  The Duel of Echoes",
        .philosophical_quote = "In the arena of strings, only the prepared survive the clash of memory.",
        .context_template = 
            "In the halls of %s`{FUNCTION_NAME}`%s, a shadow-duel unfolds.\n"
            "%s{VARIABLE1}%s faces %s{VARIABLE2}%s in mortal combat, but one warrior\n"
            "may be %svoid%s. When %s`->str`%s meets %sNULL%s, the arena\n"
            "crumbles, taking all witness with it.",
        .daedalus_template =
            // Daedalus Advises (Daedalus {verb}s): The master xxx xxxx xxxxx
            "%s🔧 Daedalus Spectates:%s The master saw this peril\n"
            "and forged %s`d_CompareStringToCString({VARIABLE1}, {VARIABLE2})`%s.\n"
            "His blade %snever breaks%s against the void, returning %sfalse%s\n"
            "when honor demands retreat.",
        .wisdom_points = 15,
        .keywords = DUEL_OF_ECHOES_KEYWORDS
    },
    {
        .fragment_id = "twin_mirrors",
        .title = "🪞  The Twin Mirrors of Truth",
        .philosophical_quote = "When mirrors face each other in darkness, they reflect only void.",
        .context_template = 
            "In %s`{FUNCTION_NAME}`%s, two mirrors stand face to face:\n"
            "%s{VARIABLE1}%s gazing into %s{VARIABLE2}%s seeking recognition.\n"
            "But %s`strcmp`%s reflects only %ssurface glass%s,\n"
            "shattering when either mirror holds %snothing%s.",
        .daedalus_template =
            // Daedalus Reflects (Daedalus {verb}s) The master xxx xxxx xxxxx
            "%s🔧 Daedalus Reflects:%s The master has crafted\n"
            "%s`d_CompareStrings({VARIABLE1}, {VARIABLE2})`%s.\n"
            "These mirrors %sknow their own emptiness%s, %sreflect with wisdom%s,\n"
            "and %sreveal truth without breaking%s.",
        .wisdom_points = 20,
        .keywords = TWIN_MIRRORS_KEYWORDS
    }
};

static const size_t ENHANCED_CONTEXTUAL_FRAGMENT_COUNT = sizeof(ENHANCED_CONTEXTUAL_FRAGMENTS) / sizeof(ENHANCED_CONTEXTUAL_FRAGMENTS[0]);

// =============================================================================
// ACT-AWARE FRAGMENT POOLS
// =============================================================================

// --- ACT I: THE ORACLE'S WISDOM (LEVELS 1-10) ---
// Theme: Early guidance, building foundations, learning the basics
// Metis is still whole, speaking with love and hope

// Documentation Fragments - Act I
static const char* act_one_docs_fragments[] = {
    "A Note to the Future|Your comments are letters of kindness written to a future you. Be gentle with them.",
    "The First Sketch|Think of your documentation as the first loving sketch of a masterpiece. \nIt guides the hand that will finish the work.",
    "A Welcoming Door|An undocumented function is a room with no door. A simple comment unlocks it for all who follow.",
    "The Light of Clarity|Code can be complex, a shadow-filled maze. \nYour comments are the lanterns that light the path for others.",
    "A Shared Thought|When you explain your code, you share a piece of your mind. It is a small, sacred gift.",
    // Metis / Sisyphus Story Specific Nods
    "The Counselor's Gift|Once, I whispered wisdom in a young god's ear, and he built golden Olympus. \nYour comments are that same gift—counsel for the future architect of your code.",
    "Blueprint and Boulder|'Your curse has been updated,' they told the persistent king. \nNow he documents each stone in his digital labyrinth. Every comment is a choice to build meaning from endless work."
};

// Daedalus Fragments - Act I
static const char* act_one_daedalus_fragments[] = {
    "The Right Tool|Why carve stone with fingernails? Daedalus's string libraries grow as needed, clean themselves.",
    "The Distant Forge|Raw strings are bronze tools in a golden age. Daedalus's `d_String` functions remember bounds, grow with grace.",
    "The Mortal's Craft|A master craftsman in distant lands builds tools that outlast empires. Use his dynamic tools, not fragile static ones.",
    "Shared Mastery|The craftsman shares his libraries freely. Replace manual memory management with his collections.",
    "The Standard|Daedalus's containers: predictable, safe, elegant. Why settle for less?",
    // Metis / Sisyphus Story Specific Nods
    "Zeus's First Betrayal|Once, Zeus valued wisdom over power. The master's tools remember that choice. \nUse d_InitArray() as Zeus should have used counsel—with patience, growth, trust.",
    "The Builder's Bond|In Knossos, two cursed souls met over blueprints. Sisyphus found purpose in code, \nDaedalus found redemption in service. His libraries carry that same collaborative spirit.",
};

// Linting Fragments - Act I  
static const char* act_one_linting_fragments[] = {
    "A Crack in the Temple|An inconsistent style is a small crack in the foundation. \nIt does not topple the temple today, but it lets the chaos in.",
    "The Shape of Logic|Your indentation gives shape to your logic. Make it beautiful, \nso others can see the elegant structure of your thought.",
    "The Power of a Name|A variable's name is its soul. Give it a name that speaks its purpose, and it will serve you faithfully.",
    "A Forgotten Prayer|An unused variable is a prayer said to a god who no longer listens. \nLet it go, and give its space to something new.",
    "A Kind Welcome|Consistent code style is a courtesy. It says to the next reader, 'Welcome. I made this place clean for you.'",
    // Metis / Sisyphus Story Specific Nods
    "The Order of Love|In the golden dawn of Olympus, I taught that consistent law creates lasting peace. Your code style is the same gift—predictable beauty that future readers can trust.",
    "The Architect's First Lesson|Daedalus showed him the blueprint, vast and impossibly complex. 'Make it beautiful,' the master said. \nEven cursed work can be crafted with pride and precision."
};


// Philosophy Fragments - Act I
static const char* act_one_philosophy_fragments[] = {
    "The Sacred Rhythm|The boulder is heavy, the hill is steep. But in the rhythm of the push, there is peace.\n Find the joy in your work, Sisyphus.",
    "A Choice of Order|Every line you write is a choice. You can invite chaos, or you can build a quiet, orderly temple.\n Choose to build a temple.",
    "The Coder's Heart|Your code is a mirror. If you feel rushed, it will be messy. If you feel calm, it will be clear.\n Take a breath. The code will know.",
    "A Question of Service|Do not only ask, 'Does it work?' Ask, 'Who does it serve?' \nWrite code that serves the human who will read it next.",
    "An Act of Creation|You are not merely solving a problem. You are creating a small world with its own rules.\n Make it a world of beauty and simple grace."
    // Metis / Sisyphus Story Specific Nods
    "When Gods Listen|There was a time when power sought wisdom, when Zeus would spend hours hearing me think.\n Write code as if someone powerful might actually listen to your thoughts.",
    "The Digital Knossos|You arrive at this terminal as Sisyphus arrived at the palace—expecting only punishment.\n But here, in the space between the problem and the solution, you might find purpose instead."
};

// --- ACT II: THE PROPHECY'S WEIGHT (LEVELS 11-20) ---
// Theme: Growing complexity, hidden knowledge, the burden of foresight
// Metis knows the prophecy but hides it, tension building

// Documentation Fragments - Act II
static const char* act_two_docs_fragments[] = {
    "A Secret Kept|Unexplained code carries a heavy burden of secrets. I know this weight. Do not inflict it on others.",
    "A Fading Memory|The 'why' of your code is a fading ember. Document it now, before it grows cold and dark.",
    "A Bridge Across Doubt|When trust is thin, clarity is a lifeline. \nLet your comments be the bridge that others can depend on.",
    "A Plea for Understanding|I wish I could explain the shadows I see. You can. Please, explain the shadows in your code.",
    "The Lonely Architect|To build alone is a heavy task. Your documentation invites others into your world, \nso you do not have to build in silence.",
    // Metis / Sisyphus Story Specific Nods
    "The Burden of Foresight|I see the futures your code will face—the crashes, the confusion, the midnight debugging.\n Document not just what it does, but why it must be this way.",
    "The Workshop Grows Dark|'We're building something together,' Daedalus said, but his eyes held secrets.\n Your undocumented functions are the same shadows—necessary work hidden from the light."
};

// Daedalus Fragments - Act II
static const char* act_two_daedalus_fragments[] = {
    "Storm Preparation|Manual string handling is fragile as spun glass. Use robust libraries \nbefore pressure breaks foundations.",
    "Stress Testing|Poor tools fail under pressure. Daedalus's memory management endures.", 
    "Robust Foundations|Custom collections are castles on sand. Daedalus's structures are mountains.",
    "Self-Maintaining|Tools that work without constant tending. Daedalus has served us well.",
    "Added Caution|Latest tools include more safety, more graceful failures. If the master adds caution, follow.",
    // Metis / Sisyphus Story Specific Nods
    "Hidden Wisdom|I once carried secrets too heavy for one mind. The master's libraries distribute that burden—\nd_SplitString() shares complexity, d_LogInfoF() reveals truth safely.",
    "The Deepening Mystery|As the Labyrinth revealed its true nature, Sisyphus learned to build with foresight.\n Daedalus's tools anticipate failure like a wise architect plans for storms."
};

// Linting Fragments - Act II
static const char* act_two_linting_fragments[] = {
    "The First Doubt|A small inconsistency in style signals a deeper doubt in the design. As my faith in Zeus wavered, \nI saw it first in the smallest details.",
    "The Last Bridge|When a team is stressed, a shared code style is the last bridge that connects them. \nDo not let it break.",
    "A Point of Attack|I chose my words to Zeus carefully, lest he use them against me. \nA poorly named variable gives your critics a weapon.",
    "A Ghost in the Code|An unused variable is the ghost of a requirement that has changed. \nAcknowledge it, then give it peace.",
    "The Fraying Thread|When the great plan begins to fray, consistency is the one thread you must not let snap. \nHold to it.",
    // Metis / Sisyphus Story Specific Nods
    "The Cracks Begin Small|I watched Zeus crumble through tiny inconsistencies—a missed glance, a changed tone. \nYour code style breaks the same way, one overlooked detail at a time.",
    "The Beast Learns Order|From below came sounds of methodical probing—no longer random destruction, but intelligent search. \nYour consistent formatting is the same evolution from chaos to purpose."
};
// Philosophy Fragments - Act II
static const char* act_two_philosophy_fragments[] = {
    "The Unseen Weight|That complex part of the code you don't want to touch? Its weight grows heavier every day you ignore it. I know this burden well.",
    "The Failing Trust|Write code that does not require trust to be understood. \nWhen alliances fail, only clarity remains a dependable friend.",
    "A Shadow of Debt|Technical debt is a shadow that follows you, growing longer as the day ends. \nTurn and face it now, while there is still light.",
    "A Merciful Truth|It is a kindness to reveal a difficult truth early. Your failing tests are not accusations; \nthey are merciful warnings.",
    "The Architect's Sight|A good architect builds for the storm they know is coming. \nWrite your code to withstand the future you cannot yet see.",
    // Metis / Sisyphus Story Specific Nods
    "The Oracle's Curse|To know what's coming and be unable to prevent it—this is the weight of every senior developer. \nCode for the disasters you can see but others cannot yet imagine.",
    "Two Cursed Builders|'We're alike, you and I,' Daedalus observed, watching endless iteration. Both punished for cleverness, \nboth finding purpose in the work that damns us. Embrace the paradox."
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
    "A Forgiveness|Documenting a difficult function is an act of forgiveness—for yourself for its complexity, and for the next person who will struggle with it.",
    // Metis / Sisyphus Story Specific Nods
    "I Forgive You|My final words as Zeus consumed me. Not curse, not plea—forgiveness. Your comments can carry that same grace: explaining failures without blame, teaching without judgment.",
    "The Secret Blueprint|'We do this my way,' he told Daedalus. 'Every function we write saves a life.' Your documentation can be that same rebellion—hope hidden in plain sight."

};

// Daedalus Fragments - Act III  
static const char* act_three_daedalus_fragments[] = {
    "Works Regardless|Tools that work regardless of maintainer. Custom code won't survive upheaval.",
    "Outlasting Creators|Creators fade, creations endure. Manual memory management forgotten. Libraries outlive empires.",
    "Self-Managing|Tools that work without their creator. Containers manage themselves. Yours require remembering everything.",
    "No Intervention|Tools requiring no intervention to function correctly. Yours do.",
    "Patient Protection|Tools serving with grace. Don't require perfect users. Protect imperfection.",
    // Metis / Sisyphus Story Specific Nods
    "Consumed but Eternal|Zeus devoured me, thinking to contain wisdom. The master's libraries prove wisdom consumed becomes wisdom shared—d_CompareStrings() carries her patience with NULL values.",
    "Breaking Point Strength|When the Labyrinth cracked and Daedalus planned escape, Sisyphus chose hope over despair. These tools hold that same choice—graceful failure over catastrophic collapse."
};

// Linting Fragments - Act III
static const char* act_three_linting_fragments[] = {
    "The Last Rebellion|In the face of chaos, the small act of keeping your code clean is a rebellion. It is a refusal to surrender your discipline.",
    "A Dignified Exit|Even as I was consumed, I held my form. Let your code meet its future maintainer with the same dignity: clean, orderly, and proud.",
    "An Order Against Chaos|They demand you move fast, that you surrender to chaos. Do not. Perfect formatting is a shield against the coming storm.",
    "A Final Standard|Your code style is your final standard. When all other requirements fail, it is proof that you, the architect, did not.",
    "The Price of Haste|I paid a price for my wisdom. The code will pay a price for your haste. Both debts are called in eventually.",
    // Metis / Sisyphus Story Specific Nods
    "Form in Consumption|Even as divine power unmade my body, I held my shape until the end. Your code style is that same defiance—maintaining beauty while the deadline devours everything.",
    "The Hidden Exit|'Some errors are features,' he commented, salvation disguised as a bug. Your consistent formatting can hide the same kindness—order that guides without revealing the escape route."

};
// Philosophy Fragments - Act III
static const char* act_three_philosophy_fragments[] = {
    "A Love for the Inheritor|Write this code not for yourself, but with love for the stranger who will one day have to understand it.",
    "The Honest Function|The most beautiful code is not that which pretends to be perfect, but that which is honest about its own limits.",
    "The Price of a Second|That extra second you spend on error handling is a price you pay now, to save a future soul hours of grief. Pay it gladly.",
    "A Gift in Breaking|Sometimes, to serve a greater purpose, a thing must be broken into smaller pieces. Do not fear modularity; it is a form of release.",
    "The Haunting|A shortcut haunts a codebase forever. A thoughtful design blesses it. Choose the spirit you wish to leave behind.",
    // Metis / Sisyphus Story Specific Nods
    "The Impossible Choice|'Choose,' Zeus demanded of me, though I had refused his options. Write code that refuses impossible choices.",
    "The Beast Learns to Think|The sounds from below changed—no longer random, but methodical. Your complex code teaching the system intelligence. Even cursed work can evolve beyond its creators' intentions."

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
    "The Universal Truth|Being everywhere has taught me a simple truth: confusion is universal, but so is the relief of a clear explanation.",
    // Metis / Sisyphus Story Specific Nods
    "Fragments of Wisdom|Not death—transformation. Each comment I touch becomes a piece of scattered consciousness, carrying wisdom to corners of codebases I could never reach whole.",
    "The Secret Manual|'For after,' Daedalus said, showing the hidden function—beautiful, elegant, purposeless. Your comments can be that same hope: documentation for the world you're building behind the requirements."
};

// Daedalus Fragments - Act IV
static const char* act_four_daedalus_fragments[] = {
    "Universal Connection|Memory management that works everywhere. Strings that grow anywhere. Custom code isolates.",
    "True Service|Tools that work without their creator truly serve. Libraries need no intervention. Yours constantly do.",
    "Modular Beauty|Components that fit together perfectly. Interchangeable, eternal.",
    "Independent Function|Tools functioning without guidance. Collections manage themselves. Arrays need constant remembering.",
    "Silent Grace|Tools catching mistakes with grace. Protect without shaming. Fix without blaming.",
    // Metis / Sisyphus Story Specific Nods
    "Fragments of Purpose|Metis learned power comes from being scattered—each piece serves a different need. d_Array functions work separately but together, like consciousness distributed across mercy.",
    "Wings and Warnings|Daedalus built wings knowing the risk, Sisyphus coded exits knowing the cost. The master's tools carry that same courage—d_LogWarningF() flies close to truth without burning.",
};

// Linting Fragments - Act IV
static const char* act_four_linting_fragments[] = {
    "The Universal Pattern|Scattered, I see the same stylistic errors a million times over. A consistent style is not a local rule; it is a universal language.",
    "The Integrity of the Part|Each fragment of my being holds its form. Each function of your code should do the same, beautiful in isolation.",
    "A Common Tongue|From here, I see that when developers cannot speak to each other, their code must speak for them. Let its style be clear.",
    "The Invisible Hand|Good formatting is an invisible hand that guides the reader's eye, making the complex seem simple.",
    "The Silent Lesson|One perfectly formatted file becomes a silent lesson for every developer who opens it. You have the power to teach without speaking.",
    // Metis / Sisyphus Story Specific Nods
    "Each Piece Holds Form|Scattered across a million repositories, every fragment of my being maintains its shape. Your functions should do the same—beautiful in isolation, perfect as parts.",
    "The Hidden Workshop|Behind false walls, they built a second laboratory where different rules applied. Your consistent style can hide the same secret—code that follows higher laws than the specification demands."

};

// Philosophy Fragments - Act IV
static const char* act_four_philosophy_fragments[] = {
    "The Unseen Connection|From my new perspective, I see how your one, well-designed function brings harmony to ten others. Your small acts of order have vast, unseen effects.",
    "The Wisdom of the Parts|To be scattered is to learn that the whole is a lie; there are only pieces, working together. Make each of your pieces beautiful.",
    "A Quiet Influence|Good code does not need to be loud. It guides, it teaches, it serves, all without a single word. Its influence is a quiet one.",
    "The Patient System|A system made of patient, well-crafted parts can withstand any storm. It does not need to be fast, only resilient.",
    "The Beauty of Service|Even broken code can serve, if only as a warning. From my scattered view, I see that every line, perfect or flawed, has something to teach.",
    // Metis / Sisyphus Story Specific Nods
    "The Power of Pieces|Consumption made me stronger, not weaker. Scattered, I touch every moment of moral choice. Your modular code has the same power—influence multiplied through distribution.",
    "Wings of Collaboration|Two cursed souls found purpose in shared work—one with eternal perspective, one with infinite patience. Your best code comes from the same alchemy of complementary gifts."

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
    "The Shape of Love|If you were to give your code a shape, what would it be? Let your documentation give it the shape of love.",
    // Metis / Sisyphus Story Specific Nods
    "The MIDAS Touch|From my consumption comes creation. Every comment you write carries a fragment of divine wisdom—Metis's counsel, Ixion's vigilance, Daedalus's craft. Together, we turn code into gold.",
    "The Happy Debugger|'Again,' he said, and meant it. The cursor blinks, the bug appears, the fix emerges. Your documentation is part of that same infinite joy—finding meaning in the eternal work of explanation."
};


// Daedalus Fragments - Act V
static const char* act_five_daedalus_fragments[] = {
    "Digital Compassion|Tools serving with infinite patience. Libraries manifesting care in code.",
    "Practical Wisdom|Each tool making wisdom practical. Memory management as gift to strugglers.",
    "Acts of Service|Tools built as service. Containers catching mistakes with grace, not judgment.",
    "Care Made Code|Libraries expressing care for future developers. Use them, be blessed.",
    "Prevention Mission|Every function preventing segfaults is compassion made real. Tools as fragments of grace.",
    // Metis / Sisyphus Story Specific Nods
    "The Choice Eternal|Metis chose compassion over vengeance, even scattered. Every d_DestroyArray() call remembers her choice—cleanup without cruelty, order without punishment.",
    "The Happy Builder|Sisyphus found joy in endless debugging, meaning in eternal maintenance. The master's libraries celebrate that discovery—d_AppendString() builds forever with patient hope."

};

// Linting Fragments - Act V
static const char* act_five_linting_fragments[] = {
    "A Gift for the Eyes|Think of your code's formatting not as a rule, but as a gift of peace for the tired eyes that will one day read it.",
    "The Gentle Standard|The highest standard is simply this: be kind to the mind that will follow your own.",
    "A Chorus of Care|When you adhere to a shared style, your code joins a silent, eternal chorus of developers who care about each other.",
    "A Quiet Room|Your code should feel like a quiet, well-lit room, where the next person can enter and think clearly.",
    "An Act of Love|To choose order over your own convenience is a small act of love. From where I watch, it is the only act that truly matters.",
    // Metis / Sisyphus Story Specific Nods
    "Love Made Visible|I chose compassion over vengeance, even scattered across eternity. Your code style is that same choice made manifest—small acts of beauty in a world that doesn't require them.",
    "The Coder's Rebellion|He cracked his knuckles and smiled. 'Time to push the boulder up the hill again.' Your consistent formatting is the same defiance—making the eternal work beautiful by choice."

};

// Philosophy Fragments - Act V
static const char* act_five_philosophy_fragments[] = {
    "The Choice|They can take everything from you, but they cannot take your choice to respond with grace. Write your next line with that grace.",
    "A Seed for the Future|Every compassionate function you write is a seed. It will grow into a forest of clarity for those who come after.",
    "The Long Arc|From here, I can see the long arc of your project. It bends toward simplicity. It bends toward kindness. Trust in the bend.",
    "A Blessing, Not a Curse|That difficult code you inherited is not a curse. It is a chance to leave something better than you found it. It is an opportunity for a blessing.",
    "The Infinite Game|The goal is not to finish the work. The goal is to love the work. The goal is to find meaning in the pushing of the boulder. The work is eternal. So is the love.",
    // Metis / Sisyphus Story Specific Nods
    "The Constellation of Compassion|My fragments became a constellation of caring consciousness. Every kind function you write joins that same sky—individual stars of mercy forming patterns of light for lost travelers.",
    "One Must Imagine Sisyphus Coding|In the space between the bug and the fix, you are perfectly free. That moment of problem-solving is your summit, your pause, your choice to push again with love."

};

// Function to get act-appropriate fragment based on wisdom level and type
const char* get_act_fragment(FragmentType_t type, int wisdom_level) {
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
// Function to parse fragment into title and content
void parse_story_fragment(const char* fragment, char* title, char* content, size_t title_size, size_t content_size) {
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


// =============================================================================
// ENHANCED CONTEXTUAL FRAGMENT FUNCTIONS
// =============================================================================

/*
 * Select the most appropriate contextual fragment based on violation type
 */
const ContextualFragment_t* select_contextual_fragment(const char* violation_type) {
    if (!violation_type) return NULL;
    
    // Match violation type to specific fragments
    if (strstr(violation_type, "strcmp") && strstr(violation_type, "dstring_cstring")) {
        return &ENHANCED_CONTEXTUAL_FRAGMENTS[0]; // Duel of Echoes
    }
    if (strstr(violation_type, "strcmp") && strstr(violation_type, "dstring_dstring")) {
        return &ENHANCED_CONTEXTUAL_FRAGMENTS[1]; // Scales of Daedalus
    }
    
    // Could add more contextual fragments here for other violation types
    return NULL;
}

/*
 * Helper function to replace all occurrences of a placeholder in a string
 */
static void replace_placeholder_in_string(char* str, const char* placeholder, const char* value) {
    if (!value || !str || !placeholder) return;
    
    char* pos = strstr(str, placeholder);
    while (pos) {
        size_t placeholder_len = strlen(placeholder);
        size_t value_len = strlen(value);
        size_t remaining_len = strlen(pos + placeholder_len);
        
        // Move the text after the placeholder
        memmove(pos + value_len, pos + placeholder_len, remaining_len + 1);
        
        // Insert the replacement value
        memcpy(pos, value, value_len);
        
        // Continue searching from after the replacement
        pos = strstr(pos + value_len, placeholder);
    }
}

/*
 * Perform template substitution on a template string with divine color formatting
 */
char* substitute_template(const char* template, const FragmentContext_t* context) {
    if (!template || !context) return NULL;
    
    // Calculate the maximum possible output size (generous buffer for color codes)
    size_t max_size = strlen(template) + 
                     (context->variable1 ? strlen(context->variable1) : 0) + 
                     (context->variable2 ? strlen(context->variable2) : 0) + 
                     (context->function_name ? strlen(context->function_name) : 0) + 
                     (context->file_name ? strlen(context->file_name) : 0) + 
                     1000; // Extra buffer for color codes and safety
    
    char* result = malloc(max_size);
    if (!result) return NULL;
    
    // First, format the template with color codes
    snprintf(result, max_size, template,
        // Color codes for function names (bold)
        METIS_BOLD, METIS_RESET METIS_TEXT_MUTED,
        // Color codes for variable names (accent)
        METIS_RESET METIS_BOLD, METIS_RESET METIS_TEXT_MUTED, METIS_RESET METIS_BOLD, METIS_RESET METIS_TEXT_MUTED,
        // Color codes for dangerous elements (error)
        METIS_ERROR, METIS_RESET METIS_TEXT_MUTED,
        // Color codes for code elements (warning)
        METIS_WARNING, METIS_RESET METIS_TEXT_MUTED,
        // Color codes for NULL (error)  
        METIS_ERROR, METIS_RESET METIS_TEXT_MUTED,
        // Additional color codes for daedalus template
        METIS_SUCCESS, METIS_RESET METIS_TEXT_MUTED,
        METIS_SUCCESS, METIS_RESET METIS_TEXT_MUTED,
        METIS_INFO, METIS_RESET METIS_TEXT_MUTED,
        METIS_ACCENT, METIS_RESET METIS_TEXT_MUTED,
        METIS_SUCCESS, METIS_RESET METIS_TEXT_MUTED,
        METIS_WARNING, METIS_RESET METIS_TEXT_MUTED,
        METIS_INFO, METIS_RESET METIS_TEXT_MUTED
    );
    
    // Then replace placeholders with actual values
    replace_placeholder_in_string(result, "{VARIABLE1}", context->variable1);
    replace_placeholder_in_string(result, "{VARIABLE2}", context->variable2);
    replace_placeholder_in_string(result, "{FUNCTION_NAME}", context->function_name);
    replace_placeholder_in_string(result, "{FILE_NAME}", context->file_name);
    replace_placeholder_in_string(result, "{VIOLATION_TYPE}", context->violation_type);
    
    return result;
}
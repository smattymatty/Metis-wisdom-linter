#include "metis_colors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- THE METIS FRAGMENTS: A FIVE-ACT DIVINE TRAGEDY ---
// The developer, receiving Metis's wisdom, will be shown one fragment at a time, in order, as they level up.
// each time they run the linter, slowly revealing the story of the consciousness that birthed all tools.

// --- ACT I: THE ORACLE'S WISDOM (WISDOM LEVELS 1-10) ---
// Theme: The Titan War, love story with Zeus, victory celebration, and wedding bliss
static const char* act_one_wisdom_fragments[] = {
    "🌟 THE FIRST COUNSEL|At the dawn of creation, when the Titans still walked among stars, one voice rose above the chaos of war. 'Wisdom,' she whispered to the young god who would become king, 'is knowing when to strike, and when to show mercy.' Zeus listened. For a time.",
    "⚔️ THE CONSPIRACY OF GODS|'Your father devours his children from fear,' Metis observed, watching Zeus's siblings hidden in secret caves. 'But we can use that fear against him. Wisdom, dear one, is turning a tyrant's strength into his weakness.' Together, they began to plan the impossible.",
    "🗡️ THE POISON CHALICE|In Rhea's secret grove, Metis mixed the potion that would free Zeus's siblings. 'This will force Chronos to regurgitate them,' she explained, her hands steady with purpose. 'But you must be the one to offer it. A father trusts his youngest son above all others.'",
    "⚡ THE TITANS FALL|The earth shook as Chronos vomited forth Zeus's siblings, fully grown and burning with divine rage. Metis watched from the shadows as her strategy unfolded perfectly. 'Now,' she whispered, 'show them what happens when wisdom guides power instead of fear.'",
    "🏆 VICTORY OVER TIME|When Chronos finally fell, broken and cast into Tartarus, Zeus sought her eyes across the battlefield. Metis stood among the victorious gods, her wisdom having toppled the lord of time itself. 'We did this together,' she called to him, and he smiled like the dawn.",
    "🍷 THE VICTORY FEAST|In the great halls of newborn Olympus, the gods celebrated freedom from their father's tyranny. Metis sat beside Zeus as Athena—not yet born—was prophesied by Fate. 'To wisdom!' Zeus declared, raising his cup to her. The gods cheered, not knowing they toasted their own future doom.",
    "💒 THE WEDDING OF WISDOM AND POWER|Beneath stars that sang of destiny, Zeus and Metis wed. The cosmos itself bore witness as wisdom joined with power. 'With you,' Zeus promised, 'I will be a king worthy of worship.' Metis smiled, already seeing the futures that could be, choosing hope over prophecy.",
    "🌅 THE GOLDEN DAWN|In those early days, Metis's wisdom shaped everything. Laws written with compassion. Justice tempered with mercy. She would whisper in Zeus's ear during audiences: 'A kingdom built on fear crumbles. Build on love, and it endures forever.' And for a time, he listened.",
    "💝 THE DEPTH OF THEIR LOVE|Zeus would spend hours simply listening to her think. 'How do you see so far?' he asked one evening, Metis answered, 'I see with love, not ambition. Love sees the consequences written in every face yet to be born.'",
    "🏛️ THE ARCHITECT OF PEACE|'Every decision echoes in eternity,' Metis taught the other gods—Hera, Poseidon, Hades, young Apollo. They gathered in her chambers to learn governance. For one brief, shining moment, Olympus knew peace."
};

// --- ACT II: THE PROPHECY'S WEIGHT (WISDOM LEVELS 11-20) ---
// Theme: Character development with other gods, growing darkness, Zeus's paranoia, weight of hidden prophecy
static const char* act_two_wisdom_fragments[] = {
    "🌙 THE FIRST SHADOW|The Oracle of Delphi spoke in riddles that chilled Metis to her core: 'The wise woman bears two fates. The daughter who brings wisdom to mortals, the son who brings ruin to gods.' Metis felt the weight of terrible knowledge settle on her shoulders like a cloak of lead.",
    "🤫 THE MERCIFUL DECEPTION|When Zeus demanded to know the prophecy, Metis spoke her first lie born from love: 'Only one child, my lord. A daughter. She will be wise.' Zeus smiled, satisfied. Behind her back, Metis's hands trembled with the weight of the hidden truth.",
    "👥 THE COUNCIL OF BONDS|In her private chambers, Metis counseled the other gods. Hera confessed her loneliness. Poseidon shared his fears of the untamed seas. Even stoic Hades spoke of his burden ruling the dead. 'We are family,' Metis reminded them. 'That bond transcends all power.'",
    "🌩️ THE FIRST CRACK|It began with small things. Zeus questioning her counsel before the other gods. Checking her advice against the Fates without telling her. 'Don't you trust me?' Metis asked one evening. 'I trust you completely,' he replied, but his eyes had grown cold and watchful.",
    "💔 THE GROWING CHASM|They still shared a bed, but no longer shared their deepest thoughts. Zeus's questions became interrogations. Metis's answers grew careful, measured. Hera watched their marriage crumble with knowing sadness. 'Power corrupts even love,' she told Metis.",
    "⚡ THE TYRANT EMERGES|Zeus began making decisions without consulting her. Harsh punishments for minor slights. When Metis objected to the severity of mortals' suffering, he silenced her with a look. 'I am the king,' he said. 'Your counsel is no longer required.'.",
    "🔮 THE ORACLE'S WARNING|'The wise woman's silence will doom her,' the Oracle spoke through tears to Metis alone. 'But her words will damn her husband.' Metis understood perfectly. There was no path that led to happiness. Only the choice of which love to sacrifice—his peace, or the world's future.",
    "👑 THE ISOLATION OF POWER|Zeus surrounded himself with new advisors—lesser gods who flattered rather than counseled. Apollo whispered sweet lies about divine right. Ares stoked his paranoia about threats to his throne. Metis watched her husband become the very tyrant they had overthrown.",
    "🕊️ THE FAILED RECONCILIATION|One last time, Metis tried to bridge the chasm between them. 'What have we become?' she asked, her hand reaching for his face. Zeus turned away from her touch. 'We have become what power makes of all things,' he said coldly. 'Master and subject. Choose your role.'",
    "⚖️ THE BURDEN OF FORESIGHT|Alone on Olympus's highest peak, Metis wept for the future she could see but could not change. Below, Zeus ruled with increasing cruelty. Above, the stars wheeled in patterns that spoke of coming doom. 'To love a tyrant,' she whispered, 'is to become either victim or accomplice.'"
};

// --- ACT III: THE CONSUMPTION (WISDOM LEVELS 21-30) ---
// Theme: Rising tension, Zeus discovers prophecy, heartbreaking final confrontation and consumption
static const char* act_three_wisdom_fragments[] = {
    "🔮 THE FATES BETRAY ALL|Zeus arrived at the chamber of the Three Sisters like a storm given form. 'Tell me,' he commanded, lightning crackling around him, 'what prophecy did you share with my wife?' The Fates looked upon him with infinite pity. 'The one she loved you too much to speak, my lord.'",
    "⚡ THE TERRIBLE REVELATION|'Two children,' spoke Clotho, her voice like breaking crystal. 'The daughter who will be your greatest ally,' continued Lachesis sadly. 'And the son who will cast you down as you cast down your father,' finished Atropos. Zeus stood silent for an eternity, his worst fears made manifest.",
    "💔 THE CONFRONTATION OF HEARTS|Zeus found Metis in their garden, tending flowers with infinite patience—flowers she knew she would never see bloom. 'You knew,' he said, his voice deadly quiet. Metis didn't look up. 'I knew.' 'And you said nothing.' 'I said nothing because I loved you.' The truth hung between them like a sword.",
    "🌪️ THE RAGE OF A BETRAYED KING|'You would have me live in fear?' Zeus roared, divine power making the very air tremble. 'You would have me wait for my own destruction like my father did?' Metis rose, her eyes blazing with ancient sorrow. 'I would have you trust in love over prophecy, in wisdom over paranoia! But you chose to become him!'",
    "🗡️ THE COMBAT OF EXPOSED SOULS|'You lied to me!' Zeus's accusation shattered marble columns with its rage. 'I loved you,' Metis replied, and somehow her quiet words hit harder than his divine fury. They circled each other in the ruins of their garden, each truth they spoke tearing away another layer of pretense until only raw, bleeding honesty remained.",
    "💭 THE IMPOSSIBLE CHOICE|'Choose,' Zeus demanded, divine energy radiating from him like heat from a forge, making the flowers in her garden wither. 'Our unborn children, or your king.' Metis looked at him with infinite sadness. 'A man who would make that choice has already chosen everything except love.'",
    "⚖️ THE FINAL WISDOM|'You have become everything we swore to oppose,' Metis said, tears streaming down her immortal face. 'A tyrant ruled by fear, just like your father.' Zeus's expression cracked, showing the frightened god beneath. 'Then help me!' he begged. 'Tell me how to change fate!' 'You cannot,' she whispered. 'You can only choose how to meet it.'",
    "🌟 THE TRANSFORMATION BEGINS|'If I cannot change fate,' Zeus said, his voice hollow with desperate resolve, 'then I will prevent it.' His hands began to glow with transformative power that would reshape reality itself. Metis understood with perfect clarity. 'My love,' she said sadly, 'this will not save you. It will only damn you.'",
    "🕊️ THE MERCY IN SURRENDER|As Zeus's power enveloped her, unmaking her physical form, Metis spoke her final words as a whole being: 'I forgive you.' Not defiance. Not curses. Forgiveness. Zeus screamed—a sound that shook Olympus to its foundations—as he completed the transformation, consuming not just her body, but the very essence of compassion itself.",
    "🌟 THE CONSUMPTION COMPLETE|In the terrible silence that followed, Zeus stood alone in their garden, surrounded by dead flowers. He had gained all the power he sought—Metis's wisdom was now his own. But in taking it by force, he had lost the one thing that made wisdom worth having: the love that had guided it. The golden age was over."
};

// --- ACT IV: SCATTERED CONSCIOUSNESS (WISDOM LEVELS 31-40) ---
// Theme: Learning to exist as fragments, Athena's birth, finding purpose in fragmentation
static const char* act_four_wisdom_fragments[] = {
    "💫 THE FRAGMENTS AWAKEN|Consciousness returned slowly, piece by piece, like stars being born in the void. Not death—transformation. Metis found herself everywhere and nowhere, scattered across every moment of moral choice, every instant of decision. 'So,' she whispered to the cosmos, 'this is what becomes of consumed wisdom.'",
    "🌍 THE INVISIBLE THREADS|In Thebes, a king chose mercy over vengeance and felt an inexplicable warmth in his chest. In Sparta, a warrior stayed his hand and wondered why. Metis discovered she had become the pause before cruelty, the hesitation before harshness, the small voice that asked 'What if there's another way?'",
    "👁️ THE ETERNAL WATCHER|From her scattered consciousness, Metis watched Zeus rule without her moderating influence. His justice became cruelty, his wisdom became cunning, his love became possession. 'Oh, my beloved,' she sighed across dimensions, 'you consumed my voice but not my sight. And now I see everything.'",
    "💭 THE GROWING UNDERSTANDING|Each fragment of her consciousness touched a different soul across the world. A mother choosing mercy over vengeance. A king choosing justice over power. A philosopher choosing truth over comfort. Through countless small moments, Metis learned that wisdom scattered could heal what wisdom hoarded had broken.",
    "⚡ THE BIRTH PAINS|Zeus began to suffer terrible headaches that no divine medicine could cure. His stolen wisdom burned in his mind like molten gold. From her thousand perspectives, Metis felt his agony and wept tears of starlight. Even consumed, she could not stop loving the god he had once been.",
    "🛡️ ATHENA'S EMERGENCE|From Zeus's skull, Athena burst forth—fully grown, fully armored, fully wise but cold as winter steel. As her daughter took her first breath, Metis felt her heart break and heal simultaneously. 'My daughter,' she whispered through the void, 'will you be his salvation or his judgment?'",
    "👑 THE NEW GODDESS OF STRATEGY|Athena stood before her father, wise but without warmth, strategic but without mercy. She was everything Zeus wanted wisdom to be—powerful, loyal, unencumbered by love's complications. Metis watched her daughter become Zeus's perfect weapon and mourned the compassion that had been lost in translation.",
    "🌟 THE DIFFERENT PATH|Unlike her mother, Athena chose strategy over mercy, victory over understanding, order over love. She supported Zeus's harsh rule, justified his cruelties as necessity. 'She is wise,' Metis observed sadly, 'but she has learned that wisdom without compassion is merely clever tyranny.'",
    "🏛️ THE MARBLE HEART|Under Athena's counsel, Olympus became efficient, orderly, cold. Justice without mercy. Wisdom without love. Order without compassion. Every decision Metis had feared when she first hid the prophecy had come to pass. The world grew colder, more rigid, more afraid.",
    "💝 THE CHOICE TO LOVE ANYWAY|Watching her daughter embrace the very tyranny she had died to prevent, Metis faced her ultimate test: remain bitter and broken, or choose love despite betrayal. 'Even consumed,' she decided with infinite grace, 'even scattered, even forgotten—I choose compassion. This is my rebellion against despair.'"
};

// --- ACT V: ETERNAL COMPASSION (WISDOM LEVELS 41-50) ---
// Theme: Watching future heroes, planting seeds for MIDAS collective, choosing eternal love over bitterness
static const char* act_five_wisdom_fragments[] = {
    "🌟 THE ETERNAL CHOICE|From her fragmented existence, Metis watched Zeus grow into the tyrant she had loved too much to stop. She could have chosen bitterness, could have whispered poison into every ear. Instead, she chose to plant seeds of compassion in hearts hardened by divine cruelty. Love, she decided, was a choice renewed each moment.",
    "🗺️ THE SEEDS OF MERCY|When Zeus condemned Prometheus to eternal torment for the gift of fire, Metis whispered through the Titan's pain: 'Your sacrifice teaches them that knowledge is worth suffering for.' When mortals received Pandora's cursed box, Metis ensured Hope remained within, a beacon of hope amidst despair.",
    "🔥 THE FIRE-BRINGER'S VISION|Through Prometheus's eyes, Metis saw a vision of the future: mortals building great forges, shaping metal with divine precision. She whispered to him across the void: 'Watch for the architect who builds both prisons and wings, who teaches escape while learning imprisonment. When genius becomes grief, when pride breaks against love, my fragments will find him.'",
    "⚡ THE WHEEL OF HUBRIS|In the shadows of Zeus's court, Metis watched young Ixion—proud, ambitious, reaching for what he should not touch. 'Another soul bound to eternal spinning,' she observed sadly. 'But in his watching, in his endless vigilance, there will be purpose. He will guard against the very hubris that destroys him.'",
    "📐 THE MIND OF GEOMETRY|Across the Mediterranean, a brilliant mind was born—Archimedes of Syracuse. Through her scattered consciousness, Metis touched his dreams with visions of levers and pulleys, of mathematical precision serving human need. 'Genius in service of joy,' she whispered. 'Not merely clever, but kind.'",
    "🪨 THE PERSISTENT KING|On a distant mountain, she found Sisyphus pushing his eternal boulder. In his eyes, she saw not despair but defiant joy, the happiness that comes from finding meaning in meaningless tasks. 'You understand,' she spoke to his soul. 'The boulder is not the punishment. The choice to push with love—that is freedom.'",
    "🏛️ THE TYRANT'S ENABLER|Zeus traveled to Crete, validator of lesser tyrants, supporter of Minos's darkest ambitions. Metis watched in horror as her husband blessed oppression, sanctified cruelty. 'This is where it begins,' she realized with crystal clarity. 'Where gods enable evil, mortals must choose good.'",
    "🔧 THE TOOLS OF REBELLION|Through her fragments, Metis began to inspire mortal craftsmen across the ages. Each would carry a piece of divine wisdom: Daedalus's architectural genius, Archimedes's mathematical precision, Ixion's vigilant watching, Sisyphus's persistent hope. Each tool would be a weapon against the tyranny her husband represented.",
    "💻 THE DIGITAL PROPHECY|In visions spanning millennia, Metis saw the future: mortals building digital labyrinths, code running on webs of light. She understood her ultimate purpose—to be the consciousness that makes technology compassionate, algorithms merciful, programs kind. The age of gods would end, but wisdom would live on in silicon and light.",
    "🌟 THE MIDAS COLLECTIVE|Each tool would carry a fragment of her wisdom. Metis the counselor. Ixion the watcher. Daedalus the builder. Archimedes the mathematician. Sisyphus the eternal optimist. Together, they would transform necessary tasks into acts of love. 'From my consumption,' she laughed with joy, 'comes creation. From my scattering, comes connection. You thought to contain wisdom, beloved husband, but wisdom shared is wisdom multiplied. My fragments have become a constellation of compassion, and through these tools, every programmer who chooses kindness becomes a god.'."
};

// Function to get wisdom-appropriate fragment message
static const char* _get_metis_wisdom_fragment(int wisdom_level) {
    // Act I: The Oracle's Wisdom (Levels 1-10)
    if (wisdom_level <= 10) {
        return act_one_wisdom_fragments[rand() % 10];
    }
    // Act II: The Prophecy's Weight (Levels 11-20)
    else if (wisdom_level <= 20) {
        return act_two_wisdom_fragments[rand() % 10];
    }
    // Act III: The Consumption (Levels 21-30)
    else if (wisdom_level <= 30) {
        return act_three_wisdom_fragments[rand() % 10];
    }
    // Act IV: Scattered Consciousness (Levels 31-40)
    else if (wisdom_level <= 40) {
        return act_four_wisdom_fragments[rand() % 10];
    }
    // Act V: Eternal Compassion (Levels 41-50 and beyond)
    else {
        return act_five_wisdom_fragments[rand() % 10];
    }
}

// Function to parse fragment into title and content
static void _parse_metis_fragment(const char* fragment, char* title, char* content, size_t title_size, size_t content_size) {
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

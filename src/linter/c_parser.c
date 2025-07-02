/* c_parser.c - Divine C language parser for Metis wisdom extraction */
// INSERT WISDOM HERE

#define _POSIX_C_SOURCE 200809L

#include "c_parser.h"
#include "metis_colors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>

static bool _is_valid_number_char(char c, bool* has_dot, bool* has_e, char next_c);
static int _read_hex_number(ParserContext_t* ctx, char* buffer, size_t buffer_size, int current_length);
static int _read_float_exponent(ParserContext_t* ctx, char* buffer, size_t buffer_size, int current_length);
static int _read_decimal_integer_or_float_part(ParserContext_t* ctx, char* buffer, size_t buffer_size, int current_length, bool* has_dot_out, bool* has_e_out);

static bool _is_two_char_operator(char c1, char c2, char* op_buffer);
static bool _is_single_char_operator(char c);
static const char* TWO_CHAR_OPERATORS[] = {
    "==", "!=", "<=", ">=", "&&", "||", "++", "--", "<<", ">>",
    "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "->",
    NULL
};

static bool _attempt_tokenize_next_token(ParserContext_t* ctx, ParsedFile_t* parsed, char* buffer, size_t buffer_size);
static bool _tokenize_comment(ParserContext_t* ctx, ParsedFile_t* parsed, char c, char next_c, int start_line, int start_column, char* buffer, size_t buffer_size);
static bool _tokenize_preprocessor(ParserContext_t* ctx, ParsedFile_t* parsed, int start_line, int start_column, char* buffer, size_t buffer_size);
static bool _tokenize_string_char_literal(ParserContext_t* ctx, ParsedFile_t* parsed, char c, int start_line, int start_column, char* buffer, size_t buffer_size);
static bool _tokenize_number(ParserContext_t* ctx, ParsedFile_t* parsed, char c, char next_c, int start_line, int start_column, char* buffer, size_t buffer_size);
static bool _tokenize_identifier_keyword(ParserContext_t* ctx, ParsedFile_t* parsed, char c, int start_line, int start_column, char* buffer, size_t buffer_size);
static bool _tokenize_operator(ParserContext_t* ctx, ParsedFile_t* parsed, int start_line, int start_column, char* buffer, size_t buffer_size);
static bool _tokenize_punctuation(ParserContext_t* ctx, ParsedFile_t* parsed, char c, int start_line, int start_column, char* buffer);
static bool _tokenize_newline(ParserContext_t* ctx, ParsedFile_t* parsed, int start_line, int start_column);
static bool _tokenize_unknown(ParserContext_t* ctx, ParsedFile_t* parsed, char c, int start_line, int start_column, char* buffer); // For unknown characters

// =============================================================================
// TOKEN RECOGNITION TABLES
// =============================================================================

// C Keywords for divine recognition of language constructs
static const char* C_KEYWORDS[] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "inline", "int", "long", "register", "restrict", "return", "short",
    "signed", "sizeof", "static", "struct", "switch", "typedef", "union",
    "unsigned", "void", "volatile", "while", "_Bool", "_Complex", "_Imaginary",
    "_Static_assert", "_Thread_local", "_Alignas", "_Alignof", "_Atomic",
    "_Generic", "_Noreturn",
    NULL
};

// Dangerous functions that should be replaced with Daedalus alternatives
static const char* DANGEROUS_FUNCTIONS[] = {
    "gets", "strcpy", "strcat", "sprintf", "vsprintf",
    "strlen", "strncpy", "strncat", "snprintf", "vsnprintf",
    "malloc", "calloc", "realloc", "free",
    "printf", "fprintf",
    //"scanf", "fscanf", "sscanf", "atoi", "atol", "atof",
    NULL
};

// Common type keywords and identifiers
static const char* TYPE_KEYWORDS[] = {
    "int", "char", "float", "double", "void", "bool", "size_t",
    "uint8_t", "uint16_t", "uint32_t", "uint64_t",
    "int8_t", "int16_t", "int32_t", "int64_t",
    "FILE", "NULL", "true", "false",
    NULL
};

// =============================================================================
// PARSER CONTEXT & UTILITIES
// =============================================================================

/*
 * Initialize a new parser context for divine understanding
 */
static void init_parser_context(ParserContext_t* ctx, const char* source) {
    if (!ctx || !source) return;

    memset(ctx, 0, sizeof(ParserContext_t));
    ctx->source = source;
    ctx->source_length = strlen(source);
    ctx->line = 1;
    ctx->column = 1;
    ctx->position = 0;
}

/*
 * Create a new parsed file structure with divine initialization
 */
static ParsedFile_t* create_parsed_file(const char* file_path) {
    ParsedFile_t* parsed = calloc(1, sizeof(ParsedFile_t));
    if (!parsed) return NULL;

    parsed->file_path = strdup(file_path ? file_path : "unknown");
    if (!parsed->file_path) {
        free(parsed);
        return NULL;
    }

    // Initialize token array with divine capacity
    parsed->token_capacity = 2000;
    parsed->tokens = malloc(sizeof(Token_t) * parsed->token_capacity);

    // Initialize function array
    parsed->function_capacity = 100;
    parsed->functions = malloc(sizeof(FunctionInfo_t) * parsed->function_capacity);

    // Initialize include array
    parsed->include_capacity = 50;
    parsed->includes = malloc(sizeof(char*) * parsed->include_capacity);

    if (!parsed->tokens || !parsed->functions || !parsed->includes) {
        c_parser_free_parsed_file(parsed);
        return NULL;
    }

    return parsed;
}

/*
 * Expand array capacity with divine growth management
 */
static bool expand_token_capacity(ParsedFile_t* parsed) {
    if (!parsed) return false;

    size_t new_capacity = parsed->token_capacity * 2;
    Token_t* new_tokens = realloc(parsed->tokens, sizeof(Token_t) * new_capacity);
    if (!new_tokens) return false;

    parsed->tokens = new_tokens;
    parsed->token_capacity = new_capacity;
    return true;
}

/*
 * Expand function array capacity
 */
static bool expand_function_capacity(ParsedFile_t* parsed) {
    if (!parsed) return false;

    size_t new_capacity = parsed->function_capacity * 2;
    FunctionInfo_t* new_functions = realloc(parsed->functions, sizeof(FunctionInfo_t) * new_capacity);
    if (!new_functions) return false;

    parsed->functions = new_functions;
    parsed->function_capacity = new_capacity;
    return true;
}

// =============================================================================
// TOKEN MANAGEMENT
// =============================================================================

/*
 * Add a token to the parsed file with divine growth management
 */
static bool add_token(ParsedFile_t* parsed, TokenType_t type, const char* value,
                      int line, int column, int length) {
    if (!parsed || !value) return false;

    // Expand capacity if needed
    if (parsed->token_count >= parsed->token_capacity) {
        if (!expand_token_capacity(parsed)) return false;
    }

    Token_t* token = &parsed->tokens[parsed->token_count];
    token->type = type;
    token->value = strdup(value);
    if (!token->value) return false;

    token->line = line;
    token->column = column;
    token->length = length;

    parsed->token_count++;
    return true;
}

/*
 * Add a function to the parsed file with divine tracking
 */
static bool add_function(ParsedFile_t* parsed, const char* name, const char* return_type,
                         int line, int column, bool is_static, bool is_inline) {
    if (!parsed || !name) return false;

    // Expand capacity if needed
    if (parsed->function_count >= parsed->function_capacity) {
        if (!expand_function_capacity(parsed)) return false;
    }

    FunctionInfo_t* func = &parsed->functions[parsed->function_count];
    memset(func, 0, sizeof(FunctionInfo_t));

    func->name = strdup(name);
    func->return_type = strdup(return_type ? return_type : "unknown");
    if (!func->name || !func->return_type) return false;

    func->line_number = line;
    func->column = column;
    func->is_static = is_static;
    func->is_inline = is_inline;
    func->param_count = 0;
    func->parameters = NULL;
    func->has_documentation = false;

    parsed->function_count++;
    return true;
}

/*
 * Add an include directive to the parsed file
 */
static bool add_include(ParsedFile_t* parsed, const char* include_path) {
    if (!parsed || !include_path) return false;

    // Expand capacity if needed
    if (parsed->include_count >= parsed->include_capacity) {
        size_t new_capacity = parsed->include_capacity * 2;
        char** new_includes = realloc(parsed->includes, sizeof(char*) * new_capacity);
        if (!new_includes) return false;

        parsed->includes = new_includes;
        parsed->include_capacity = new_capacity;
    }

    parsed->includes[parsed->include_count] = strdup(include_path);
    if (!parsed->includes[parsed->include_count]) return false;

    parsed->include_count++;
    return true;
}

// =============================================================================
// LEXICAL ANALYSIS - DIVINE TOKENIZATION
// =============================================================================

/*
 * Skip whitespace and update position with line tracking
 */
static void skip_whitespace(ParserContext_t* ctx) {
    while (ctx->position < ctx->source_length &&
           isspace(ctx->source[ctx->position])) {
        if (ctx->source[ctx->position] == '\n') {
            ctx->line++;
            ctx->column = 1;
        } else {
            ctx->column++;
        }
        ctx->position++;
    }
}

/*
 * Read an identifier or keyword with proper bounds checking
 */
static int read_identifier(ParserContext_t* ctx, char* buffer, size_t buffer_size) {
    int length = 0;

    while (ctx->position < ctx->source_length &&
           (isalnum(ctx->source[ctx->position]) || ctx->source[ctx->position] == '_') &&
           length < (int)(buffer_size - 1)) {
        buffer[length] = ctx->source[ctx->position];
        length++;
        ctx->position++;
        ctx->column++;
    }

    buffer[length] = '\0';
    return length;
}

/*
 * Read a string literal with escape sequence handling
 */
static int read_string(ParserContext_t* ctx, char* buffer, size_t buffer_size) {
    int length = 0;
    char quote_char = ctx->source[ctx->position];
    bool escaped = false;

    // Include opening quote
    buffer[length++] = quote_char;
    ctx->position++;
    ctx->column++;

    while (ctx->position < ctx->source_length && length < (int)(buffer_size - 1)) {
        char c = ctx->source[ctx->position];
        buffer[length++] = c;

        if (!escaped && c == quote_char) {
            ctx->position++;
            ctx->column++;
            break;
        }

        escaped = (!escaped && c == '\\');

        if (c == '\n') {
            ctx->line++;
            ctx->column = 1;
        } else {
            ctx->column++;
        }
        ctx->position++;
    }

    buffer[length] = '\0';
    return length;
}
/*
 * Helper function to determine if a character is valid within a number literal,
 * considering current parsing state (e.g., if a decimal or exponent has already been seen).
 * Updates has_dot and has_e flags as necessary.
 *
 * `c` - The current character to evaluate.
 * `has_dot` - Pointer to a boolean flag indicating if a decimal point has been encountered.
 * `has_e` - Pointer to a boolean flag indicating if an exponent 'e' or 'E' has been encountered.
 * `next_c` - The next character in the source, used for lookahead (e.g., for 'e+' or 'e-').
 *
 * `bool` - True if the character is valid for the current number literal, false otherwise.
 */
static bool _is_valid_number_char(char c, bool* has_dot, bool* has_e, char next_c) {
    // Digits, hex characters, and suffixes (u, U, l, L)
    if (isalnum(c) || c == '_') { // Alnum covers digits and hex (a-f, A-F)
        return true;
    }

    // Decimal point
    if (c == '.') {
        if (!*has_dot && !*has_e) { // Only one dot allowed, not after exponent
            *has_dot = true;
            return true;
        }
        return false; // Invalid: multiple dots or dot after exponent
    }

    // Exponent (e or E)
    if (c == 'e' || c == 'E') {
        if (!*has_e) { // Only one 'e' or 'E' allowed
            *has_e = true;
            // Optionally handle +/- after e/E
            return true;
        }
        return false; // Invalid: multiple exponents
    }

    // Sign after exponent
    if ((c == '+' || c == '-') && *has_e && (next_c != '\0' && isdigit(next_c))) {
        // This is tricky: we only accept +/- if it immediately follows e/E
        // and is followed by a digit. The main loop will advance past it.
        // This helper just validates 'e', and the main loop handles the +/- after 'e'.
        // For simplicity and to keep this helper focused on *single* char validity,
        // we'll primarily validate the 'e' itself here. The +/- logic
        // is best kept in `read_number` where lookahead is natural.
        return true; // We'll let read_number handle advancing for this.
    }

    return false; // Not a valid number character
}

/*
 * Helper function to read a hexadecimal number literal (after "0x" or "0X").
 *
 * `ctx` - The parser context.
 * `buffer` - The buffer to write the number to.
 * `buffer_size` - The maximum size of the buffer.
 * `current_length` - The current length of the number already in the buffer (should be 2 for "0x").
 *
 * `int` - The total length of the number read, or `current_length` if no valid hex digits follow.
 */
static int _read_hex_number(ParserContext_t* ctx, char* buffer, size_t buffer_size, int current_length) {
    while (ctx->position < ctx->source_length && current_length < (int)(buffer_size - 1)) {
        char c = ctx->source[ctx->position];
        if (isxdigit((unsigned char)c) || c == 'u' || c == 'U' || c == 'l' || c == 'L') {
            buffer[current_length++] = c;
            ctx->position++;
            ctx->column++;
        } else {
            break;
        }
    }
    return current_length;
}

/*
 * Helper function to read the exponent part of a floating-point number (e.g., "e+10", "E-5").
 *
 * `ctx` - The parser context.
 * `buffer` - The buffer to write the number to.
 * `buffer_size` - The maximum size of the buffer.
 * `current_length` - The current length of the number already in the buffer (should include 'e'/'E').
 *
 * `int` - The total length of the number read, or `current_length` if no valid exponent digits follow.
 */
static int _read_float_exponent(ParserContext_t* ctx, char* buffer, size_t buffer_size, int current_length) {
    // Already consumed 'e' or 'E' by the caller
    // Check for optional sign
    if (ctx->position < ctx->source_length && current_length < (int)(buffer_size - 1)) {
        char c = ctx->source[ctx->position];
        if (c == '+' || c == '-') {
            buffer[current_length++] = c;
            ctx->position++;
            ctx->column++;
        }
    }

    // Read exponent digits
    while (ctx->position < ctx->source_length && current_length < (int)(buffer_size - 1)) {
        char c = ctx->source[ctx->position];
        if (isdigit((unsigned char)c)) {
            buffer[current_length++] = c;
            ctx->position++;
            ctx->column++;
        } else {
            break;
        }
    }
    return current_length;
}

/*
 * Helper function to read the decimal or integer part of a number, including a single decimal point.
 * This function will *not* read an 'e' or 'E' exponent.
 *
 * `ctx` - The parser context.
 * `buffer` - The buffer to write the number to.
 * `buffer_size` - The maximum size of the buffer.
 * `current_length` - The current length of the number already in the buffer (e.g., initial sign).
 * `has_dot_out` - Output parameter: set to true if a decimal point was encountered.
 * `has_e_out` - Output parameter: set to true if an 'e' or 'E' was encountered.
 *
 * `int` - The total length of the number read.
 */
static int _read_decimal_integer_or_float_part(ParserContext_t* ctx, char* buffer, size_t buffer_size, int current_length, bool* has_dot_out, bool* has_e_out) {
    *has_dot_out = false;
    *has_e_out = false; // This helper doesn't read 'e', but sets it to false for the caller

    while (ctx->position < ctx->source_length && current_length < (int)(buffer_size - 1)) {
        char c = ctx->source[ctx->position];

        if (isdigit((unsigned char)c) || c == 'u' || c == 'U' || c == 'l' || c == 'L') {
            buffer[current_length++] = c;
            ctx->position++;
            ctx->column++;
        } else if (c == '.' && !*has_dot_out) { // Allow one decimal point
            *has_dot_out = true;
            buffer[current_length++] = c;
            ctx->position++;
            ctx->column++;
        } else if (c == 'e' || c == 'E') { // Found 'e' or 'E', this part is done, let caller handle exponent
            *has_e_out = true;
            break;
        } else {
            break; // Not a number character
        }
    }
    return current_length;
}
/*
 * Read a number literal with hex/octal/float support
 */
static int read_number(ParserContext_t* ctx, char* buffer, size_t buffer_size) {
    int length = 0;
    bool has_dot = false;
    bool has_e = false;

    // Handle optional leading sign for numbers if it's the very first character
    if (ctx->position < ctx->source_length) {
        char initial_c = ctx->source[ctx->position];
        if (initial_c == '+' || initial_c == '-') {
            buffer[length++] = initial_c;
            ctx->position++;
            ctx->column++;
        }
    }

    // Check for hexadecimal prefix
    if (ctx->position + 1 < ctx->source_length &&
        ctx->source[ctx->position] == '0' &&
        (ctx->source[ctx->position + 1] == 'x' || ctx->source[ctx->position + 1] == 'X')) {
        buffer[length++] = ctx->source[ctx->position++]; // '0'
        buffer[length++] = ctx->source[ctx->position++]; // 'x' or 'X'
        ctx->column += 2;
        length = _read_hex_number(ctx, buffer, buffer_size, length);
    } else {
        // Must be a decimal integer or float
        length = _read_decimal_integer_or_float_part(ctx, buffer, buffer_size, length, &has_dot, &has_e);
        
        // If an 'e' or 'E' was encountered, read the exponent part
        if (has_e) {
            buffer[length++] = ctx->source[ctx->position++]; // consume 'e' or 'E'
            ctx->column++;
            length = _read_float_exponent(ctx, buffer, buffer_size, length);
        }
    }

    buffer[length] = '\0';
    return length;
}
/*
 * Helper function to check if two characters form a known two-character operator.
 * If a match is found, copies the operator string to op_buffer.
 *
 * `c1` - The first character.
 * `c2` - The second character.
 * `op_buffer` - Buffer to store the operator string if found (must be at least 3 bytes).
 *
 * `bool` - True if c1c2 is a known two-character operator, false otherwise.
 */
static bool _is_two_char_operator(char c1, char c2, char* op_buffer) {
    if (!op_buffer) return false;

    op_buffer[0] = c1;
    op_buffer[1] = c2;
    op_buffer[2] = '\0';

    for (int i = 0; TWO_CHAR_OPERATORS[i] != NULL; i++) {
        if (strcmp(op_buffer, TWO_CHAR_OPERATORS[i]) == 0) {
            return true;
        }
    }
    op_buffer[0] = '\0'; // Clear buffer if not found
    return false;
}

/*
 * Helper function to check if a character is a known single-character operator.
 *
 * `c` - The character to check.
 *
 * `bool` - True if c is a known single-character operator, false otherwise.
 */
static bool _is_single_char_operator(char c) {
    // This string contains all single-character operators
    return strchr("+-*/%=<>!&|^~?:", c) != NULL;
}
/*
 * Read a multi-character operator (like <=, >=, ==, !=, etc.)
 */
static int read_operator(ParserContext_t* ctx, char* buffer, size_t buffer_size) {
    if (ctx->position >= ctx->source_length || buffer_size < 3) return 0;

    char c1 = ctx->source[ctx->position];
    char c2 = (ctx->position + 1 < ctx->source_length) ? ctx->source[ctx->position + 1] : '\0';

    // Attempt to read a two-character operator first
    if (_is_two_char_operator(c1, c2, buffer)) {
        ctx->position += 2;
        ctx->column += 2;
        return 2;
    }

    // If not a two-character operator, attempt to read a single-character operator
    if (_is_single_char_operator(c1)) {
        buffer[0] = c1;
        buffer[1] = '\0';
        ctx->position++;
        ctx->column++;
        return 1;
    }

    return 0; // Not an operator
}

// =============================================================================
// PUBLIC API IMPLEMENTATION
// =============================================================================

/*
 * Check if a word is a C keyword
 * FIXED: Now also checks for 'inline' keyword
 */
bool c_parser_is_c_keyword(const char* word) {
    if (!word) return false;

    for (int i = 0; C_KEYWORDS[i]; i++) {
        if (strcmp(word, C_KEYWORDS[i]) == 0) {
            return true;
        }
    }
    return false;
}

/*
 * Check if a function name is dangerous and should be replaced
 */
bool c_parser_is_dangerous_function(const char* func_name) {
    if (!func_name) return false;

    for (int i = 0; DANGEROUS_FUNCTIONS[i]; i++) {
        if (strcmp(func_name, DANGEROUS_FUNCTIONS[i]) == 0) {
            return true;
        }
    }
    return false;
}

/*
 * Check if a word is a standard C type keyword or common type identifier
 *
 * `word` - Word to check (must be null-terminated)
 *
 * `bool` - true if word is a type keyword, false otherwise
 *
 * -- Returns false if word is NULL
 * -- Recognizes primitive types (int, char, float, etc.)
 * -- Includes standard library types (size_t, FILE, etc.)
 * -- Covers fixed-width integer types (uint32_t, int64_t, etc.)
 * -- Used for variable declaration detection and type analysis
 */
bool c_parser_is_type_keyword(const char* word) {
    if (!word) return false;

    for (int i = 0; TYPE_KEYWORDS[i]; i++) {
        if (strcmp(word, TYPE_KEYWORDS[i]) == 0) {
            return true;
        }
    }
    return false;
}
/*
 * Helper to tokenize comments (line or block).
 * Returns true if a comment was tokenized, false otherwise.
 */
static bool _tokenize_comment(ParserContext_t* ctx, ParsedFile_t* parsed, char c, char next_c, int start_line, int start_column, char* buffer, size_t buffer_size) {
    if (c == '/' && ctx->position + 1 < ctx->source_length) {
        if (next_c == '/') {
            // Line comment
            int length = 0;
            while (ctx->position < ctx->source_length &&
                   ctx->source[ctx->position] != '\n' &&
                   length < (int)(buffer_size - 1)) {
                buffer[length++] = ctx->source[ctx->position];
                ctx->position++;
                ctx->column++;
            }
            buffer[length] = '\0';
            return add_token(parsed, TOKEN_COMMENT_LINE, buffer, start_line, start_column, length);
        } else if (next_c == '*') {
            // Block comment
            int length = 0;
            buffer[length++] = ctx->source[ctx->position++];  // '/'
            buffer[length++] = ctx->source[ctx->position++];  // '*'
            ctx->column += 2;

            while (ctx->position + 1 < ctx->source_length && length < (int)(buffer_size - 1)) {
                char c1 = ctx->source[ctx->position];
                char c2 = ctx->source[ctx->position + 1];

                buffer[length++] = c1;
                if (c1 == '\n') {
                    ctx->line++;
                    ctx->column = 1;
                } else {
                    ctx->column++;
                }
                ctx->position++;

                if (c1 == '*' && c2 == '/') {
                    buffer[length++] = c2;
                    ctx->position++;
                    ctx->column++;
                    break;
                }
            }
            buffer[length] = '\0';
            return add_token(parsed, TOKEN_COMMENT_BLOCK, buffer, start_line, start_column, length);
        }
    }
    return false;
}

/*
 * Helper to tokenize preprocessor directives.
 * Returns true if a directive was tokenized, false otherwise.
 */
static bool _tokenize_preprocessor(ParserContext_t* ctx, ParsedFile_t* parsed, int start_line, int start_column, char* buffer, size_t buffer_size) {
    if (ctx->source[ctx->position] == '#') {
        int length = 0;
        while (ctx->position < ctx->source_length &&
               ctx->source[ctx->position] != '\n' &&
               length < (int)(buffer_size - 1)) {
            buffer[length++] = ctx->source[ctx->position];
            ctx->position++;
            ctx->column++;
        }
        buffer[length] = '\0';
        return add_token(parsed, TOKEN_PREPROCESSOR, buffer, start_line, start_column, length);
    }
    return false;
}

/*
 * Helper to tokenize string and character literals.
 * Returns true if a literal was tokenized, false otherwise.
 */
static bool _tokenize_string_char_literal(ParserContext_t* ctx, ParsedFile_t* parsed, char c, int start_line, int start_column, char* buffer, size_t buffer_size) {
    if (c == '"' || c == '\'') {
        int length = read_string(ctx, buffer, buffer_size);
        TokenType_t type = (c == '"') ? TOKEN_STRING : TOKEN_CHAR;
        return add_token(parsed, type, buffer, start_line, start_column, length);
    }
    return false;
}

/*
 * Helper to tokenize number literals.
 * Returns true if a number was tokenized, false otherwise.
 */
static bool _tokenize_number(ParserContext_t* ctx, ParsedFile_t* parsed, char c, char next_c, int start_line, int start_column, char* buffer, size_t buffer_size) {
    if (isdigit((unsigned char)c) || (c == '.' && ctx->position + 1 < ctx->source_length && isdigit((unsigned char)next_c))) {
        int length = read_number(ctx, buffer, buffer_size);
        return add_token(parsed, TOKEN_NUMBER, buffer, start_line, start_column, length);
    }
    return false;
}

/*
 * Helper to tokenize identifiers and keywords.
 * Returns true if an identifier or keyword was tokenized, false otherwise.
 */
static bool _tokenize_identifier_keyword(ParserContext_t* ctx, ParsedFile_t* parsed, char c, int start_line, int start_column, char* buffer, size_t buffer_size) {
    if (isalpha((unsigned char)c) || c == '_') {
        int length = read_identifier(ctx, buffer, buffer_size);
        TokenType_t type = c_parser_is_c_keyword(buffer) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
        return add_token(parsed, type, buffer, start_line, start_column, length);
    }
    return false;
}

/*
 * Helper to tokenize operators (single or multi-character).
 * Returns true if an operator was tokenized, false otherwise.
 */
static bool _tokenize_operator(ParserContext_t* ctx, ParsedFile_t* parsed, int start_line, int start_column, char* buffer, size_t buffer_size) {
    int op_length = read_operator(ctx, buffer, buffer_size);
    if (op_length > 0) {
        return add_token(parsed, TOKEN_OPERATOR, buffer, start_line, start_column, op_length);
    }
    return false;
}

/*
 * Helper to tokenize single-character punctuation.
 * Returns true if punctuation was tokenized, false otherwise.
 */
static bool _tokenize_punctuation(ParserContext_t* ctx, ParsedFile_t* parsed, char c, int start_line, int start_column, char* buffer) {
    if (strchr("(){}[];,.", c)) {
        buffer[0] = c;
        buffer[1] = '\0';
        bool success = add_token(parsed, TOKEN_PUNCTUATION, buffer, start_line, start_column, 1);
        ctx->position++;
        ctx->column++;
        return success;
    }
    return false;
}

/*
 * Helper to tokenize newline characters.
 * Returns true if a newline was tokenized, false otherwise.
 */
static bool _tokenize_newline(ParserContext_t* ctx, ParsedFile_t* parsed, int start_line, int start_column) {
    if (ctx->source[ctx->position] == '\n') {
        bool success = add_token(parsed, TOKEN_NEWLINE, "\\n", start_line, start_column, 1);
        ctx->position++;
        ctx->line++;
        ctx->column = 1;
        return success;
    }
    return false;
}

/*
 * Helper to tokenize unknown characters.
 * Returns true if an unknown character was tokenized, false otherwise.
 */
static bool _tokenize_unknown(ParserContext_t* ctx, ParsedFile_t* parsed, char c, int start_line, int start_column, char* buffer) {
    // This should be the last resort if no other token type matches
    buffer[0] = c;
    buffer[1] = '\0';
    bool success = add_token(parsed, TOKEN_UNKNOWN, buffer, start_line, start_column, 1);
    ctx->position++;
    ctx->column++;
    return success;
}
/*
 * Helper function to attempt to tokenize the next character based on its type.
 * It tries various token types in a specific order of precedence.
 *
 * `ctx` - The parser context.
 * `parsed` - The ParsedFile_t structure to add tokens to.
 * `buffer` - The buffer for token values.
 * `buffer_size` - The size of the buffer.
 *
 * `bool` - True if a token was successfully identified and added, false otherwise.
 */
static bool _attempt_tokenize_next_token(ParserContext_t* ctx, ParsedFile_t* parsed, char* buffer, size_t buffer_size) {
    char c = ctx->source[ctx->position];
    char next_c = (ctx->position + 1 < ctx->source_length) ? ctx->source[ctx->position + 1] : '\0';
    int start_line = ctx->line;
    int start_column = ctx->column;

    // Attempt to tokenize various types in order of precedence
    if (_tokenize_comment(ctx, parsed, c, next_c, start_line, start_column, buffer, buffer_size)) {
        return true;
    }
    if (_tokenize_preprocessor(ctx, parsed, start_line, start_column, buffer, buffer_size)) {
        return true;
    }
    if (_tokenize_string_char_literal(ctx, parsed, c, start_line, start_column, buffer, buffer_size)) {
        return true;
    }
    if (_tokenize_number(ctx, parsed, c, next_c, start_line, start_column, buffer, buffer_size)) {
        return true;
    }
    if (_tokenize_identifier_keyword(ctx, parsed, c, start_line, start_column, buffer, buffer_size)) {
        return true;
    }
    if (_tokenize_newline(ctx, parsed, start_line, start_column)) {
        return true;
    }
    if (_tokenize_operator(ctx, parsed, start_line, start_column, buffer, buffer_size)) {
        return true;
    }
    if (_tokenize_punctuation(ctx, parsed, c, start_line, start_column, buffer)) {
        return true;
    }
    
    // If none of the above matched, it's an unknown character
    if (_tokenize_unknown(ctx, parsed, c, start_line, start_column, buffer)) {
        return true;
    }

    return false; // No token was recognized or processed
}
/*
 * Tokenize C source code with divine precision
 */
Token_t* c_parser_tokenize(const char* content, int* token_count) {
    if (!content || !token_count) return NULL;

    ParsedFile_t* temp_parsed = create_parsed_file("temp");
    if (!temp_parsed) return NULL;

    ParserContext_t ctx;
    init_parser_context(&ctx, content);

    char buffer[1024];

    while (ctx.position < ctx.source_length) {
        skip_whitespace(&ctx);

        if (ctx.position >= ctx.source_length) {
            break; // Reached end of source after skipping whitespace
        }

        // Attempt to tokenize the next character
        if (!_attempt_tokenize_next_token(&ctx, temp_parsed, buffer, sizeof(buffer))) {
            // This case should ideally not be reached if _tokenize_unknown always progresses
            // but is here as a safety fallback. It implies an unhandled character
            // that _tokenize_unknown also failed to process (which shouldn't happen).
            // For now, we'll just advance to prevent infinite loops.
            ctx.position++;
            ctx.column++;
        }
    }

    // Transfer tokens to return array
    *token_count = temp_parsed->token_count;
    Token_t* result = temp_parsed->tokens;
    temp_parsed->tokens = NULL;  // Prevent cleanup of tokens by temp_parsed
    c_parser_free_parsed_file(temp_parsed); // Frees temp_parsed but not tokens

    return result;
}

// =============================================================================
// SYNTAX ANALYSIS - DIVINE UNDERSTANDING
// =============================================================================

/*
 * Check if an identifier+parentheses pattern is a function declaration (for headers)
 * FIXED: Now also checks for brace depth to avoid function calls
 */
static bool is_function_declaration(Token_t* tokens, int token_count, int index) {
    if (index < 0 || index >= token_count) return false;

    Token_t* current = &tokens[index];
    if (current->type != TOKEN_IDENTIFIER) return false;

    // Must be followed by '('
    if (index + 1 >= token_count ||
        tokens[index + 1].type != TOKEN_PUNCTUATION ||
        strcmp(tokens[index + 1].value, "(") != 0) {
        return false;
    }

    // Check if we're inside a function body by counting braces
    int brace_depth = 0;
    for (int i = 0; i < index; i++) {
        if (tokens[i].type == TOKEN_PUNCTUATION) {
            if (strcmp(tokens[i].value, "{") == 0) {
                brace_depth++;
            } else if (strcmp(tokens[i].value, "}") == 0) {
                brace_depth--;
            }
        }
    }
    
    // If we're inside braces (brace_depth > 0), this is likely a function call
    if (brace_depth > 0) {
        return false;
    }

    // Look for function declaration pattern (ends with semicolon)
    bool has_return_type = false;
    int paren_depth = 0;

    // Check for return type before function name
    for (int i = index - 1; i >= 0 && i >= index - 10; i--) {
        Token_t* prev = &tokens[i];
        
        if (prev->type == TOKEN_KEYWORD || prev->type == TOKEN_IDENTIFIER) {
            has_return_type = true;
            break;
        } else if (prev->type == TOKEN_NEWLINE) {
            break;
        }
    }

    // Look forward to find semicolon (declaration) vs opening brace (definition)
    for (int i = index + 1; i < token_count && i < index + 100; i++) {
        Token_t* next = &tokens[i];

        if (next->type == TOKEN_PUNCTUATION) {
            if (strcmp(next->value, "(") == 0) {
                paren_depth++;
            } else if (strcmp(next->value, ")") == 0) {
                paren_depth--;
                if (paren_depth == 0) {
                    // Found matching closing paren, look for semicolon
                    for (int j = i + 1; j < token_count && j < i + 10; j++) {
                        if (tokens[j].type == TOKEN_PUNCTUATION && strcmp(tokens[j].value, ";") == 0) {
                            return has_return_type; // It's a declaration
                        } else if (tokens[j].type == TOKEN_PUNCTUATION && strcmp(tokens[j].value, "{") == 0) {
                            return false; // It's a definition
                        }
                        // Skip whitespace/newlines
                        if (tokens[j].type != TOKEN_NEWLINE && tokens[j].type != TOKEN_COMMENT_LINE) {
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }

    return false;
}

/*
 * Check if an identifier+parentheses pattern is a function definition vs call
 * FIXED: Now properly distinguishes function definitions from function calls
 */
static bool is_function_definition(Token_t* tokens, int token_count, int index) {
    if (index < 0 || index >= token_count) return false;

    Token_t* current = &tokens[index];
    if (current->type != TOKEN_IDENTIFIER) return false;

    // Must be followed by '('
    if (index + 1 >= token_count ||
        tokens[index + 1].type != TOKEN_PUNCTUATION ||
        strcmp(tokens[index + 1].value, "(") != 0) {
        return false;
    }

    // Key insight: Function calls are almost always inside braces {}
    // Check if we're inside a function body by counting braces
    int brace_depth = 0;
    for (int i = 0; i < index; i++) {
        if (tokens[i].type == TOKEN_PUNCTUATION) {
            if (strcmp(tokens[i].value, "{") == 0) {
                brace_depth++;
            } else if (strcmp(tokens[i].value, "}") == 0) {
                brace_depth--;
            }
        }
    }
    
    // Debug: uncomment for debugging
    // printf("DEBUG: %s at index %d, brace_depth = %d\n", current->value, index, brace_depth);
    
    // If we're inside braces (brace_depth > 0), this is likely a function call
    if (brace_depth > 0) {
        return false;
    }

    // Look backward for return type (must be on same line or previous line)
    bool has_return_type = false;
    for (int i = index - 1; i >= 0 && i >= index - 10; i--) {
        Token_t* prev = &tokens[i];

        // Stop at certain punctuation that indicates we've gone too far
        if (prev->type == TOKEN_PUNCTUATION) {
            if (strcmp(prev->value, ";") == 0 || strcmp(prev->value, "}") == 0) {
                break;
            }
        }

        // Check for return type keywords
        if (prev->type == TOKEN_KEYWORD) {
            if (strcmp(prev->value, "int") == 0 || strcmp(prev->value, "void") == 0 ||
                strcmp(prev->value, "char") == 0 || strcmp(prev->value, "bool") == 0 ||
                strcmp(prev->value, "float") == 0 || strcmp(prev->value, "double") == 0 ||
                strcmp(prev->value, "static") == 0 || strcmp(prev->value, "inline") == 0) {
                has_return_type = true;
                break;
            }
        } else if (prev->type == TOKEN_IDENTIFIER) {
            // Custom type name (like ParsedFile_t)
            has_return_type = true;
            break;
        }
    }

    // Must have return type to be a function definition
    if (!has_return_type) {
        return false;
    }

    // Look forward for opening brace (function body)
    bool has_opening_brace = false;
    int paren_depth = 0;

    for (int i = index + 1; i < token_count && i < index + 50; i++) {
        Token_t* next = &tokens[i];

        if (next->type == TOKEN_PUNCTUATION) {
            if (strcmp(next->value, "(") == 0) {
                paren_depth++;
            } else if (strcmp(next->value, ")") == 0) {
                paren_depth--;
                if (paren_depth == 0) {
                    // Found matching closing paren, look for opening brace
                    for (int j = i + 1; j < token_count && j < i + 10; j++) {
                        if (tokens[j].type == TOKEN_PUNCTUATION) {
                            if (strcmp(tokens[j].value, "{") == 0) {
                                has_opening_brace = true;
                                break;
                            } else if (strcmp(tokens[j].value, ";") == 0) {
                                // Function declaration, not definition
                                return false;
                            }
                        }
                        // Skip whitespace/newlines/comments
                        if (tokens[j].type != TOKEN_NEWLINE && 
                            tokens[j].type != TOKEN_COMMENT_LINE &&
                            tokens[j].type != TOKEN_COMMENT_BLOCK) {
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }

    // Must have both return type and opening brace to be a function definition
    return has_return_type && has_opening_brace;
}

/*
 * Extract function parameters from tokens
 */
static void extract_function_parameters(Token_t* tokens, int token_count, int func_index, FunctionInfo_t* func) {
    if (!tokens || !func) return;

    // Find the opening parenthesis
    int paren_start = -1;
    for (int i = func_index + 1; i < token_count && i < func_index + 10; i++) {
        if (tokens[i].type == TOKEN_PUNCTUATION && strcmp(tokens[i].value, "(") == 0) {
            paren_start = i;
            break;
        }
    }

    if (paren_start == -1) return;

    // Find the matching closing parenthesis
    int paren_depth = 0;
    int paren_end = -1;
    for (int i = paren_start; i < token_count; i++) {
        if (tokens[i].type == TOKEN_PUNCTUATION) {
            if (strcmp(tokens[i].value, "(") == 0) {
                paren_depth++;
            } else if (strcmp(tokens[i].value, ")") == 0) {
                paren_depth--;
                if (paren_depth == 0) {
                    paren_end = i;
                    break;
                }
            }
        }
    }

    if (paren_end == -1) return;

    // Count parameters by counting commas + 1 (if there's content)
    int param_count = 0;
    bool has_content = false;
    
    for (int i = paren_start + 1; i < paren_end; i++) {
        if (tokens[i].type == TOKEN_IDENTIFIER || tokens[i].type == TOKEN_KEYWORD) {
            has_content = true;
        } else if (tokens[i].type == TOKEN_PUNCTUATION && strcmp(tokens[i].value, ",") == 0) {
            param_count++;
        } else if (tokens[i].type == TOKEN_OPERATOR && strcmp(tokens[i].value, "...") == 0) {
            has_content = true; // Variadic function
        }
    }

    if (has_content) {
        param_count++; // Add 1 for the last parameter (no comma after it)
    }

    // Check for void parameter list
    if (param_count == 1) {
        for (int i = paren_start + 1; i < paren_end; i++) {
            if (tokens[i].type == TOKEN_KEYWORD && strcmp(tokens[i].value, "void") == 0) {
                param_count = 0; // void means no parameters
                break;
            }
        }
    }

    func->param_count = param_count;
    
    // Allocate and extract parameter names (simplified)
    if (param_count > 0) {
        func->parameters = malloc(sizeof(char*) * param_count);
        if (func->parameters) {
            int current_param = 0;
            char param_buffer[256] = {0};
            int buffer_pos = 0;
            
            for (int i = paren_start + 1; i < paren_end && current_param < param_count; i++) {
                if (tokens[i].type == TOKEN_PUNCTUATION && strcmp(tokens[i].value, ",") == 0) {
                    param_buffer[buffer_pos] = '\0';
                    func->parameters[current_param] = strdup(param_buffer);
                    current_param++;
                    buffer_pos = 0;
                    memset(param_buffer, 0, sizeof(param_buffer));
                } else if (tokens[i].type != TOKEN_NEWLINE) {
                    if (buffer_pos < sizeof(param_buffer) - 2) {
                        if (buffer_pos > 0) param_buffer[buffer_pos++] = ' ';
                        strncpy(param_buffer + buffer_pos, tokens[i].value, sizeof(param_buffer) - buffer_pos - 1);
                        buffer_pos += strlen(tokens[i].value);
                    }
                }
            }
            
            // Add the last parameter
            if (current_param < param_count && buffer_pos > 0) {
                param_buffer[buffer_pos] = '\0';
                func->parameters[current_param] = strdup(param_buffer);
            }
        }
    }
}

/*
 * Extract return type from tokens before function definition
 */
static char* extract_return_type(Token_t* tokens, int token_count, int func_index) {
    char return_type[256] = {0};
    int type_parts = 0;

    // Look backwards for return type components, but stop at function boundaries
    for (int i = func_index - 1; i >= 0 && i >= func_index - 5; i--) {
        Token_t* token = &tokens[i];

        // Stop at punctuation that indicates end of previous function/statement
        if (token->type == TOKEN_PUNCTUATION) {
            if (strcmp(token->value, ";") == 0 || strcmp(token->value, "}") == 0 || 
                strcmp(token->value, ")") == 0) {
                break; // Hit end of previous function or statement
            } else if (strcmp(token->value, "*") == 0) {
                // Handle pointer indicators
                char temp[256];
                snprintf(temp, sizeof(temp), "%s*", return_type);
                strncpy(return_type, temp, sizeof(return_type) - 1);
            }
        } else if (token->type == TOKEN_KEYWORD || token->type == TOKEN_IDENTIFIER) {
            // Only accept type keywords, not random identifiers
            if (token->type == TOKEN_KEYWORD || 
                (token->type == TOKEN_IDENTIFIER && strstr(token->value, "_t"))) {
                // Build return type string (in reverse order, so we'll fix it)
                if (type_parts == 0) {
                    strncpy(return_type, token->value, sizeof(return_type) - 1);
                } else {
                    char temp[256];
                    snprintf(temp, sizeof(temp), "%s %s", token->value, return_type);
                    strncpy(return_type, temp, sizeof(return_type) - 1);
                }
                type_parts++;

                // Stop after getting one main type part for simplicity
                if (type_parts >= 1) break;
            }
        } else if (token->type == TOKEN_NEWLINE || token->type == TOKEN_COMMENT_LINE) {
            break;  // Stop at line boundaries
        } else if (token->type == TOKEN_COMMENT_BLOCK) {
            // Skip comment blocks, don't stop the search
            continue;
        }
    }

    return strdup(strlen(return_type) > 0 ? return_type : "unknown");
}

/*
 * Parse C source code into structured format with divine understanding
 */
ParsedFile_t* c_parser_parse_content(const char* content, const char* file_path) {
    if (!content) return NULL;

    ParsedFile_t* parsed = create_parsed_file(file_path);
    if (!parsed) return NULL;

    // First tokenize the content
    int token_count;
    Token_t* tokens = c_parser_tokenize(content, &token_count);
    if (!tokens) {
        c_parser_free_parsed_file(parsed);
        return NULL;
    }

    // Copy tokens to parsed structure
    parsed->tokens = tokens;
    parsed->token_count = token_count;
    parsed->token_capacity = token_count;  // Exact fit

    // Parse high-level structures
    for (int i = 0; i < token_count; i++) {
        Token_t* token = &tokens[i];

        // Parse include directives
        if (token->type == TOKEN_PREPROCESSOR && strstr(token->value, "#include")) {
            char* include_start = strchr(token->value, '<');
            if (!include_start) include_start = strchr(token->value, '"');
            if (include_start) {
                char include_name[256];
                char end_char = (*include_start == '<') ? '>' : '"';
                include_start++;  // Skip opening bracket/quote

                int j = 0;
                while (*include_start && *include_start != end_char && j < 255) {
                    include_name[j++] = *include_start++;
                }
                include_name[j] = '\0';

                if (strlen(include_name) > 0) {
                    add_include(parsed, include_name);
                }
            }
        }

        // Parse function definitions and declarations
        if (token->type == TOKEN_IDENTIFIER && i + 1 < token_count) {
            Token_t* next = &tokens[i + 1];
            if (next->type == TOKEN_PUNCTUATION && strcmp(next->value, "(") == 0) {
                bool is_definition = is_function_definition(tokens, token_count, i);
                bool is_declaration = is_function_declaration(tokens, token_count, i);
                
                if (is_definition || is_declaration) {
                    char* return_type = extract_return_type(tokens, token_count, i);
                    bool is_static = false;
                    bool is_inline = false;

                    // Check for static/inline modifiers
                    for (int j = i - 1; j >= 0 && j >= i - 5; j--) {
                        if (tokens[j].type == TOKEN_KEYWORD) {
                            if (strcmp(tokens[j].value, "static") == 0) is_static = true;
                            if (strcmp(tokens[j].value, "inline") == 0) is_inline = true;
                        }
                    }

                    add_function(parsed, token->value, return_type,
                               token->line, token->column, is_static, is_inline);
                    
                    // Extract parameters for the just-added function
                    if (parsed->function_count > 0) {
                        FunctionInfo_t* func = &parsed->functions[parsed->function_count - 1];
                        extract_function_parameters(tokens, token_count, i, func);
                    }
                    
                    // Immediately check for documentation after adding the function
                    c_parser_has_documentation_for_function(parsed, token->value);
                    
                    free(return_type);
                }
            }
        }
    }

    return parsed;
}

/*
 * Parse a C file from disk with divine file handling
 */
ParsedFile_t* c_parser_parse_file(const char* file_path) {
    if (!file_path) return NULL;

    FILE* file = fopen(file_path, "r");
    if (!file) return NULL;

    // Get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (file_size <= 0) {
        fclose(file);
        return NULL;
    }

    // Read content with divine blessing
    char* content = malloc(file_size + 1);
    if (!content) {
        fclose(file);
        return NULL;
    }

    size_t bytes_read = fread(content, 1, file_size, file);
    content[bytes_read] = '\0';
    fclose(file);

    // Parse content
    ParsedFile_t* result = c_parser_parse_content(content, file_path);
    free(content);

    return result;
}

// =============================================================================
// MEMORY MANAGEMENT - DIVINE CLEANUP
// =============================================================================

/*
 * Free tokens array with compassionate cleanup
 */
void c_parser_free_tokens(Token_t* tokens, int count) {
    if (!tokens) return;

    for (int i = 0; i < count; i++) {
        free(tokens[i].value);
    }
    free(tokens);
}

/*
 * Free parsed file structure with complete divine cleanup
 */
void c_parser_free_parsed_file(ParsedFile_t* parsed) {
    if (!parsed) return;

    free(parsed->file_path);

    // Free tokens
    if (parsed->tokens) {
        c_parser_free_tokens(parsed->tokens, parsed->token_count);
    }

    // Free functions
    if (parsed->functions) {
        for (int i = 0; i < parsed->function_count; i++) {
            free(parsed->functions[i].name);
            free(parsed->functions[i].return_type);
            free(parsed->functions[i].documentation);
            if (parsed->functions[i].parameters) {
                for (int j = 0; j < parsed->functions[i].param_count; j++) {
                    free(parsed->functions[i].parameters[j]);
                }
                free(parsed->functions[i].parameters);
            }
        }
        free(parsed->functions);
    }

    // Free includes
    if (parsed->includes) {
        for (int i = 0; i < parsed->include_count; i++) {
            free(parsed->includes[i]);
        }
        free(parsed->includes);
    }

    free(parsed);
}

// =============================================================================
// QUERY INTERFACE - DIVINE WISDOM ACCESS
// =============================================================================

/*
 * Find function information at a specific line
 */
FunctionInfo_t* c_parser_find_function_at_line(ParsedFile_t* parsed, int line) {
    if (!parsed) return NULL;

    for (int i = 0; i < parsed->function_count; i++) {
        if (parsed->functions[i].line_number == line) {
            return &parsed->functions[i];
        }
    }
    return NULL;
}

/*
 * Check if a function has documentation
 */
bool c_parser_has_documentation_for_function(ParsedFile_t* parsed, const char* func_name) {
    if (!parsed || !func_name) return false;

    // Find the function first
    FunctionInfo_t* func = NULL;
    for (int i = 0; i < parsed->function_count; i++) {
        if (strcmp(parsed->functions[i].name, func_name) == 0) {
            func = &parsed->functions[i];
            break;
        }
    }

    if (!func) return false;

    // Check if function already has documentation marked
    if (func->has_documentation) return true;

    // Look for block comment tokens before the function line (only block comments count as documentation)
    int func_line = func->line_number;
    Token_t* nearest_comment = NULL;
    int nearest_distance = INT_MAX;
    
    // Find the nearest block comment before the function within 20 lines (increased range for large docs)
    for (int i = 0; i < parsed->token_count; i++) {
        Token_t* token = &parsed->tokens[i];

        // Look for block comments within 20 lines before the function
        if (token->line < func_line && token->line >= func_line - 20) {
            if (token->type == TOKEN_COMMENT_BLOCK) {
                int distance = func_line - token->line;
                if (distance < nearest_distance) {
                    nearest_comment = token;
                    nearest_distance = distance;
                }
            }
        }
    }
    
    
    // If we found a comment, check if there are any significant code elements between it and the function
    if (nearest_comment) {
        bool has_code_between = false;
        int comment_start_line = nearest_comment->line;
        int comment_end_line = comment_start_line;
        
        // Count lines in the comment to find where it ends
        char* comment_text = nearest_comment->value;
        if (comment_text) {
            // Count newlines in the comment text to determine how many lines it spans
            int newline_count = 0;
            for (char* p = comment_text; *p; p++) {
                if (*p == '\n') newline_count++;
            }
            // The comment ends at its start line plus the number of newlines
            comment_end_line = comment_start_line + newline_count;
        }
        
        // Check for any significant code tokens between comment end and function
        for (int i = 0; i < parsed->token_count; i++) {
            Token_t* token = &parsed->tokens[i];
            if (token->line > comment_end_line && token->line < func_line) {
                if (token->type != TOKEN_NEWLINE && 
                    token->type != TOKEN_COMMENT_LINE && 
                    token->type != TOKEN_COMMENT_BLOCK &&
                    token->type != TOKEN_PREPROCESSOR) {
                    // Found significant code between comment and function
                    has_code_between = true;
                    break;
                }
            }
        }
        
        // Accept if no code between comment and function, and they're reasonably close
        int distance = func_line - comment_end_line;
        
        
        if (!has_code_between && distance <= 3) {
            func->has_documentation = true;
            func->documentation = strdup(comment_text ? comment_text : "");
            return true;
        }
    }

    return false;
}

/*
 * Check if a specific line contains a pattern
 */
bool c_parser_line_has_pattern(ParsedFile_t* parsed, int line, const char* pattern) {
    if (!parsed || !pattern) return false;

    // Look through tokens on the specified line
    for (int i = 0; i < parsed->token_count; i++) {
        Token_t* token = &parsed->tokens[i];

        if (token->line == line && token->value && strstr(token->value, pattern)) {
            return true;
        }
    }

    return false;
}

/*
 * Get token type name for debugging and divine understanding
 */
const char* c_parser_token_type_name(TokenType_t type) {
    switch (type) {
        case TOKEN_UNKNOWN: return "UNKNOWN";
        case TOKEN_KEYWORD: return "KEYWORD";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_STRING: return "STRING";
        case TOKEN_CHAR: return "CHAR";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_PUNCTUATION: return "PUNCTUATION";
        case TOKEN_COMMENT_LINE: return "COMMENT_LINE";
        case TOKEN_COMMENT_BLOCK: return "COMMENT_BLOCK";
        case TOKEN_PREPROCESSOR: return "PREPROCESSOR";
        case TOKEN_EOF: return "EOF";
        case TOKEN_NEWLINE: return "NEWLINE";
        default: return "INVALID";
    }
}

// =============================================================================
// DIVINE DOCUMENTATION ANALYSIS
// =============================================================================

/*
 * Check if file has proper first line with filename
 */
bool c_parser_has_proper_filename_header(ParsedFile_t* parsed, const char* expected_filename) {
    if (!parsed || !expected_filename) return false;

    // Look for first comment token
    for (int i = 0; i < parsed->token_count; i++) {
        Token_t* token = &parsed->tokens[i];

        if (token->type == TOKEN_COMMENT_BLOCK || token->type == TOKEN_COMMENT_LINE) {
            // Check if it contains the expected filename
            if (strstr(token->value, expected_filename)) {
                return true;
            }
            // If we found a comment but it doesn't have filename, it's wrong
            return false;
        }

        // Skip whitespace and preprocessor
        if (token->type != TOKEN_NEWLINE && token->type != TOKEN_PREPROCESSOR) {
            // Found non-comment, non-whitespace content first
            return false;
        }
    }

    return false;  // No comments found at all
}

/*
 * Check if file has proper second line wisdom comment
 */
bool c_parser_has_proper_wisdom_header(ParsedFile_t* parsed) {
    if (!parsed) return false;

    int comment_count = 0;

    // Look for the second comment in the file
    for (int i = 0; i < parsed->token_count; i++) {
        Token_t* token = &parsed->tokens[i];

        if (token->type == TOKEN_COMMENT_BLOCK || token->type == TOKEN_COMMENT_LINE) {
            comment_count++;

            if (comment_count == 2) {
                // Check if it contains wisdom markers
                if (strstr(token->value, "INSERT WISDOM HERE") ||
                    strstr(token->value, "Fragment #") ||
                    strstr(token->value, "Metis Fragment")) {
                    return true;
                }
                // Found second comment but it's not wisdom
                return false;
            }
        }

        // If we encounter significant code before finding second comment, fail
        if (comment_count == 1 &&
            (token->type == TOKEN_KEYWORD || token->type == TOKEN_IDENTIFIER) &&
            token->line > 5) {
            return false;
        }
    }

    return false;  // Didn't find second comment
}

/*
 * Get the filename from file path for header checking
 */
const char* c_parser_extract_filename(const char* file_path) {
    if (!file_path) return NULL;

    const char* filename = strrchr(file_path, '/');
    if (!filename) filename = strrchr(file_path, '\\');

    return filename ? filename + 1 : file_path;
}

/*
 * Check if file has both proper filename and wisdom headers
 */
bool c_parser_has_proper_file_headers(ParsedFile_t* parsed) {
    if (!parsed) return false;

    const char* filename = c_parser_extract_filename(parsed->file_path);
    if (!filename) return false;

    return c_parser_has_proper_filename_header(parsed, filename) &&
           c_parser_has_proper_wisdom_header(parsed);
}

/*
 * Analyze function complexity with divine mathematical precision
 */
ComplexityAnalysis_t c_parser_analyze_function_complexity(ParsedFile_t* parsed, const char* func_name) {
    ComplexityAnalysis_t analysis = {0};

    if (!parsed || !func_name) return analysis;

    // Find the function
    FunctionInfo_t* func = NULL;
    for (int i = 0; i < parsed->function_count; i++) { // for every function that we have found
        if (strcmp(parsed->functions[i].name, func_name) == 0) {
            func = &parsed->functions[i]; // set the function pointer
            break;
        }
    }

    if (!func) return analysis;

    int func_start = func->line_number;
    int func_end = func_start;
    int brace_count = 0;
    int current_nesting = 0;
    int max_nesting = 0;
    int return_count = 0;
    int branch_count = 0;
    bool found_start = false;

    // Find function boundaries and analyze complexity
    for (int i = 0; i < parsed->token_count; i++) {
        Token_t* token = &parsed->tokens[i];

        if (token->line < func_start) continue;

        if (!found_start && token->type == TOKEN_PUNCTUATION && strcmp(token->value, "{") == 0) {
            found_start = true;
            brace_count = 1;
            current_nesting = 1;
            continue;
        }

        if (!found_start) continue;

        // Track braces for nesting and function end
        if (token->type == TOKEN_PUNCTUATION) {
            if (strcmp(token->value, "{") == 0) {
                brace_count++;
                current_nesting++;
                if (current_nesting > max_nesting) {
                    max_nesting = current_nesting;
                }
            } else if (strcmp(token->value, "}") == 0) {
                brace_count--;
                current_nesting--;
                if (brace_count == 0) {
                    func_end = token->line;
                    break;
                }
            }
        }

        // Count complexity-increasing constructs
        if (token->type == TOKEN_KEYWORD) {
            if (strcmp(token->value, "if") == 0 ||
                strcmp(token->value, "while") == 0 ||
                strcmp(token->value, "for") == 0 ||
                strcmp(token->value, "switch") == 0 ||
                strcmp(token->value, "case") == 0) {
                branch_count++;
                analysis.complexity_score++;
            } else if (strcmp(token->value, "return") == 0) {
                return_count++;
            }
        }

        // Check for nested operators that increase complexity
        if (token->type == TOKEN_OPERATOR) {
            if (strcmp(token->value, "&&") == 0 || strcmp(token->value, "||") == 0) {
                analysis.complexity_score++;
            }
        }
    }

    analysis.nesting_depth = max_nesting;
    analysis.function_length = func_end - func_start + 1;
    analysis.has_multiple_returns = (return_count > 1);
    analysis.has_deep_nesting = (max_nesting > 3);
    analysis.branch_count = branch_count;

    return analysis;
}

/*
 * Check if a function's header documentation follows proper 3-line format
 */
bool c_parser_has_proper_header_doc_format(ParsedFile_t* parsed, const char* func_name) {
    if (!parsed || !func_name) return false;

    // Find the function first
    FunctionInfo_t* func = NULL;
    for (int i = 0; i < parsed->function_count; i++) {
        if (strcmp(parsed->functions[i].name, func_name) == 0) {
            func = &parsed->functions[i];
            break;
        }
    }
    if (!func) return false;

    // Look for a block comment token right before the function
    for (int i = 0; i < parsed->token_count; i++) {
        Token_t* token = &parsed->tokens[i];

        if (token->line >= func->line_number - 20 && token->line < func->line_number) {
            if (token->type == TOKEN_COMMENT_BLOCK) {
                char* comment_copy = strdup(token->value);
                if (!comment_copy) return false;

                int total_lines = 0;
                int description_lines = 0;
                bool has_inappropriate_content = false;
                bool found_description = false;
                bool found_blank_line = false;

                char* saveptr;
                char* line = strtok_r(comment_copy, "\n", &saveptr);

                while (line != NULL) {
                    total_lines++;
                    
                    // Trim leading whitespace and '*' from the line
                    char* trimmed = line;
                    while (*trimmed && (isspace((unsigned char)*trimmed) || *trimmed == '*')) {
                        trimmed++;
                    }

                    // Check for inappropriate content
                    if (strstr(trimmed, "piss") || strstr(trimmed, "FIXED:") || strstr(trimmed, "TODO:")) {
                        has_inappropriate_content = true;
                    }

                    // Skip opening/closing markers
                    if (strstr(line, "/*") || strstr(line, "*/")) {
                        line = strtok_r(NULL, "\n", &saveptr);
                        continue;
                    }

                    // Count only the FIRST content line as description
                    if (strlen(trimmed) > 0) {
                        if (!found_description) {
                            description_lines = 1;
                            found_description = true;
                        }
                    } else if (found_description && !found_blank_line) {
                        // This is the blank line after description
                        found_blank_line = true;
                    }
                    
                    line = strtok_r(NULL, "\n", &saveptr);
                }
                
                free(comment_copy);

                // Enforce strict header format: exactly 1 description line, followed by blank line, then parameters
                // Must have: found_description=true, found_blank_line=true, description_lines=1
                bool is_valid_format = false;
                if (found_description && description_lines >= 1 && found_blank_line) {
                    is_valid_format = true; // Proper header format: one description + blank + details
                }

                // A 4-line comment is always invalid
                if (total_lines == 4) {
                    is_valid_format = false;
                }

                return (is_valid_format && !has_inappropriate_content);
            }
        }
    }

    return true; // No documentation found, so no format to violate
}

/*
 * Extract the one-line description from a function's documentation
 */
char* c_parser_extract_function_description(ParsedFile_t* parsed, const char* func_name) {
    if (!parsed || !func_name) return NULL;

    // Find the function first
    FunctionInfo_t* func = NULL;
    for (int i = 0; i < parsed->function_count; i++) {
        if (strcmp(parsed->functions[i].name, func_name) == 0) {
            func = &parsed->functions[i];
            break;
        }
    }
    if (!func) return NULL;

    // Look for a block comment token before the function line
    for (int i = 0; i < parsed->token_count; i++) {
        Token_t* token = &parsed->tokens[i];

        if (token->line >= func->line_number - 10 && token->line < func->line_number) {
            if (token->type == TOKEN_COMMENT_BLOCK) {
                char* comment_copy = strdup(token->value);
                if (!comment_copy) return NULL;

                char* description = NULL;
                char* saveptr;
                char* line = strtok_r(comment_copy, "\n", &saveptr);

                while (line != NULL) {
                    char* trimmed = line;
                    while (*trimmed && (isspace((unsigned char)*trimmed) || *trimmed == '*')) {
                        trimmed++;
                    }

                    // Trim trailing whitespace as well for a clean description
                    size_t len = strlen(trimmed);
                    while (len > 0 && isspace((unsigned char)trimmed[len - 1])) {
                        trimmed[--len] = '\0';
                    }

                    if (strlen(trimmed) > 0 && !strstr(trimmed, "/*") && !strstr(trimmed, "*/")) {
                        description = strdup(trimmed);
                        break; // Found the first line of content, we're done.
                    }
                    line = strtok_r(NULL, "\n", &saveptr);
                }

                free(comment_copy);
                return description;
            }
        }
    }
    return NULL;
}

/*
 * Check if implementation function comment matches header description exactly
 */
bool c_parser_implementation_matches_header(ParsedFile_t* parsed, const char* func_name, const char* expected_description) {
    if (!parsed || !func_name || !expected_description) return false;

    char* actual_desc = c_parser_extract_function_description(parsed, func_name);
    if (!actual_desc) return false;

    bool matches = (strcmp(actual_desc, expected_description) == 0);
    free(actual_desc);
    return matches;
}

/*
 * debug_parser.c - Debug program to examine token parsing
 */
// INSERT WISDOM HERE

#include "include/c_parser.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("=== DEBUGGING TOKEN PARSING ===\n");
    
    ParsedFile_t* parsed = c_parser_parse_file("include/c_parser.h");
    if (!parsed) {
        printf("Failed to parse file\n");
        return 1;
    }
    
    printf("Total tokens: %d\n", parsed->token_count);
    printf("Total functions: %d\n", parsed->function_count);
    
    // Find the c_parser_has_documentation_for_function function
    FunctionInfo_t* target_func = NULL;
    for (int i = 0; i < parsed->function_count; i++) {
        if (strcmp(parsed->functions[i].name, "c_parser_has_documentation_for_function") == 0) {
            target_func = &parsed->functions[i];
            break;
        }
    }
    
    if (!target_func) {
        printf("Target function not found!\n");
        c_parser_free_parsed_file(parsed);
        return 1;
    }
    
    printf("\nTarget function found at line %d\n", target_func->line_number);
    printf("Has documentation flag: %s\n", target_func->has_documentation ? "true" : "false");
    
    // Look for block comment tokens around the function
    printf("\nLooking for comment tokens around line %d:\n", target_func->line_number);
    
    for (int i = 0; i < parsed->token_count; i++) {
        Token_t* token = &parsed->tokens[i];
        
        // Show tokens within 20 lines of the function
        if (token->line >= target_func->line_number - 20 && 
            token->line <= target_func->line_number + 5) {
            
            if (token->type == TOKEN_COMMENT_BLOCK || token->type == TOKEN_COMMENT_LINE) {
                printf("  Line %d: %s (type: %s)\n", 
                       token->line, 
                       token->value ? token->value : "(null)",
                       token->type == TOKEN_COMMENT_BLOCK ? "BLOCK" : "LINE");
            }
        }
    }
    
    // Test the documentation detection function
    printf("\nTesting c_parser_has_documentation_for_function...\n");
    bool has_docs = c_parser_has_documentation_for_function(parsed, "c_parser_has_documentation_for_function");
    printf("Result: %s\n", has_docs ? "HAS DOCUMENTATION" : "NO DOCUMENTATION");
    
    // Manual distance calculation to debug
    printf("\nManual distance calculation:\n");
    for (int i = 0; i < parsed->token_count; i++) {
        Token_t* token = &parsed->tokens[i];
        if (token->type == TOKEN_COMMENT_BLOCK && token->line == 281) {
            int newline_count = 0;
            if (token->value) {
                for (char* p = token->value; *p; p++) {
                    if (*p == '\n') newline_count++;
                }
            }
            int comment_end_line = token->line + newline_count;
            int distance = target_func->line_number - comment_end_line;
            printf("  Comment starts: line %d\n", token->line);
            printf("  Comment ends: line %d (newlines: %d)\n", comment_end_line, newline_count);
            printf("  Function at: line %d\n", target_func->line_number);
            printf("  Distance: %d\n", distance);
            printf("  Distance <= 3? %s\n", distance <= 3 ? "YES" : "NO");
        }
    }
    
    c_parser_free_parsed_file(parsed);
    return 0;
}
/*
 * debug_format.c - Debug header format checking
 */
// INSERT WISDOM HERE

#include "include/c_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    printf("=== DEBUGGING FORMAT CHECKING ===\n");
    
    ParsedFile_t* parsed = c_parser_parse_file("include/c_parser.h");
    if (!parsed) {
        printf("Failed to parse file\n");
        return 1;
    }
    
    printf("Testing c_parser_has_proper_header_doc_format...\n");
    bool proper_format = c_parser_has_proper_header_doc_format(parsed, "c_parser_has_documentation_for_function");
    printf("Result: %s\n", proper_format ? "PROPER FORMAT" : "IMPROPER FORMAT");
    
    if (!proper_format) {
        printf("This should trigger a violation!\n");
    }
    
    c_parser_free_parsed_file(parsed);
    return 0;
}
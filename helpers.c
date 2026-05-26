#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "helpers.h"

char* str_append(char* dest, char* source, unsigned int* dest_max_len, unsigned int* dest_len, unsigned int source_len) {
    if (source_len + *dest_len >= *dest_max_len) {
        dest = (char*) realloc(dest, *dest_max_len*2);
        *dest_max_len *= 2;
    }
    *dest_len = *dest_len + source_len;
    return strncat(dest, source, *dest_max_len);
}

char* str_set(char* dest, char* source, unsigned int* dest_max_len, unsigned int* dest_len, unsigned int source_len) {
    if (source_len + *dest_len >= *dest_max_len) {
        dest = (char*) realloc(dest, *dest_max_len*2);
        *dest_max_len *= 2;
        printf("reallocating...\n");
    }
    *dest_len = *dest_len + source_len;
    return strncpy(dest, source, *dest_max_len);
}

char* str_set_line(char* dest, char* source, unsigned int line_num, unsigned int* dest_max_len, unsigned int* dest_len, unsigned int source_len) {
    char *char_ptr = dest;
    unsigned int lines_passed = 0;
    //moving char_ptr to the correct spot
    while ((lines_passed < line_num) && (char_ptr < dest + *dest_max_len)) {
        if (*char_ptr == '\n') lines_passed++;
        char_ptr++;
    }
    
    //setting the destination pointer to the source, effectively overwritting that section
    return str_set(char_ptr, source, dest_max_len, dest_len, source_len);
}

char* str_inv_set_line(char* dest, char* source, unsigned int line_num, unsigned int* dest_max_len, unsigned int* dest_len, unsigned int source_len) {
    char *char_ptr = dest + *dest_max_len;
    unsigned int lines_passed = 0;
    unsigned int iterations = 0;
    //moving char_ptr to the correct spot
    while ((lines_passed < line_num) && (char_ptr > dest)) {
        if (*char_ptr == '\n') lines_passed++;
        char_ptr--;
        iterations++;
    }

    //adjusting pointer to right after the new line character
    char_ptr+=2;

    //setting the destination pointer to the source, effectively overwritting that section
    return str_set(char_ptr, source, &iterations, dest_len, source_len);
}
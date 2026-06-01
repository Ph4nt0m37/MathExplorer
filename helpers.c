#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <float.h>

#include "helpers.h"

char* str_append(char* dest, char* source, unsigned int* dest_max_len, unsigned int* dest_len, unsigned int source_len) {
    if (source_len + *dest_len >= *dest_max_len) {
        dest = (char*) realloc(dest, *dest_max_len*2);
        *dest_max_len *= 2;
    }
    *dest_len += source_len;
    return strncat(dest, source, *dest_max_len);
}

char* str_set(char* dest, char* point1, char* source, unsigned int* dest_max_len, unsigned int* dest_len, unsigned int source_len) {
    if (point1 + source_len >= dest + *dest_max_len) {
        dest = (char*) realloc(dest, *dest_max_len*2);
        *dest_max_len *= 2;
    }
    *dest_len = *dest_len + source_len;
    strncpy(point1, source, source_len);
    return dest;
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
    return str_set(dest, char_ptr, source, dest_max_len, dest_len, source_len);
}

char* str_inv_set_line(char* dest, char* source, unsigned int line_num, unsigned int* dest_max_len, unsigned int* dest_len, unsigned int source_len) {
    char *char_ptr = dest + *dest_len;
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
    char *new_str = str_set(dest, char_ptr, source, dest_max_len, dest_len, source_len);
    //*dest_len += iterations;
    return new_str;
}

//https://stackoverflow.com/questions/2353211/hsl-to-rgb-color-conversion
void HSLtoRGB(const float hsl[], float rgb[])
{
    if(hsl[1] < FLT_MIN)
        rgb[0] = rgb[1] = rgb[2] = hsl[2];
    else if(hsl[2] < FLT_MIN)
        rgb[0] = rgb[1] = rgb[2] = 0.0f;
    else
    {
        const float q = hsl[2] < 0.5f ? hsl[2] * (1.0f + hsl[1]) : hsl[2] + hsl[1] - hsl[2] * hsl[1];
        const float p = 2.0f * hsl[2] - q;
        float t[] = {hsl[0] + 2.0f, hsl[0], hsl[0] - 2.0f};

        for(int i=0; i<3; ++i)
        {
            if(t[i] < 0.0f)
                t[i] += 6.0f;
            else if(t[i] > 6.0f)
                t[i] -= 6.0f;

            if(t[i] < 1.0f)
                rgb[i] = p + (q - p) * t[i];
            else if(t[i] < 3.0f)
                rgb[i] = q;
            else if(t[i] < 4.0f)
                rgb[i] = p + (q - p) * (4.0f - t[i]);
            else
                rgb[i] = p;
          }
      }
}
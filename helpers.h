#ifndef HELPERS_HEADER
#define HELPERS_HEADER

char* str_append(char* dest, char* source, unsigned int* dest_max_len, unsigned int* dest_len, unsigned int source_len);
char* str_set(char* dest, char* source, unsigned int* dest_max_len, unsigned int* dest_len, unsigned int source_len);
char* str_set_line(char* dest, char* source, unsigned int line_num, unsigned int* dest_max_len, unsigned int* dest_len, unsigned int source_len);
char* str_inv_set_line(char* dest, char* source, unsigned int line_num, unsigned int* dest_max_len, unsigned int* dest_len, unsigned int source_len);

#endif
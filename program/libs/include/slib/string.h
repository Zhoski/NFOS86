#ifndef STRING
#define STRING

int strlen(const char s[]);
int strcmp(const char *s1, const char *s2);
void itos(int num, char* out);
void strcat(char* s1, char* s2);
void strcpy(char* dest, char* src);

#endif
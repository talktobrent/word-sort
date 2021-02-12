#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define WORD_SORT_REVERSE       0x1
#define WORD_SORT_IGNORE_CASE   0x2

/* linked list of words */
struct word
{
    const char *first; // pointer to beginning of a word in a string
    struct word *next; // next linked word
};

unsigned int word_sort( const char *src, char *dst, unsigned int dst_len, unsigned int flags );
struct word * word_gets(const char *src, unsigned int dst_len);
unsigned int word_place(struct word *head, char *dst, unsigned int dst_len);
struct word * word_list(struct word *head, const char *first, unsigned int case_insensitive);
unsigned int word_compare(const char *new, const char *current, unsigned int case_insensitive);
void free_list(struct word *head);
struct word * reverse_list(struct word *head, struct word *prev);


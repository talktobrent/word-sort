/* NAME
        word_sort

SYNOPSIS
        #include "word_sort.h"

        unsigned int word_sort( const char *src, char *dst, unsigned int dst_len, unsigned int flags );

DESCRIPTION
        The word_sort() function sorts a whitespace separated list of words in the string src into
        alphabetical order and stores them separated by a single space in the provided output buffer dst.
        Words are read from src until a '\0' is encountered.  A '\0' appended to dst.  The dst_len parameter
        is the size of the provided output buffer dst in chars.  Note that the required output buffer
        length may be less than strlen(src) + 1, if src contains adjacent white spaces.  If src contains
        only whitespace the output buffer will contain a single '\0'.

        The flags parameter maybe contain the following:

        WORD_SORT_REVERSE
                Sort in reverse alphabetical order.

        WORD_SORT_IGNORE_CASE
                Case insensitive sort.

RETURN VALUE
       The function returns the number of characters written to dst including the trailing '\0'.
       If there is insuffient space in dst then the function returns 0.

THREAD SAFETY
       The function is thread safe.

EXAMPLES
        src                       flags                             dst             return value
        ========================================================================================
        "The   \t\n cat  sat"     0                                 "The cat sat"   12 
        "The   \t\n cat  sat"     WORD_SORT_IGNORE_CASE             "cat sat The"   12
        "The   \t\n cat  sat"     WORD_SORT_REVERSE                 "sat cat The"   12

 */
#include <stdlib.h>
#include "word_sort.h"

/* compares the ascii codes of a word to determine larger word */
unsigned int word_compare(const char *new, const char *current, unsigned int case_insensitive)
{
    int new_val, current_val;

    if (case_insensitive)
    {
        new_val = tolower(*new);
        current_val = tolower(*current);
    }
    else
    {
        new_val = *new;
        current_val = *current;
    }
    if (new_val < 33)
        return 0;
    if (new_val == current_val)
        return word_compare(new + 1, current + 1, case_insensitive);
    if (new_val > current_val)
        return 1;
    return 0;
}

/* Creates a linked lists of words found in string */
struct word * word_list(struct word *head, const char *first, unsigned int case_insensitive)
{
    struct word *new_word, *runner;
    new_word = malloc(sizeof(struct word));
    new_word->first = first;
    new_word->next = NULL;
    if (head)
    {
        if (!word_compare(first, head->first, case_insensitive))
        {
            new_word->next = head;
            return new_word;
        }
        runner = head;
        while (runner->next && word_compare(first, runner->next->first, case_insensitive))
            runner = runner->next;
        new_word->next = runner->next;
        runner->next = new_word;
        return head;
    }
    return new_word;

}

/* frees the given linked list */
void free_list(struct word *head)
{
    if (!head)
        return;
    free_list(head->next);
    free(head);
}

/* reverses linked list */
struct word * reverse_list(struct word *head, struct word *prev)
{
    struct word *new_head;

    if (head)
        new_head = reverse_list(head->next, head);
    else
        return prev;
    head->next = prev;
    return new_head;
}

/* gets word string to be sorted */
struct word * word_gets( const char *src, unsigned int flags)
{
    unsigned int case_insensitive = 0, reverse = 0, white = 1;
    struct word *head = NULL;

    // set flags
    if (flags >= WORD_SORT_IGNORE_CASE)
        case_insensitive = 1;
    if (flags > 10 || flags == WORD_SORT_REVERSE)
        reverse = 1;

    // parse string
    for (; *src != '\0'; src++) {
        // if a new word is found
        if (*src > 32 && white) {
            head = word_list(head, src, case_insensitive);
            white = 0;
            continue;
        }
        // turn on whitespace flag when we re-enter whitespace
        if (*src < 33)
            white = 1;
    }
    // reverse if flagged
    if (reverse && head)
        head = reverse_list(head, NULL);
    return head;
}

/* puts words into the destination string */
unsigned int word_place(struct word *head, char *dst, unsigned int dst_len)
{
    int count = -1;
    struct word *runner = NULL;
    const char *letter;

    runner = head;
    while (runner && dst_len >= 2)
    {
        // copy letters into dst and keep track
        letter = runner->first;
        while (*letter > 32 dst_len >= 2)
        {
            *dst = *letter;
            letter++;
            dst++;
            count++;
            dst_len--;
        }
        runner = runner->next;

        // add ' ' if we have more words and memory space
        if (runner && dst_len >= 2)
        {
            // add space after word placed
            *dst = ' ';
            dst++;
            dst_len--;
            count++;
        }
    }
    // set final null character
    *dst = '\0';
    count++;

    // free our list
    free_list(head);
    return count;
}

/* main word_sort program */
unsigned int word_sort(const char *src, char *dst, unsigned int dst_len, unsigned int flags )
{
    struct word *head = NULL;
    unsigned int count;

    head = word_gets(src, flags);
    count = word_place(head, dst, dst_len);
    puts(dst);
    return count;
}


int main()
{
    char test[] = {"This is a    test of this crazy   program  "};
    char test2[] = {"  a b c d dz    dc da   \n\tAb Az"};
    char dst[50];
    char dst2[50];

    printf("%d\n", word_sort(&test[0], &dst[0], 50, 0));
    printf("%d\n", word_sort(&test[0], &dst[0], 50, 1));
    printf("%d\n", word_sort(&test[0], &dst[0], 50, 12));
    printf("%d\n", word_sort(&test2[0], &dst2[0], 50, 1));
    printf("%d\n", word_sort(&test2[0], &dst2[0], 50, 12));
    printf("%d\n", word_sort(&test2[0], &dst2[0], 5, 12));
}


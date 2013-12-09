/* Exercise 3.4.3: Construct the failure function for the strings:
 * a) abababaab.
 * b) aaaaaa
 * c) abbaabb.
 */

/* Introduction to Aho-Corasick algorithm.
 *
 * Purpose:
 *   Provide a fast way to match strings.
 *
 * How:
 *   Construction a failure function for a string.
 *
 * What is _failure function_?
 *   For example: we want to match string "ababaa" from the input:
 *
 *   State:    0 1 2 3 4 5 6
 *   Input:     a b b a b a b a a XXXXXXXX
 *   Dest:      a b a b a a
 *
 *   We see that state 3 don't match, now we can move to the next input 'b'
 *   and start the matching process again (i.e:)
 *
 *   State:    0 1 2 3 4 5 6
 *   Input:   a b b a b a b a a XXXXXXXX
 *   Dest:      a b a b a a
 *
 *   This works except that it is slow. For example: 
 *
 *   Dest:  a(999 a)b
 *   Input: a(2000 a)b
 *
 *   Now we match the first 999 a and fail at the final b, and then we move
 *   the the next character 'a' and start the process, which matches the 999
 *   As one more time. Which takes O(n*m).
 *
 *   Now we have a better way to do it. We memorize the characters we've
 *   matched. For example:
 *
 *   State:    0 1 2 3 4 5 6
 *   Input:     a b a b b a b a a XXXXXXXX
 *              ---
 *                  ---
 *   Dest:      a b a b a a
 *
 *   Now we fail at state 5, but we remember we've matched for "abab" and now
 *   it fails, we are sure that [1,2] and [3,4] of the input are both "ab",
 *   thus, instead of moving forward 1 character, we can move forward by 2.
 *   and again match state 3.
 *
 *   State:      0 1 2 3 4 5 6
 *   Input:     a b a b b a b a a XXXXXXXX
 *              ---
 *                  ---
 *   Dest:          a b a b a a
 *
 *   And _failure function_ is a map, it tell us if we fail in state 5, we can
 *   start in 3 instead of 0.
 *
 *       a        b       a      b       a       a
 *   (0) --> (1) --> (2) --> (3) --> (4) --> (5) --> ((6))
 *
 * How to generate failure function/map ?
 *   By calculating the length of longest string that is both the prefix and
 *   postfix of the string to be matched up to currest state.
 *
 *   For example, current state is 5, the string to be matched is "ababa" and
 *   the longest string that is both the prefix and postfix of "ababa" is
 *   "aba", and the length is 3, thus f(5) = 3.
 */

#include <stdio.h>
#include <string.h>
#include <malloc.h>

int *generate_failure_function(char *str_to_be_matched)
{
    /* allocate spaces for the map */
    int len = strlen(str_to_be_matched);
    int *map = (int *)malloc((len+1) * sizeof(*map));
    if (map == NULL) {
        fprintf(stderr, "generate_failure_function: not enough memory\n");
        return NULL;
    }


    /* note that map[t] points the end of first part of pre-post-prefix string
     * in the matched string 
     *   map[9]
     *    |
     * "123456123"
     */
    int i = 0;
    int t = 0;
    map[0] = map[1] = 0;
    for (i = 1; i < len; i++) {
        /* Note: string index begins with 0 while *map* begins with 1 */
        while (t>0 && str_to_be_matched[t] != str_to_be_matched[i]) {
            t = map[t];
        }

        if (str_to_be_matched[i] == str_to_be_matched[t]) {
            t = t+1;
            map[i+1] = t;
        } else {
            map[i+1] = 0;
        }
    }

    return map;
}

/* debug function */
void print_map(int *map, int len)
{
    int i;
    for (i = 1; i <= len; i++) {
        printf("%d->%d\n", i, map[i]);
    }
}

void test(char *str)
{
    int *map = NULL;
    map = generate_failure_function(str);
    printf("Map for string: \"%s\"\n", str);
    print_map(map, strlen(str));
    free(map);
}

int main(int argc, const char *argv[])
{
    test("abababaab");
    test("aaaaaa");
    test("abbaabb");
    test("ababaa");
    return 0;
}

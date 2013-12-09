/* apply the KMP algorithm */
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>

/* generate_failure_function from exercise 3.4.3 */
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

bool KMP(char *input, char *pattern)
{
    int *map = generate_failure_function(pattern);
    if (map == NULL) {
        fprintf(stderr, "KMP: generate failure function failed.\n");
        return false;
    }

    int i;
    int s = 0;  /* current state */
    int len_input = strlen(input);
    int len_pattern = strlen(pattern);
    for (i = 0; i < len_input; i++) {
        while (s > 0 && input[i] != pattern[s]) {
            s = map[s];
        }

        if (input[i] == pattern[s]) {
            s++;
        }

        if (s == len_pattern) {
            return true;
        }
    }

    return false;
}


int main(int argc, const char *argv[])
{
    char input[BUFSIZ];
    char pattern[BUFSIZ];
    while (true) {
        printf("Please input the pattern string:\n");
        fflush(stdin);
        scanf("%s", pattern);
        printf("Please input the string:\n");
        fflush(stdin);
        scanf("%s", input);
        printf("%s\n", KMP(input, pattern)?"The string matches:)\n" : "Match fail:(\n");
    }
    return 0;
}

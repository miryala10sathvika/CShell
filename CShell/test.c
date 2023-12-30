#include <stdio.h>
#include <string.h>

int main() {
    char input[] = "part1;part2;part3";
    char delimiter[] = ";";

    // Get the first token
    char *token = strtok(input, delimiter);

    // Skip the first token and continue with the rest
    while (token != NULL) {
        token = strtok(NULL, delimiter);  // Passing NULL to continue tokenizing
        if (token != NULL) {
            printf("Remaining part: %s\n", token);
        }
    }

    return 0;
}


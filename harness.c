#include <stdio.h>
#include <string.h>

int main(void) {
    char input[256];

    while (1) {
        printf("Enter text (type 'exit' to quit): ");

        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0) {
            printf("Exiting the program. Goodbye!\n");
            break;
        }

        if (strstr(input, "hello") != NULL) {
            printf("Hello there! Nice to see you.\n");
        } else {
            printf("You said: %s\n", input);
        }
    }

    return 0;
}

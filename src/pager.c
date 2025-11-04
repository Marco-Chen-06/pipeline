/*
 * Implementation notes:
 * Read keyboard input from /dev/tty, not stdin
 * If 23 lines are shown, then on the 24th line, display the message
 */

#include "programs.h"

int main(int argc, char* argv[]) {
    FILE *stdout_ptr;
    char line[256]; // lines aren't expected to be longer than 256 bytes

    stdout_ptr = fdopen(1, "w");

    while (fgets(line, sizeof(line), stdin) != NULL) {
        size_t len = strlen(line);

        // write the line to stdout if a newline is detected
        if (len > 0 && line[len-1] == '\n') {
            // remember to ERROR HANDLE
            fwrite(line, sizeof(char), len, stdout_ptr);
        }
    }
    return 0;
}
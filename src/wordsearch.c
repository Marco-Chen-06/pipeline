#include "programs.h"

int main(int argc, char* argv[]) {

    if(argc != 2) {
        perror("No file to read provided");
        return 0;
    }
    int count = 0;

    char *word = NULL; 
    size_t word_len = 0;

    char *dict_word = NULL;
    size_t dict_word_len = 0;



    ssize_t dict_read_length = 0;
    
    
    while(true) {
        FILE* dictionary = fopen(argv[1], "r");
        //open file for reading

        if(dictionary == NULL) {
            fprintf(stderr, "Failed to open file %s: %s.", argv[1], strerror(errno));
            return 0;
        }

        int dict_fd = fileno(dictionary);

        ssize_t read_length = getline(&word, &word_len, stdin); 
        //read one word from 

        // converts each "wordgen" word to uppercase
        for(int i = 0; i < word_len; i++) {
            if (word[i] >= 'a' && word[i] <= 'z') {
                word[i] = word[i]- 32;
            }
        }

        if ((read_length == -1)) {
            // don't print error if eof
            if (feof(stdin)) {
                break;
            }
            fprintf(stderr, "Failed to read line: %s. Latest word read: %s\n.", strerror(errno), word);
            free(word);
            free(dict_word);
            break;
        }

        while(getline(&dict_word, &dict_word_len, dictionary) != -1) {
            // converts each dictionary word to uppercase
            for(int i = 0; i < dict_word_len; i++) {
                if (dict_word[i] >= 'a' && dict_word[i] <= 'z') {
                    dict_word[i] = dict_word[i]- 32;
                }
            }

            if(strcmp(dict_word, word) == 0) {
                fprintf(stdout, "%s", word);
                count++;
            }
        }

        fclose(dictionary);

        /*if(lseek(dict_fd, 0, SEEK_SET) == -1) {
            perror("");
            return 0;
        } */
        //resets reading offset to start of file

    }
    
    
}
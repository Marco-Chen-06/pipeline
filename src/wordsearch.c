#include "programs.h"

int main(int argc, char* argv[]) {
    if(argc != 2) {
        perror("No file to read provided");
        return 0;
    }
    int accept_count = 0;
    int reject_count = 0;

    bool reject_flag = false;
    char *word = NULL; 
    size_t word_len = 0;

    char *dict_word = NULL;
    size_t dict_word_len = 0;

    char *dict_arr[1000000]; // make piping atomic to make life easier (holds BUFSIZ string pointers)

    //open file for reading
    FILE* dictionary = fopen(argv[1], "r");
    if(dictionary == NULL) {
        fprintf(stderr, "Failed to open file %s: %s.", argv[1], strerror(errno));
        return 0;
    }

    //-------------------TODO- implement something that prints to stderr and returns -1 if more than 8192 lines ------

    // loop through the dictionary and accept/reject words, then stuff words into dict_arr
    while((dict_word_len = getline(&dict_word, &dict_word_len, dictionary)) != -1) {
        // converts each dictionary word to uppercase
        for(int i = 0; i < dict_word_len-1; i++) {
            if (!((dict_word[i] >= 'a' && dict_word[i] <= 'z') || (dict_word[i] >= 'A' && dict_word[i] <= 'Z'))) {
                reject_count++;
                reject_flag = true;
                break;
            }
            if (dict_word[i] >= 'a' && dict_word[i] <= 'z') {
                dict_word[i] = dict_word[i]- 32;
            }
        }

        // don't add the word if it was rejected
        if (!reject_flag) {
            dict_arr[accept_count] = strdup(dict_word);
            accept_count++;
        }
        reject_flag = false;
    }

    // if dictionary stopped exiting for reasons other than seeing an EOF, then there was an error
    if (!(feof(dictionary))) {
        fprintf(stderr, "Failed to read line from dictionary: %s. Latest word read: %s\n.", strerror(errno), word);
    }

    fclose(dictionary);

    while(true) {

        ssize_t read_length = getline(&word, &word_len, stdin); 
        //read one word from 

        // converts each "wordgen" word to uppercase
        for(int i = 0; i < word_len-1; i++) {
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
            for (int i = 0; i < accept_count; i++) {
                free(dict_arr[i]);
            }
            free(word);
            free(dict_word);
            break;
        }


        for (int i = 0; i < accept_count; i++) {
            if(strcmp(dict_arr[i], word) == 0) {
                fprintf(stdout, "%s", word);
            }   
        }

        /*if(lseek(dict_fd, 0, SEEK_SET) == -1) {
            perror("");
            return 0;
        } */
        //resets reading offset to start of file
    }
    fprintf(stderr, "Accepted %d words, rejected %d.\n", accept_count, reject_count);
}
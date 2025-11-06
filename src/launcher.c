#include "programs.h"

int launcher(int wordcount);

int main(int argc, char* argv[]) {
    if(argc == 1 || (argc == 2 && strcmp(argv[1], "0") == 0)) {
        if ((launcher(-1)) == -1) {
            return -1;
        }
    }

    else {
        if (launcher(atoi(argv[1])) == -1) {
            return -1;
        }
    }
}

int launcher(int wordcount) {
    if(wordcount == 0) {
        wordcount = -1;
    }

    // file descriptors from wordgen to search
    int gen_to_search[2];

    // file descriptors from search to pager
    int search_to_pg[2];

    if (pipe(gen_to_search) < 0) {
        return -1;
    }

    if (pipe(search_to_pg) < 0) {
        return -1;
    }

    int pid = fork();
    int count = 0;

    while (count != 2 && pid != 0) {
        count++;
        pid = fork();
    }


    char wordcount_str[20]; // holds wordcount formatted as a string 
    sprintf(wordcount_str, "%d", wordcount);

    // only children are allowed to run exec and manipulate the pipes
    if (pid == 0) {
        switch(count) {
            case 0:
                close(gen_to_search[0]);
                dup2(gen_to_search[1], 1); // redirect stdout of wordgen to write side of pipe1
                close(gen_to_search[1]);
                close(search_to_pg[0]);
                close(search_to_pg[1]);

                char *wordgen_args[] = {"./wordgen", wordcount_str, NULL};
                execve("./wordgen", wordgen_args, NULL);
                return -1;
            case 1:
                dup2(gen_to_search[0], 0); // redirect stdin of wordsearch to read side of pipe1
                close(gen_to_search[0]);
                close(gen_to_search[1]);
                close(search_to_pg[0]);
                dup2(search_to_pg[1], 1); // redirect stdout of wordsearch to write side of pipe2
                close(search_to_pg[1]);

                char *wordsearch_args[] = {"./wordsearch", "./words.txt", NULL};
                execve("./wordsearch", wordsearch_args, NULL);
                return -1;
            case 2:
                close(gen_to_search[0]);
                close(gen_to_search[1]);
                dup2(search_to_pg[0], 0); // redirect stdin of pager to read side of pipe2
                close(search_to_pg[0]);
                close(search_to_pg[1]);
                
                char *pager_args[] = {"./pager", NULL};
                execve("./pager", pager_args, NULL);
                return -1;
        }
    }

    // parental actions, since all children have been exec'ed
    close(gen_to_search[0]);
    close(gen_to_search[1]);
    close(search_to_pg[0]);
    close(search_to_pg[1]);

    int status;
    pid_t child_pid;
    for(int i = 0; i < 3; i++) {
        child_pid = wait(&status);
        if(WIFEXITED(status)) {
            printf("Child %d exited with %d\n", child_pid, WEXITSTATUS(status));
        } else if(WIFSIGNALED(status)) {
            printf("Child %d exited with %d\n", child_pid, WTERMSIG(status));
        }
    }

    return 0;
}

// // -- pseudocode layout

// int pid = fork();
// int count = 0;
// while (count != 2 && pid != 0) {
//     count++;
//     pid = fork();
// }

// switch(count) {
//     case 0:
//         exec(wordgen);
//     case 1:
//         exec(wordsearch);
//     case 2:
//         exec(pager);
//     default:
//         wait();
//         wait();
//         wait();
// }
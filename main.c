#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define inputSize  100
#define historySize  20
#define clear() printf("\033[H\033[J")
#define Blue "\033[0;34m"
#define Green "\033[0;32m"
#define Cyan "\033[0;36m"
#define Purple "\033[0;35m"
#define Yeelow  "\x1b[33m"
#define Red "\x1b[31m"
#define Reset "\033[0m"

void nextLine() {
    printf("\n");
}

void executeHelp() {
    nextLine();
    printf(""Purple"*** MicroShell SOP ***"), nextLine();
    printf(""Yeelow"Author: ");
    printf(""Green"Jakub Olek"), nextLine();
    printf(""Red"My functions: "), nextLine();
    printf(""Cyan"help"), nextLine();
    printf("cd"), nextLine();
    printf("pwd"), nextLine();
    printf("mkdir"), nextLine();
    printf("history"), nextLine();
    printf("clear"), nextLine();
    printf("exit"), nextLine();
    nextLine();
}

void executeCd(char **userCommand) {
    if (userCommand[1] == NULL || strcmp(userCommand[1], "~") == 0) {
        chdir(getenv("HOME"));
    } else if (chdir(userCommand[1]) != 0) {
        printf("cd: %s: No such file or directory", userCommand[1]), nextLine();
    }
}

void executeMkdir(char **userCommand) {
    if (mkdir(userCommand[1], 0777) == 0) {
        printf("mkdir : directory ‘%s’ created", userCommand[1]), nextLine();
    } else {
        printf("mkdir : cannot create directory ‘%s’: File exists", userCommand[1]), nextLine();
    }
}

void executeHistory(char commandHistory[historySize][inputSize], int historyCounter) {
    int j;
    for (j = 0; j < historyCounter; j++) {
        printf("%d  %s", j + 1, commandHistory[j]), nextLine();
    }
}

void executePwd(char userInput[inputSize]) {
    printf("%s", getcwd(userInput, inputSize)), nextLine();
}

void printUserPath(char userInput[inputSize]) {
    printf(""Green"[%s:"Blue"%s]"Reset"\n$ ", getenv("USER"), getcwd(userInput, inputSize));
}

int main() {
    clear();
    int historyCounter = 0;
    char commandHistory[historySize][inputSize];
    char userInput[inputSize];

    while (1) {
        printUserPath(userInput);
        scanf(" %[^\n]%*c", userInput);
        strcpy(commandHistory[historyCounter++], userInput);

        char *separation = strtok(userInput, " ");
        char *userCommand[sizeof(userInput)] = {0};
        int index = 0;
        while (separation != NULL) {
            userCommand[index++] = separation;
            separation = strtok(NULL, " ");
        }

        if (strcmp(userCommand[0], "help") == 0) {
            executeHelp();
        } else if (strcmp(userCommand[0], "history") == 0) {
            executeHistory(commandHistory, historyCounter);
        } else if (strcmp(userCommand[0], "cd") == 0) {
            executeCd(userCommand);
        } else if (strcmp(userCommand[0], "pwd") == 0) {
            executePwd(userInput);
        } else if (strcmp(userCommand[0], "exit") == 0) {
            exit(0);
        } else if (strcmp(userCommand[0], "mkdir") == 0) {
            executeMkdir(userCommand);
        } else if (strcmp(userCommand[0], "clear") == 0) {
            clear();
        } else {
            int pid = fork();
            if (pid == 0) {
                if (execvp(userCommand[0], userCommand) != 0) {
                    printf("Command `%s` not found.", userCommand[0]), nextLine();
                    exit(0);
                }
            } else {
                wait(NULL);
            }
        }
    }
    return 0;
}
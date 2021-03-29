/*
 * -----
 * Code by: Abhijeet Suryawanshi
 * Student Number: 19370773
 * Email: abhijeet.suryawanshi@ucdconnect.ie
 * -----
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "exec.h"
#include "prompt.h"
void signalhandler(int signum);

/* This main function has Feature 3. It also has calls to exec() and prompt(). */
int main()
{
    /* signal() call to catch SIGNINT. Passed in a pointer to a function below main. */
    signal(SIGINT, &signalhandler);

    /* These are the variables used for reading and storing the input. */
    char *line = NULL;
    size_t len;
    ssize_t read;

    /* The date-time, path and # are printed. */
    prompt();

    while ((read = getline(&line, &len, stdin)) != -1)
    {
        /* Trimming of the newline character, replacing with null terminator. */
        if (line[strlen(line) - 1] == '\n')
        {
            line[strlen(line) - 1] = '\0';
        }

        /* Replaced system() call with exec() call. We execute the line that is read with getline. */
        exec(line);
        prompt();
    }

    free(line);
}

/* Function to pass into the signal() call. */
void signalhandler(int signum) {}
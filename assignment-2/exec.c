/*
 * File: exec.c
 * Project: assignment-2
 * Created Date: Friday, 19th March 2021 5:30:01 pm
 * -----
 * Code by: Abhijeet Suryawanshi
 * Student Number: 19370773
 * -----
 * Modified By: Abhijeet Suryawanshi
 * Date Modified: Saturday, 20th March 2021 6:30 pm
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "cd.h"

// This is the function that replaces system() call in main. Has features 1 and 5.
void exec(char *input)
{

    /* Tokenizing the passed in string input and storing. Counter keeps track of array positions. */
    char *token;
    int counter = 0;
    char *argv[32];

    /* Get the first token. */
    token = strtok(input, " ");

    /* If first token is NULL, return but do not exit program. */
    if (token == NULL) {
        return;
    }

    argv[0] = token;
    counter++;

    /* Tokenizing rest of the string until end is reached. */
    while (token != NULL)
    {
        token = strtok(NULL, " ");
        argv[counter] = token;
        counter++;
    }

    /* If the first argument is the builtin cd argument, then call cd() with the next argument as input. */
    if (strcmp(argv[0], "cd") == 0)
    {
        cd(argv[1]);
        return;
    }
    
    /* We are saving stdout so we can restore it later to terminal after redirecting. */
    int saved_stdout = -1;
    int f = -1;
    
    /* Loop though the entire argv[] string array to search for ">" symbol. */
    for (int i = 0; argv[i] != NULL; i++)
    {

        if (strcmp(argv[i], ">") == 0) {

            /* Error check to see if redirect file is specified. */
            if (argv[i + 1] == NULL) {

                perror("Redirect file not specified.\n");
                return;
            }
            
            /* Set the redirect filename to the next argument from ">" and set saved_stdout so we can restore it. */
            char *filename = argv[i + 1];
            f = open(filename, O_WRONLY|O_CREAT|O_TRUNC, 0640); 
            saved_stdout = dup(1);
            dup2(f, STDOUT_FILENO);
            argv[i] = NULL;            
        }   
    }

    int pid = fork();

    /* Error check to see if the fork() failed to execute. */
    if (pid == -1)
    {
        perror("fork() failed.\n");
    }
    /* Wait for the child process to finish. */
    else if (pid > 0)
    {
        wait(NULL);
    }
    else
    {
        /* We are the child */
        execvp(argv[0], argv);
        /* execvp() only returns if there is an error. */
        exit(EXIT_FAILURE);
    }

    /* If saved_stdout is not -1 then a redirect occurred and stdout needs to be restored to terminal. */
    if (saved_stdout != -1) {
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);
    }
    
    /* We also close the file if it was opened during redirect. */
    if (f != -1) 
    {
        close(f);
    }

    return;
}
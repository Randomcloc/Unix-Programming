/*
 * -----
 * Code by: Abhijeet Suryawanshi
 * Student Number: 19370773
 * Email: abhijeet.suryawanshi@ucdconnect.ie
 * -----
 */
#include "cd.h"

/* This function is the change-directory builtin which is Feature 4. */
void cd(char *dir)
{
    /* If there is nothing after the cd command, send the user to home directory. */
    if (dir == NULL)
    {

        dir = getenv("HOME");
    }

    /* This follows from when we set the home path to appear as "~". So all paths will start with this. */
    if (dir[0] == '~')
    {
        /* We concatenate the rest of the input path with the "~". */
        char buffer[PATH_MAX];
        buffer[0] = '\0';
        char *home = getenv("HOME");

        strcat(buffer, home);
        strcat(buffer, dir + 1);

        dir = buffer;
    }

    /* chdir is called on the input path, if it's -1 then there is an error. */
    if (chdir(dir) == -1)
    {
        /* Error is printed based on the error number received. */
        fprintf(stderr, "cd: %s: %s\n", dir, strerror(errno));
    }
}
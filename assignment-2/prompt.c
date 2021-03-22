/*
 * -----
 * Code by: Abhijeet Suryawanshi
 * Student Number: 19370773
 * Email: abhijeet.suryawanshi@ucdconnect.ie
 * -----
 */

#include "prompt.h"

// This method is used to print the time, the path for shell, and the #. Has feature 2.
void prompt()
{
    /* The timer and tm_info are there to store the epoch time and the converted time. */
    time_t timer;
    char buffer[PATH_MAX];
    struct tm *tm_info;

    /* timer is set to current time, tm_info is set to the converted timer time (epoch). */
    timer = time(NULL);
    tm_info = localtime(&timer);

    /* strftime() converts the tm_info time to a readable format. */
    strftime(buffer, 26, "[%d/%m %H:%M] ", tm_info);
    printf("%s", buffer);

    /* getcwd() gets the current directory the shell is in. */
    if (getcwd(buffer, sizeof(buffer)) == NULL) {
        perror("error\n");
        return;
    }

    /* Replacing home path with "~". */
    char* home = getenv("HOME");

    /* Adding ~ to the string. */
    if (strncmp(buffer, home, strlen(home)) == 0) {
        buffer[strlen(home) - 1] = '~';
    }

    /* Offsetting the original string so we don't see the home path. */
    printf("%s # ", buffer + strlen(home) - 1);

    return;
}
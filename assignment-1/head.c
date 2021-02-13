/* -----
 * Code by: Abhijeet Suryawanshi
 * Student Number: 19370773
 * Email: abhijeet.suryawanshi@ucdconnect.ie
 * -----
 * Explanation: 
 * This code has the head functionality inplemented which gets the first 10 lines of a given file for input (if the -n flag is not present).
 * The user can specify how many lines they want to be printed to the terminal with the -n flag followed by the desired number.
 * The -e flag will print the first n even lines from the file.
 * The -h flag prints all the options/flags available for the user.
 * the -V flag prints my name, email, and student number in the terminal.
 * -----
 */
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {

	/* A boolean value which tells us if the even flag was used or not.*/
	bool evenflag = false;
	/* The value of the number of lines to be printed (10 by default).*/
	int nvalue = 10;
	/* This stores the return value from the getopt() function.*/
	int opt;
	/* Stores the character value of the number which is followed by the -n flag.*/
	char num;

	// This loop sets the variables/prints the required strings to the terminal based on the flags the user inputs.
	while ((opt = getopt(argc, argv, "n:eVh")) != -1) {
		
		switch (opt)
    	{	
			/* -n flag takes in a number with it, but this is a character, so we convert it to an int using sscanf(). */
			case 'n':
				sscanf(optarg, "%d", &nvalue);
				break;
			case 'e':
				evenflag = true;		/* We set the evenflag value to true since the -e flag would be used by the user. */
				break;
			case 'V':
				printf("Name: Abhijeet Suryawanshi\nEmail: abhijeet.suryawanshi@ucdconnect.ie\nStudent Number: 19370773");
				return 0;
			case 'h':
				printf("Options:\n\
				-n K output the first|last K lines.\n\
				-V Output version info: Your name, email, student number.\n\
				-h display all options ( something like this ) and exit.\n\
				-e print even lines.\n");
				return 0;
		}
	}

    FILE *fp;
	size_t len; 
	char *line = NULL;
	ssize_t read;
	/* This counter will be used to count the number of lines already printed to the terminal. */
	int printCount = 0;
	/* The counter that will be used to check the line at which the file is reading.*/
	int i = 1;
	
	fp = fopen(argv[argc - 1], "r");
	
	if (fp == NULL) {

		printf("error\n");
		exit(EXIT_FAILURE);	
	}

	while ((read = getline(&line, &len, fp)) != -1) { 
		
		//If the even flag was used, only print the even lines 
		if (evenflag) {
			if (i % 2 == 0 && printCount <= nvalue) {
				printf("%s", line);
				printCount++;
			}
			
			i++;
		}
		else {
			if (i <= nvalue)
				printf("%s", line); 
			i++; 
		}
	}

	free(line);	
	fclose(fp);
	return 0;
}
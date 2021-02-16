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
 * The program can take in standard input and print the lines that should be printed, but it does so after immidiately receiving a line.
 * -----
 * Progress:
 * If a file is given, the program works perfectly. If there is a -n value that is larger than the lines that can be retreived, the program prints the entire file.
 * The program can only be stopped by manual termination if standard input is being taken (Ctrl + c).
 * If standard input is taken, the program will output to standard output as the lines are being typed into stdin.
 * -----
 */
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <string.h>

int main(int argc, char *argv[]) {

	/* A boolean value which tells us if the even/odd flag was used or not.*/
	bool evenflag = false;
	bool oddflag = false;
	/* The value of the number of lines to be printed (10 by default).*/
	int nvalue = 10;
	/* This stores the return value from the getopt() function.*/
	int opt;

	FILE *fp;
	size_t len; 
	char *line = NULL;
	ssize_t read;
	/* This counter will be used to count the number of lines already printed to the terminal. */
	int printCount = 0;
	/* The counter that will be used to check the line at which the file is reading.*/
	int i = 1;

	// This loop sets the variables/prints the required strings to the terminal based on the flags the user inputs.
	while ((opt = getopt(argc, argv, "n:eoVh")) != -1) {
		
		switch (opt)
    	{	
			/* -n flag takes in a number with it, but this is a character, so we convert it to an int using sscanf(). */
			case 'n':
				sscanf(optarg, "%d", &nvalue);
				break;
			case 'e':
				evenflag = true;		/* We set the evenflag value to true since the -e flag would be used by the user. */
				break;
			case 'o':
				oddflag = true;
				break;
			case 'V':
				printf("Name: Abhijeet Suryawanshi\nEmail: abhijeet.suryawanshi@ucdconnect.ie\nStudent Number: 19370773\n");
				return 0;
			case 'h':
				printf("Options:\n\
				-n K output the first|last K lines.\n\
				-V Output version info: Your name, email, student number.\n\
				-h display all options ( something like this ) and exit.\n\
				-e print even lines.\n\
				-o print odd lines.\n");
				return 0;
		}
	}
	
	fp = fopen(argv[argc - 1], "r");
	
	if (fp == NULL || strcmp(argv[argc - 1], "./head") == 0) {

		//If no file, or a non-existent file is given, standard input will be taken in.
		printf("Error opening file: Input from user will be taken, press (Crtl + c) to exit.\n");
		fp = stdin;	
	}

	while ((read = getline(&line, &len, fp)) != -1) { 
		
		if (i % 2 == 0 && printCount < nvalue && evenflag) {
			printf("%d. %s", i, line);
			printCount++;
		}
			
		else if (i % 2 != 0 && printCount < nvalue && oddflag) {
			printf("%d. %s", i, line);
			printCount++;
		}

		else if (i <= nvalue && evenflag == oddflag) 
			printf("%d. %s", i, line); 
		i++;		
	}

	free(line);	
	fclose(fp);
	return 0;
}
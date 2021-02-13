#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

	if (argc != 3) {
		printf("\nargc must by 3.\n");
		exit(1);
	}
	
	FILE *fp;
	char *line = NULL;
	size_t len = 0; 
	ssize_t read;
	
	fp = fopen(argv[argc - 1], "r");
	
	if (fp == NULL) {

		exit(EXIT_FAILURE);	
	}

	while ((read = getline(&line, &len, fp)) != -1) { 
		
		if ((strstr(line, argv[argc - 2])) != NULL) {
			printf("%s\n", line);
		}
		else {
			printf("\nSubstring not present\n");
			return 0;
		}
	}

	free(line);	
	fclose(fp);
	return 0;
}

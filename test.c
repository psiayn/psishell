#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"


/*
 * Reads a line of input from the terminal
 */
char*
lsh_read_line(void)
{
	char* line = NULL;
	size_t bufsize = 0;

	if (getline(&line, &bufsize, stdin) == -1) {
		if(feof(stdin)) {
			exit(EXIT_SUCCESS);
		} else {
			perror("readline");
			exit(EXIT_FAILURE);
		}
	}

	return line;
}


/*
 * Splits the line read from the user into different readable commands
*/
char**
lsh_split_line(char* line)
{
	int bufsize = LSH_TOK_BUFSIZE, position = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
	char *token;

	if (!tokens) {
		fprintf(stderr, "lsh: allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, LSH_TOK_DELIM);
	while(token != NULL) {
		tokens[position] = token;
		position++;

		if (position >= bufsize) {
			bufsize += LSH_TOK_BUFSIZE;
			tokens = realloc(tokens, bufsize * sizeof(char*));
			if (!tokens) {
				fprintf(stderr, "lsh: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}

		token = strtok(NULL, LSH_TOK_DELIM);
}
	tokens[position] = NULL;
	return tokens;

}


/*
 * Launches programs that aren't part of the shell
*/
int 
lsh_launch(char **args) 
{ 
	pid_t pid, wpid;
	int status;

	pid = fork();
	if (pid ==0) {
		if (execvp(args[0], args) == -1) {
			perror("lsh");
		}
		exit(EXIT_FAILURE);
	} else if (pid < 0) {
		perror("lsh");
	} else { 
		do { 
			wpid = waitpid(pid, &status, WUNTRACED);
		}while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}

	return 1;
}

int
lsh_cd(char **args);

int
lsh_help(char **args);

int
lsh_exit(char **args);

char* builtin_str[] = {
	"cd",
	"help",
	"exit"
};

int (*builtin_func[]) (char **) = {
	&lsh_cd,
	&lsh_help,
	&lsh_exit
};

int
lsh_num_builtins() 
{
	return sizeof(builtin_str) / sizeof(char*);
}

int
lsh_cd(char **args)
{
	if (args[1] == NULL) {
		fprintf(stderr, "lsh: expected argument to \"cd\"\n");
	} else {
		if (chdir(args[1]) != 0) {
			perror("lsh");
		}
	}
	return 1;
}

int 
lsh_help(char**args)
{
	int i;
	printf("Psiayn's LSH (Inspired by Stephen Brennans's LSH)\n");
	printf("Type program names and arguments, and hit enter.\n");
	printf("The following commands are builtin:\n");
	
	int lsh_num = lsh_num_builtins();

	for (i = 0; i < lsh_num; i++) {
		printf( " %s\n", builtin_str[i]);	
	}
	
	printf("Use the \"man\" command for information on other programs.\n");
	return 1;
}

int 
lsh_exit(char **args)
{
	return 0;
}

int
lsh_execute(char **args)
{
	int i;

	if (args[0] == NULL) {
		return 1;	
	}

	int lsh_num = lsh_num_builtins();
	for (i = 0; i < lsh_num; i++) {
		if (strcmp(args[0], builtin_str[i]) == 0) {
			return (*builtin_func[i])(args);	
		}	
	}

	return lsh_launch(args);
}


/*
 * The basic loop of the shell
*/
void
lsh_loop(void)
{
	char *line;
	char **args;
	int status;
	do {
		printf("> ");
		line = lsh_read_line();
		args = lsh_split_line(line);
		status = lsh_execute(args);
		
		free(args);
		free(line);
	} while (status);
}

int
main (int argc, char** arv)
{
	lsh_loop();
	
	return EXIT_SUCCESS;	
}


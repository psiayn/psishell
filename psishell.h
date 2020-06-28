#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>

/*** Functions that add colors to the output ***/
void red();
void red_bold();

void green();
void green_bold();

void yellow();
void yellow_bold();

void blue();
void blue_bold();

void magenta();
void magenta_bold();

void cyan();
void cyan_bold();

void reset();

/*** rawmode functions ***/
void disableRawMode();
void enableRawMode();

/*** shell history functions ***/
void initHistory();
int checkCapacity();
void appendCommand(char* command);
void removeLastCommand();
void addHistory(char *command);
char* getCommandPrev();
char* getCommandNext();
void getCommand(int direction);

/*** main shell functions ***/
char* lsh_read_line(void);
char** lsh_split_line(char* line);
int lsh_launch(char **args);

int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);

int lsh_num_builtins();



int lsh_execute(char **args);

void lsh_loop(void);


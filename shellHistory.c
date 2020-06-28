#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "psishell.h"


#define COMMAND_CAPACITY 10

struct commandHistory { 
	char** commands;
	int capacity;
	int cursor;
};

struct commandHistory commHis;

void 
initHistory() 
{
	commHis.capacity = 0;
	commHis.cursor = commHis.capacity;
	commHis.commands = malloc(sizeof(char*) * commHis.capacity);	
}

int 
checkCapacity()
{
	return (commHis.capacity == COMMAND_CAPACITY) ? 0 : 1;
}

void 
appendCommand(char* command)
{
	commHis.commands[commHis.capacity - 1] = command;
	commHis.capacity++;
	commHis.cursor = commHis.capacity;
}

void 
removeLastCommand()
{
	memmove(commHis.commands, commHis.commands, sizeof(COMMAND_CAPACITY - 1));
	commHis.capacity--;	
	commHis.cursor = commHis.capacity;
}

void 
addHistory(char *command)
{
	if(!checkCapacity()) {
		removeLastCommand();
	} 
		appendCommand(command);	
}

char*
getCommandPrev() 
{	
	char* result = "";
	result = commHis.commands[commHis.cursor - 1];
	if (commHis.cursor == 0) {
		return result;
	}
	commHis.cursor--;
	return result;	
}

char*
getCommandNext() 
{	
	char* result = "";
	if (commHis.cursor == commHis.capacity) {
		return result;
	}
	result = commHis.commands[commHis.cursor - 1];
	commHis.cursor++;
	return result;	
}

void
getCommand(int direction)
{
	switch ( direction ) {
		case 1:
			getCommandNext();
			break;
		case -1:
			getCommandPrev();
			break;
	}
}



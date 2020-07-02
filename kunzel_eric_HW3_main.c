/**************************************************************
* Class:  CSC-415-02 Summer 2020
* Name: Eric Kunzel
* Student ID: 917054221
* Project: Assignment 3 – Simple Shell
*
* File: kunzel_eric_HW3_main.txt
*
* Description: This is a simple shell that takes in user input  and then attempts to execute that input through * the use of fork and exec. The input is parsed and sent to an exec function
**************************************************************/
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

char ** parseString(char []);			//function declarations
int exec_inpt(char**);

char ** parseString(char c[])			//tokenizes input, placing tokens in array of strings command
{
	char ** command = malloc(1024);		//will hold tokens, return
	char * parse;				//will hold parsed strings temporarily, then overwritten
	int i = 0;				//counter for parsing though string
	parse = strtok(c, " \n");		//this parses until a space is reached
	while (parse != NULL) 
	{
		command[i] = parse;
		i++;
		parse = strtok(NULL, " \n");
    	}
	command[i] = NULL;			//sets last to null, for execvp
	return command;				//returns parsed string arrray
}

int exec_inpt(char ** inpt )
{
	pid_t childId;				//process id created for child after fork
	childId = fork();			//fork creates second concurrent process
	int status;
	if(childId < 0)				//negative number is an error
	{
		printf("//error\n");
		printf("Not a valid command\n");
		return -1;
	}
	else if(childId == 0)
	{
		if (execvp(inpt[0], inpt) == -1) 	//exec here,past here in child not executed if no err
		{
      			printf("ERROR: %d\n", errno);	//error message if execvp encounters error
			exit(-1);
    		}
	}else
	{
		waitpid(childId, &status, WUNTRACED);	//parent waits for child, 
		
	}
	return 0;
}

int main(int argc, char* argv[])					
{
	char c[1024];				//will store input entered by user
	char ** p_command;			//array of parsed strings ends in null
	char * default_st = ">_";		//prints before user input
	char* exit = "exit";			//compares to p_command, exits if equal
	while (1)
	{
		printf("Enter command, or type exit to quit\n");	
		printf("%s", default_st);	//prints >_ at start
		fgets(c, sizeof(c), stdin);	//accepts in user input, which is the command
		p_command = parseString(c);	//fills p_command array with parsed data
		if(strcmp(p_command[0],exit) == 0)	//compares strings and exits if p_command == exit
		{
			free(p_command);	//free memory
			return 0;
		}
		exec_inpt(p_command);		//executes command given by user

	}
	return 0;
}

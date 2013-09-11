#include "command.h"
#include "buddySystem.h"
/**
* Parses a string buffer for the commands and it's arguments and places them in an array of strings.
* @param commands Array of strings to place the parsed command and arguments into.
* @param buf String to parse
* @param count Pointer to an int to store the count of arguments placed into the string array.
*/
void getArgument(char *commands[], char* buf, int* count)
{
	char *delim = " ";
	char *token;
	char *savePtr;
	char *origBuf;
	int i = 0;

	origBuf = (char *)buddy_malloc((strlen(buf)+1)*sizeof(char));
	strcpy(origBuf, buf);

	for(i=0; i<2048; i++) commands[i] = 0;
	i=0;
	token = strtok_r(buf, delim, &savePtr);
	if(token != NULL){
		commands[0] = buddy_malloc(sizeof(char)*strlen(token)+1);
		strcpy(commands[0], token);
	}
	i++;
	while((token = strtok_r(NULL, delim, &savePtr)) != NULL)
	{
		commands[i] = buddy_malloc(sizeof(char)*strlen(token)+1);
		strcpy(commands[i], token);
		i++;
	}

	strcpy(buf, origBuf);
	buddy_free(origBuf);
	*(count) = i;
}
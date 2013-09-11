#include "shellUtil.h"

/**
* Changes the directory of the dash
* @param argyments Array of strings that contains the arguments passed in for the cd command
*/
void changeDir(char** arguments)
{
	if(arguments[1] == NULL)
	{
		struct passwd * userData = getpwuid(getuid());
		char * path = userData->pw_dir;
		if(chdir(path) < 0)
		{
			err_ret("couldn't change to home directory: %s", path);
		}
	}
	else
	{
		if(chdir(arguments[1]) < 0)
		{
			err_ret("couldn't change to directory: %s", arguments[1]);
		}
	}
}

/**
* Takes a process on the job list and puts it in the foreground of the dash
* Note: if the job list contains no running jobs, no stopped jobs, or the argument
* points to a job that does not exist, the function will simply do nothing and return.
* @param arguments Array of string arguments passed in with the fg command
* @param l Job list for the dash
* @param count Number of arguments in the arguments array. Used to free the array
* when it is no longer needed
* @param bufCopy Copy of the buffer to free after the job is put in the foregrond. Optional.
*/
void fg(char** arguments, ListPtr l, int count, char* bufCopy)
{
	int isArgumentNull = arguments[1] == NULL;
	if(isArgumentNull)
	{
		freeArguments(count, arguments, bufCopy);
		NodePtr n = searchOnRunning(l, RUNNING);
		if(!isNodeNull(n))
		{
			JobPtr j = n->data;
			if(!isJobNull(j)) continueJobInForeground(n, l);
		}
		else{
			NodePtr n = searchOnRunning(l, STOPPED);
			if(!isNodeNull(n))
			{
				JobPtr j = n->data;
				if(!isJobNull(j)) continueJobInForeground(n, l);
			}
		}

	}
	else
	{
		int jobNumber = atoi(arguments[1]);
		freeArguments(count, arguments, bufCopy);
		NodePtr n = searchOnJobNu(l, jobNumber);
		if(!isNodeNull(n))
		{
			JobPtr j = n->data;
			if(!isJobNull(j)) continueJobInForeground(n, l);
		}
	}
}

/**
* Takes a process on the job list and puts it in the background of the dash
* Note: if the job list contains no running jobs, no stopped jobs, or the argument
* points to a job that does not exist, the function will simply do nothing and return.
* @param arguments Array of string arguments passed in with the fg command
* @param l Job list for the dash
* @param count Number of arguments in the arguments array. Used to free the array
* when it is no longer needed
* @param bufCopy Copy of the buffer to free after the job is put in the foregrond. Optional.
*/
void bgS(char** arguments, ListPtr l, int count, char* bufCopy)
{
	int isNull = arguments[1] == NULL;
	if(isNull)
        {
		freeArguments(count, arguments, bufCopy);
		NodePtr n = searchOnRunning(l, STOPPED);
		if(!isNodeNull(n))
		{
			JobPtr j = n->data;
			if(!isJobNull(j)) continueJobInBackground(n, l);
		}
	}
	else
	{
		int jobNumber = atoi(arguments[1]);
		freeArguments(count, arguments, bufCopy);
		NodePtr n = searchOnJobNu(l, jobNumber);
		if(!isNodeNull(n))
		{
			JobPtr j = n->data;
			if(!isJobNull(j)) continueJobInBackground(n, l);
		}
	}
}

/**
* Gets and returns the prompt for the dash to use.
* The environment variable that defines the dash prompt is DASH_PROMPT
* @return Prompt that is either defined by the evironment variable, or the default value %%
*/
char* getPrompt()
{
	char *prompt = getenv("DASH_PROMPT");
	return (prompt == NULL) ? "%% " : prompt; 
}

/**
* Prints any jobs that are done in the job list.
* @param jobList Job list to search for done jobs.
*/
void printFinishedJobs(ListPtr jobList)
{
	int backgroundStatus;
	
	if(jobList->size != 0)
	{
		NodePtr node = jobList->head;
		while(node != NULL)
		{
			if(waitpid(node->data->jobid, &backgroundStatus, WNOHANG) != 0)
			{
				if(!isJobStopped(node->data))
				{
					printf("[%i] Done %s\n", node->data->jobNu, node->data->command);
					node->data->running = 0;
					node->data->status = backgroundStatus;
				}
			}
			node = node->next;
		}
	}
}

/**
* Returns whether or not the string buffer contains the exit command
* @param buf String buffer
* @return Boolean true or false depending on whether the buffer contains the exit command
*/
int isExitCommand(char *buf)
{
	return (strcmp(buf, "exit") == 0 || strcmp(buf, "logout") == 0);
}

/**
* Returns whether or not the string buffer contains the background command
* @param buf String buffer
* @return Boolean true or false depending on whether the buffer contains the background command
*/
int isBackgroundCommand(char *buf)
{
	if(buf[strlen(buf)-1] == '&')
	{
		buf[strlen(buf)-1] = '\0';
		return 1;
	}
	else
		return 0;

}

/*int isStdInOrOutRedirected(char **buf, int count)
{
	int i = 0;
	for(; i < count; i++)
		if(strcmp(buf[i], ">") == 0 || strcmp(buf[i], "<" == 0)) return 1;

	return 0;
}

void removeRedirectionFromBuffer(char *buf)
{
	int i = 0;
	int strLength = strlen(buf);

	for(; i < strLength; i++)
	{
		if(buf[i] == '<' || buf[i] == '>'){
			buf[i] = '\0';
			return;
		}
	}
}

void removeRedirectionFromBufferArray(char **buf, int co, int *count)
{
	int i = 0;
	int c = co;

	for(; i < c; i++)
		if(strcmp(buf[i], ">") == 0 || strcmp(buf[i], "<")) break;

	for(; i < c; i++)
	{
		free(buf[i]);
		*(count)--;
	}
}*/


/**
* Returns whether or not the string buffer contains the cd command
* @param buf String buffer
* @return Boolean true or false depending on whether the buffer contains the cd command
*/
int isCdCommand(char* bufCopy, char** arguments)
{
	return (strlen(bufCopy) > 0 && strcmp(arguments[0], "cd") == 0);
}

/**
* Returns whether or not the string buffer contains the jobs command
* @param buf String buffer
* @return Boolean true or false depending on whether the buffer contains the jobs command
*/
int isJobsCommand(char* buf)
{
	return (strcmp(buf, "jobs") == 0);
}

/**
* Returns whether or not the string buffer is empty
* @param buf String buffer
* @return Boolean true or false depending on whether the buffer is empty
*/
int isCommandNotEmpty(char* buf)
{
	return (strlen(buf) != 0);
}

/**
* Returns whether or not the string buffer contains the fg command
* @param buf String buffer
* @return Boolean true or false depending on whether the buffer contains the fg  command
*/
int isFgCommand(char* bufCopy, char** arguments)
{
	return (strlen(bufCopy) > 0 && strcmp(arguments[0], "fg") == 0);
}

/**
* Returns whether or not the string buffer contains the bg command
* @param buf String buffer
* @return Boolean true or false depending on whether the buffer contains the bg command
*/
int isBgCommand(char* bufCopy, char** arguments)
{
	return (strlen(bufCopy) > 0 && strcmp(arguments[0], "bg") == 0);
}

/**
* Executes the command and it's arguments contained in the array of string.
* @param args Array of strings containing the command itself at index 0.
* @param buf String buffer used to display an error to the user if the command can not execute.
*/
void runCommand(char **args, char* buf)
{
	printf("%s", args[0]);
	execvp(*args, args);
	err_ret("couldn't execute: %s", buf);
	exit(127);
}

/**
* Returns whether or not the dash's arguments contains the version command
* @param buf String buffer
* @return Boolean true or false depending on whether the dash's arguments contains the version command
*/
int isArgumentForShowingVersion(char **argv)
{
	return (argv[1] != NULL && strcmp(argv[1],"v"));
}

/**
* Shows the current repository version of the dash, and exits the dash.
*/
void showVersionAndExit()
{
	printf("Version %s\n", SVNVER);
	exit(0);
}

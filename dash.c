/* 
  ch2/shell1.c
  This program needs the file error.c and ourhdr.h to compile. 
 
 */
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <pwd.h>
#include <signal.h>
#include <setjmp.h>
#include "ourhdr.h"
#include "List.h"
#include "keyBindings.h"
#include "command.h"
#include "shellUtil.h"
#include "signals.h"
#include "buddySystem.h"

ListPtr jobList;
jmp_buf env;

void jumpBack(){ siglongjmp(env,333333); }

void initialize()
{
	jobList = createList();
	using_history();
	bindKeys();
	initSignals(jumpBack);
}

NodePtr createAndSetNode(int *jobNumber, int backgroundJob, char* bufCopy, int isBackground){
     JobPtr job = createJob((int)backgroundJob, bufCopy, *jobNumber);
	 if(isBackground == 1) job->background = 1;
	 (*jobNumber)++;
	 NodePtr node = createNode(job);
	 setNode(node);
	 return node;
}

void recordCommand(NodePtr n, int jobNumber, int isBackground)
{
	printf("[%i] %i %s\n", jobNumber, n->data->jobid, n->data->command);
	addAtFront(jobList, n);
}

int main(int argc, char **argv)
{
	if(isArgumentForShowingVersion(argv)) showVersionAndExit();
	char	   *buf;
	char       *bufCopy;
	pid_t	   pid = getpid();
	pid_t      backgroundJob;
	int		   status = 0;
	char       *prompt = getPrompt();
	int  	   bg;
	int        jobNumber = 1;
	int counts;
	int jumpSet = 0;
	char *arguments[2048];
	initialize();
	printf("\n");
	setPrompt(prompt);
	while ((buf = readline(prompt))) {
		if(strlen(buf) != 0) {
			bufCopy = buddy_malloc((strlen(buf)+1)*sizeof(char));
			strcpy(bufCopy, buf);
			add_history(bufCopy);
		}
		else
		{
			 printFinishedJobs(jobList);
			 free(buf);
			 continue;
		}
		free(buf);
		if(isCommandNotEmpty(bufCopy)) bg = isBackgroundCommand(bufCopy);
		getArgument(arguments, bufCopy, &counts);
		if(isExitCommand(bufCopy)){
			buddy_free(bufCopy);
			freeList(jobList);
			if(arguments != NULL){
				freeArguments(counts, arguments, NULL);
			}
			exit(0);
		}
		if(isCdCommand(bufCopy, arguments)){
			changeDir(arguments);
		    freeArguments(counts, arguments, bufCopy);		
		}
		else if(isJobsCommand(bufCopy)) {
			printList(jobList);
			freeArguments(counts, arguments, bufCopy);
		}
		else if(isFgCommand(bufCopy, arguments)){			
			fg(arguments, jobList, counts, bufCopy);
		}
		else if(isBgCommand(bufCopy, arguments)){
			bgS(arguments, jobList, counts, bufCopy);
		}
		else if (isCommandNotEmpty(bufCopy)) 
		{
			if(bg == 1)
			{
				backgroundJob = fork();
				if(backgroundJob == 0) {
					ignoreSignals();
					runCommand(arguments, bufCopy);
				}
				else{
					NodePtr n = createAndSetNode(&jobNumber, backgroundJob, bufCopy, bg);
					recordCommand(n, jobNumber, bg);
					freeArguments(counts, arguments, bufCopy);
				}
			}
			else
			{		
				pid = fork();
				if(pid == 0){
					runCommand(arguments, bufCopy);
				}
				else {
					NodePtr n = createAndSetNode(&jobNumber, pid, bufCopy, bg);
					ignoreSignals();
					if((waitpid(pid,&status,WUNTRACED))<0) err_sys("waitpid error");
					freeArguments(counts, arguments, bufCopy);
					restoreSignalsAndDetermineStatus(status, jobList, n);
				}
			}
		}
		else
		{
			freeArguments(counts, arguments, bufCopy);
		}
		if(jumpSet == 0){
			sigsetjmp(env, 1);
			jumpSet = 1;
		}
		continue;
	}
	if(buf == NULL) printf("\n");
	exit(0);
}

/* vim: set ts=4: */

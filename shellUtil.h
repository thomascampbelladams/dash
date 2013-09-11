#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <pwd.h>
#include <signal.h>
#include <unistd.h>
#include "ourhdr.h"
#include "List.h"
#include "Node.h"
#include "signals.h"

char* getPrompt();
void printFinishedJobs(ListPtr jobList);
int isExitCommand(char *buf);
int isBackgroundCommand(char *buf);
int isCdCommand(char* bufCopy, char** arguments);
int isJobsCommand(char* buf);
int isCommandNotEmpty(char* buf);
void runCommand(char** command, char* buf);
int isArgumentForShowingVersion(char **argv);
void showVersionAndExit();
void changeDir(char** arguments);
int isFgCommand(char* bufCopy, char** arguments);
int isBgCommand(char* bufCopy, char** arguments);
void fg(char** arguments, ListPtr l, int count, char* bufCopy);
void bgS(char** arguments, ListPtr l, int count, char* bufCopy);
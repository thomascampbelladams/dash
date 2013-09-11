#ifndef __JOB_H
#define __JOB_H

#include <stdio.h>
#include <stdlib.h>
#include "common.h"

#define MAXPID_DIGITS 20
#define RUNNING 1
#define DONE 0
#define STOPPED 2

/**
* Alias for a Job Struct
*/
typedef struct job  Job;
/**
* Alias for a pointer to a Job Struct
*/
typedef struct job * JobPtr;

/**
* A structure to represent jobs in a list
*/
struct job {
	/*@{*/
	int jobid; /**< PID for the job */
	int jobNu; /**< Number for the job */
	char *command; /**< Command represented by this job struct */
	int status; /**< Status of the job */
	int running; /**< Status of the job */
	int background; /**< Boolean value for if the job is in the background */
	/*@}*/
};

JobPtr createJob (int, char *, int);
void freeJob(JobPtr job);
char *toString(JobPtr);
int isJobNull(JobPtr job);
int isJobRunning(JobPtr job);
int isJobInBackground(JobPtr job);
int isJobStopped(JobPtr job);
int isJobDone(JobPtr job);
void freeArguments(int count, char** args, char* bufCopy);
#endif /* __JOB_H */

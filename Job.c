#include "Job.h"
#include "buddySystem.h"

/**
* Tests the JobPtr to see if it is NULL
* @param job JobPtr to test
* @return Result of test
*/
int isJobNull(JobPtr job){ return job == NULL; }

/**
* Tests the JobPtr to see if it is running
* @param job JobPtr to test
* @return Result of test
*/
int isJobRunning(JobPtr job){ return job->running == RUNNING;}

/**
* Tests the JobPtr to see if it is stopped
* @param job JobPtr to test
* @return Result of test
*/
int isJobStopped(JobPtr job){ return job->running == STOPPED; }

/**
* Tests the JobPtr to see if it is in the background
* @param job JobPtr to test
* @return Result of test
*/
int isJobInBackground(JobPtr job) { return job->background == 1; }

/**
* Tests the JobPtr to see if it is done
* @param job JobPtr to test
* @return Result of test
*/
int isJobDone(JobPtr job) { return job->running == DONE; }

/**
* Creates a new JobPtr and returns it
* @param jobid PID for the new job.
* @param command Command represented by the job.
* @param jobnu Number for the new job.
* @return New JobPtr
*/
JobPtr createJob(int jobid, char *command, int jobnu)
{
	JobPtr newJob = (JobPtr) buddy_malloc (sizeof(Job));
	newJob->jobid = jobid;	
	newJob->jobNu = jobnu;
	newJob->command = (char *) buddy_malloc(sizeof(char)*(strlen(command)+1));
	strcpy(newJob->command, command);
	newJob->status = 0;
	newJob->running = 1;
	newJob->background = 0;
	return newJob;
}

/**
* Returns a string representation of the jobs current status
* @param status Status of the job
* @return String representation of that job
*/
char *getStatusString(int status){
	char *running;
	switch(status){
	        case DONE:
			running = "Done   ";
			break;

		case RUNNING:
		        running = "Running";
		        break;

		case STOPPED:
		        running = "Stopped";    
		        break;

		default:
		        running = "Invalid";
		        break;
	}
	return running;
}

/**
* Prints out a string representation of the JobPtr
* @param node JobPtr to print
* @return String that represents the JobPtr
*/
char *toString(JobPtr node)
{
	char *temp;
	char *running;
	
	running = getStatusString(node->running);
	temp = (char *)buddy_malloc(sizeof(char)*strlen(node->command)+1+MAXPID_DIGITS+4);
	sprintf(temp, "[%i] %s %s\n", node->jobNu, running, node->command);
	return temp;
}

/**
* Deallocs a job
* @param job JobPtr to dealloc
*/
void freeJob(JobPtr job)
{
	if (job == NULL) return;
	buddy_free(job->command);
	buddy_free(job);
}

/**
* Frees all the arguments used to run a job
* @param count Number of arguments to free
* @param args Array of strings to free
* @param bufCopy String Buffer to free. Optional.
*/
void freeArguments(int count, char** args, char* bufCopy){
    int i;
    for(i=0; i< count; i++){
	   buddy_free(args[i]);
    }
    if(bufCopy != NULL) buddy_free(bufCopy);
}

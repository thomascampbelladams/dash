#include "signals.h"

NodePtr node;
static void interrupt();
static void stop();
void (*jumpB)(void);
char *pr;

/**
* Initalizes the signals for the signals sent by ctrl-c and ctrl-z to probably manage jobs and/or to simply ignore the signal
* to keep the dash from exiting on those signals
* @param jb Function pointer to use to jump back to a specific point in the main function loop
*/
void initSignals(void (*jb)(void)){
	signal(SIGINT, interrupt);
	signal(SIGTSTP, stop);
	jumpB = jb;
	node = NULL;
}

/**
* Sets the node to the nod passed in. Used for references in this file.
* @param nodee Node to set.
*/
void setNode(NodePtr nodee){ node = nodee; }

void setPrompt(char* prompts){ pr = prompts; }

/**
* Function to run when the dash receives an interrupt signal. Will either pass the signal to a node set by setNode
* or if no node is set, will inform the user that they pressed ctrl-c.
*/
void interrupt(){
	if(node == NULL){
		printf("\nCaught Control-c\n%s", pr);
		//jumpB();
		return;
	}
	JobPtr job = node->data;
	if(isJobNull(job) || isJobInBackground(job) || isJobStopped(job) || isJobDone(job)){
		//printf("IJN: %d IJIB: %d IJS: %d IJD: %d JNU: %d", isJobNull(job),isJobInBackground(job),isJobStopped(job),isJobDone(job), getpid());
		printf("\nCaught Control-c\n%s", pr);
		printf(pr);
		//jumpB();
	}
	else{
		printf("Killing PID: %d", job->jobid);
		kill(job->jobid, SIGINT);
	}
}

/**
* Function to run when the dash receives a stop signal. Will either pass the signal on to a node set by setNode or 
* , if no node is set, inform the user that they pressed ctrl-z
*/
void stop(int a){
	if(node == NULL){
	        printf("\nCaught Control-z\n%s", pr);
	        //jumpB();
	        return;
	}
	JobPtr job = node->data;
	if(isJobNull(job) || isJobInBackground(job) || isJobStopped(job) || isJobDone(job)){ 
	        printf("\nCaught Control-z\n%s", pr);
		//jumpB();
	}
	else kill(job->jobid, SIGTSTP);
}

/**
* Reassigns the interrupt and stop signals to the functions in this file.
*/
void restoreSignals(){
    signal(SIGINT, interrupt);
	signal(SIGTSTP, stop);
}

/**
* Marks a node as stopped by a signal, informs the user it was stopped, and adds it to a job list in the front.
* Note: This does not use a jump to return to the main function as undefined 
* behavior was observed from the program when it utilized it.
* @param l ListPtr to add the node to
* @param n NodePtr that contains the job that received the stop signal that will be added to the list
*/
void markAsStopped(ListPtr l, NodePtr n){
	JobPtr job = n->data;
	job->running = 2;
	printf("\n[%d] %s %s\n", job->jobNu, "Stopped", job->command);
	addAtFront(l, node);
}

/**
* Marks a node as killed by a signal and informas the user it was killed.
* Note: This does not use a jump to return to the main function as undefined 
* behavior was observed from the program when it utilized it.
* @param n NodePtr that contains the job that received the interrupt signal
*/
void markAsKilled(NodePtr n){
	JobPtr job = n->data;
	printf("\n[%d] %s %s\n", job->jobNu, "Killed", job->command); 
}

/**
* Sets the interrupt and stop signals to be ignored
* This is used to ensure that background processes do not receive signals
* meant for the foreground, and that the shell does not send multiple signals
* to multiple children.
*/
void ignoreSignals(){
	signal(SIGINT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
}

/**
* Continues a job in the foreground of the dash.
* Note: This does not use a jump to return to the main function as undefined 
* behavior was observed from the program when it utilized it.
* @param nodee Node that contains the job to continue
* @param jobList Job List that contains the list of jobs currently managed by the dash
*/
void continueJobInForeground(NodePtr nodee, ListPtr jobList){
	setNode(nodee);
	JobPtr job = nodee->data;
	int pid = job->jobid;
	int status = 0;
	ignoreSignals();
	removeNode(jobList, nodee);
	kill(pid, SIGCONT);
	if((pid=waitpid(pid,&status,WUNTRACED))<0) err_sys("waitpid error");
	restoreSignalsAndDetermineStatus(status, jobList, nodee);
}

/**
* Continues a job in the background of the dash and adds it the front of a job list.
* @param nodee Node that contains the job to continue
* @param jobList Job List that contains the list of jobs currently managed by the dash
*/
void continueJobInBackground(NodePtr nodee, ListPtr jobList){
	JobPtr job = nodee->data;
	int pid = job->jobid;
	job->running = RUNNING;
	printf("\n[%d] %s\n", job->jobNu, job->command);
	kill(pid, SIGCONT);
	restoreSignals();
	//if(nodee != NULL) freeNode(nodee);
	jumpB();
}

/**
* Resubscribes the interrupt and stop signals to the functions in this file,
* and marks jubs as stopped or killed depending on their status
* @param status Status of the child process that exited
* @param jobList List of jobs managed by the dash
* @param n NodePtr that contains the job that exited
*/
void restoreSignalsAndDetermineStatus(int status, ListPtr jobList, NodePtr n){
	restoreSignals();
	if(WIFSIGNALED(status) || WIFSTOPPED(status)){
		if(WIFSTOPPED(status)) markAsStopped(jobList, n);
		else markAsKilled(n);
	}
	else
	{
		//if(n != NULL) freeNode(n);
	}
}

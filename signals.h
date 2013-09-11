#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "ourhdr.h"
#include "Job.h"
#include "Node.h"
#include "List.h"

void initSignals(void (*jb) (void));
void setNode(NodePtr node);
void markAsStopped(ListPtr l, NodePtr n);
void markAsKilled(NodePtr n);
void ignoreSignals();
void restoreSignals();
void continueJobInForeground(NodePtr nodee, ListPtr jobList);
void restoreSignalsAndDetermineStatus(int status, ListPtr jobList, NodePtr n);
void continueJobInBackground(NodePtr nodee, ListPtr jobList);
void setPrompt(char* prompt);

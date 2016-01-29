#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ipc.h>
#include <time.h>

struct msgbuf{
	long type;
	int cmd;
	char nick[10];
	char text[256];
	char date[30];
	int pid;
	int status;
};

struct users{
	char nick[10];
	int pid;
};

struct group{
	char group_name[10];
	int users_pids[10];
};
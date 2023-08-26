#include <unistd.h>
#include <stdio.h>

#define NAMED_PIPE "/var/lock/pipename"

struct message {
	int pid;
	int counter;
}

int main(){
	int numProcesses = 15;

	/*
	upon receiving no message
		send uid to left

	upon receiving message from right
	case:
		m.uid > uid:
			send m to left
		m.uid < uid:
			discard m
		m.uid = uid:
			leader := i
			send <terminate, i> to left
			terminate
		endcase

	upon receiving <terminate, i> from right
		leader := i
		send <terminate, i> to left
		terminate
	*/

	// create the named pipe (fifo) with permission
    int ret = mkfifo(NAMED_PIPE, 0666);
    if (ret < 0)
        printf("Error when creating FIFO. %s\n", strerror(errno));

	for (int processIdx = 0; processIdx < numProcesses; processIdx++){
		pid = fork();
	}

	if (pid < 0) {
		perror('Fork failed\n');
		return pid;
	}

	if (pid == 0){ // child
	}
}
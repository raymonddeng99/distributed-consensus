


int main(){
	int numProcesses = 15;

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
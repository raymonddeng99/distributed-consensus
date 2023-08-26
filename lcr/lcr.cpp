


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
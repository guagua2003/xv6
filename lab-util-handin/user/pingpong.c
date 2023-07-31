#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    int parentToChild[2], childToParent[2]; 
    char byte; 

    if (pipe(parentToChild) < 0 || pipe(childToParent) < 0) {
        printf("pipe error\n");
        exit(1);
    }

    int pid = fork();
    if (pid < 0) {
        printf("fork error\n");
        exit(1);
    } 
	else if (pid == 0) {
        close(parentToChild[1]);
        close(childToParent[0]);

        read(parentToChild[0], &byte, sizeof(byte));
		printf("%d: received ping\n", getpid());

        write(childToParent[1], &byte, sizeof(byte));

        close(parentToChild[0]);
        close(childToParent[1]);
    } 
	else {
        close(parentToChild[0]);
        close(childToParent[1]);

		byte = 'A';
        write(parentToChild[1], &byte, sizeof(byte));
        wait(&pid);

        read(childToParent[0], &byte, sizeof(byte));
        printf("%d: received pong\n", getpid());

        close(parentToChild[1]);
        close(childToParent[0]);        
    }
    
    exit(0);
}


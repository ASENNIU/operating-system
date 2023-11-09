#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int p[2];
char buf[512];

int main(int argc, char* argv[])
{
    if (pipe(p) < 0) {
        fprintf(2, "Pipe failture...\n");
	exit(1);
    }

    int pid = fork();
    if (pid < 0) {
    	fprintf(2, "Fork failture...\n");
    }

    if (pid == 0){
	read(p[0], buf, 1);
	int cur_pid = getpid();
	printf("%d: received ping\n", cur_pid);
	write(p[1], buf, 1);
	exit(0);	
    } else {
   	int cur_pid = getpid();
	buf[0] = 'a';
	write(p[1], buf, 1);
	read(p[0], buf, 1);
	printf("%d: received pong\n", cur_pid);
	wait(0);
	exit(0);
    }
}

#include "kernel/types.h"
#include "user/user.h"
#include <stdbool.h>

void handle_data(int listenfd)
{
    int passed_data;
    int cur_prime;
    bool is_forked = false;
    int p[2];
    read(listenfd, &cur_prime, 4);
    printf("prime %d\n", cur_prime);
    while (true) {
   	int ret = read(listenfd, &passed_data, 4);
	if (ret == 0) {
	    close(listenfd);
	    if (is_forked) {
		close(p[1]);    
	        wait(0);   
	    }
	    exit(0);
	}
	int res = passed_data % cur_prime;
	if (res != 0) {
	    if (!is_forked) {
	        pipe(p);
		is_forked = true;
		int pid = fork();
		if (pid == 0) {
		    close(p[1]);
		    close(listenfd);
		    handle_data(p[0]);
		} else {
		    close(p[0]);
		}
	    }
	    write(p[1], &passed_data, 4);
	}
    }
}

int main()
{
    int p[2];
    pipe(p);
    int pid = fork();
    if (pid == 0) {
	close(p[1]);
        handle_data(p[0]);
    } else {
        int prime = 2;
        close(p[0]);
        write(p[1], &prime, 4);
	for (int i = 3; i <= 35; ++i) {
	    write(p[1], &i, 4);
	}
	close(p[1]);
	wait(0);
	exit(0);
    }
    return 0;
}

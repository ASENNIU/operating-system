#include "kernel/param.h"
#include "kernel/types.h"
#include "user/user.h"

#define BUF_SIZE 512

int main(int argc, char* argv[])
{
    char buf[BUF_SIZE + 1] = {0};
    char x_buf[BUF_SIZE + 1] = {0};
    char* xargv[MAXARG] = {0};
    for (int i = 1; i < argc; ++i) {
   	xargv[i-1] = argv[i];
    }
    int cur_buf_length = 0;
    int ret = 0;
    int is_end = 0;
    

    while (!(is_end && cur_buf_length == 0)) {
	if (!is_end) {
   	    ret = read(0, buf + cur_buf_length, BUF_SIZE - cur_buf_length);
    	    if (ret < 0) {
   	        fprintf(2, "xargs: read returns -1 error...\n");
		exit(1);
   	    }	
    	    if (ret == 0) {
   	       close(0);
	       is_end = 1;
    	    }
    	    cur_buf_length += ret;
	}
        char* line_off = strchr(buf, '\n');
        while (line_off != 0) {
	    memcpy(x_buf, buf, line_off - buf);
	    xargv[argc - 1] = x_buf;
            int pid = fork();
	    if (pid == 0) {
	        close(0);
	        if (exec(argv[1], xargv) < 0) {
	   	    fprintf(2, "xargs: exec fails with -1...\n");
		    exit(1);
	        }
	    } else {
		memset(x_buf, 0, line_off - buf);
		memmove(buf, line_off + 1, cur_buf_length - (line_off - buf) - 1);
		cur_buf_length -= line_off - buf + 1;
		memset(buf + cur_buf_length, 0, BUF_SIZE - cur_buf_length);
	        wait(0);
		line_off = strchr(buf, '\n');
	    }
    	}
    }
    exit(0);
}

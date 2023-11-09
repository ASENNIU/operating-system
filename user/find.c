#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


char *basename(char *pathname) {
  char *prev = 0;
  char *curr = strchr(pathname, '/');
  while (curr != 0) {
    prev = curr;
    curr = strchr(curr + 1, '/');
  }
  return prev + 1;
}

void find(char* path, char* filename) {
    char buf[512], *base_name, *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0) {
	fprintf(2, "find: cannot open %s\n", path);
	return;	
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
	close(fd);
	return;
    }

    switch (st.type) {
   	case T_FILE:
	    base_name =  basename(path);
	    if (strcmp(filename, base_name) == 0) {
	   	printf("%s\n", path);
	    }
	    break;
	case T_DIR:
	    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
	    	printf("find: path too long\n");
		break;
	    }
	 
	
	    strcpy(buf, path);
	    p = buf + strlen(buf);
	    *p++ = '/';
	    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
	    	if (de.inum == 0 || 
		    strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
		    continue;
		
		memmove(p, de.name, DIRSIZ);
		p[DIRSIZ] = 0;
		find(buf, filename);
	    }
	    break;
    }
    close(fd);
}

int main(int argc, char* argv[])
{
    if (argc != 3) {
    	fprintf(2, "Usage: find [directory] [filename]\n");
	exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}

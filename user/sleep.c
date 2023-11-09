#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(int argc, char* argv[])
{
    if (argc <= 1) {
        fprintf(2, "Usage: time tick to sleep...");
        exit(1);
    }

    int time_tick = atoi(argv[1]);
    sleep(time_tick);
    exit(0);
                        
}



#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char** argv)
{
    if(4 != argc)
    {
        printf("[Arguments ERROR!]\n");
        printf("\tUsage:\n");
        printf("\t\t%s <Target_PID> <Signal_Number> <content>\n", argv[0]);
        return -1;
    }
    int pid = atoi(argv[1]);
    int sig = atoi(argv[2]);

    if(pid > 0 && sig > 0)
    {
        //int sigqueue(pid_t pid, int sig, const union sigval value);
        union sigval val;
        val.sival_int = atoi(argv[3]);
        printf("send: %d\n", atoi(argv[3]));
        sigqueue(pid, sig, val);
    }
    else
    {
        printf("Target_PID or Signal_Number MUST bigger than 0!\n");
    }
    
    return 0;
}

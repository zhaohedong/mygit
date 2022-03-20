#include<signal.h>
#include<stdio.h>
#include <unistd.h>

//void (*sa_sigaction)(int, siginfo_t *, void *);
void handler(int signum, siginfo_t * info, void * context)
{
    if(signum == SIGIO)
        printf("SIGIO   signal: %d\n", signum);
    else if(signum == SIGUSR1)
        printf("SIGUSR1   signal: %d\n", signum);
    else
        printf("error\n");
    
    if(context)
    {
        printf("content: %d\n", info->si_int);
        printf("content: %d\n", info->si_value.sival_int);
    }
}

int main(void)
{
    //int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
    struct sigaction act;
    
    /*
     struct sigaction {
     void     (*sa_handler)(int);
     void     (*sa_sigaction)(int, siginfo_t *, void *);
     sigset_t   sa_mask;
     int        sa_flags;
     };
     */

    sigset_t s;
    sigemptyset(&s);
    sigaddset(&s, SIGINT);
    //阻塞s中的有效信号：即 SIGINT
    sigprocmask(SIG_BLOCK, &s, NULL);

    act.sa_sigaction = handler;
    act.sa_flags = SA_SIGINFO;
    
    sigaction(SIGIO, &act, NULL);
    sigaction(SIGUSR1, &act, NULL);
    for(;;)
    {
        sleep(10000);
    }
    return 0;
}
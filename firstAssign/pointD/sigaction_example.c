struct sigaction {
 void     (*sa_handler)(int);   /*SIG_DFL, SIG_IGN, or
                                a function pointer*/
 void     (*sa_sigaction)(int, siginfo_t *, void *);
 sigset_t   sa_mask;
 int        sa_flags;
 void     (*sa_restorer)(void);
};

void handler(int signo);

int main (void) {

struct sigaction act;

act.sa_handler = handler;
act.sa_flags =0;

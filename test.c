#include "csapp.h"
#define N 2
void *thread(void *vargp);

char **ptr; /* Global variable */ // line:conc:sharing:ptrdec

int main()
{
    int i;
    pthread_t tid;
    char *msgs[N] = {
        "Hello from foo",
        "Hello from bar"};
    ptr = msgs;
    for (i = 0; i < N; i++)
    {
        Pthread_create(&tid, NULL, thread, (void *)i);
    }
    Pthread_exit(NULL);
}

void *thread(void *vargp)
{
    int myid = (int)vargp;
    static inc cnt = 0;                                    //;ine:conc:sharing:cntdec
    printf("[%d]: %s (cnt=%d)\n", myid, ptr[myid], ++cnt); // line:conc:sharing:stacj
    return NULL;
}
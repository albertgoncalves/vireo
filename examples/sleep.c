#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include <vireo.h>

void
sleep_(int seconds)
{
    struct timespec time = {0};
    clock_gettime(CLOCK_MONOTONIC, &time);
    time.tv_sec += seconds;

    int result;
    do {
        result = clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &time, NULL);
    } while (result != 0);
}

typedef struct {
    int seconds;
    int who;
} Arg;

void
child(void* args)
{
    int seconds = ((Arg*)args)->seconds;
    int who = ((Arg*)args)->who;
    printf("   - child [%d] thread sleeping for %d seconds\n", who, seconds);
    sleep_(seconds);
    printf("   - child [%d] thread has awoken\n", who);
}

#define N 3

void
umain(void* _ __attribute__((unused)))
{
    int envs[N];
    Arg args[N];

    printf(" - creating child threads\n");
    for (int i = 0; i < N; ++i) {
        envs[i] = vireo_create(child, (void*)&args[i]);
        args[i].seconds = N - i;
        args[i].who = envs[i];
        vireo_yield();
    }

    int seconds = N + 1;
    printf(" - parent thread sleeping for %d seconds\n", seconds);
    sleep_(seconds);

    printf(" - destroying child threads\n");
    for (int i = 0; i < N; ++i) {
        vireo_destroy(envs[i]);
    }
}

#include <stdio.h>

#include <vireo.h>

static void
yield_thread(void* _ __attribute__((unused)))
{
	int i;

	printf("Hello, I am environment %08x.\n", vireo_getid());
	for (i = 0; i < 5; i++) {
		vireo_yield();
		printf("Back in environment %08x, iteration %d.\n",
		       vireo_getid(), i);
	}
	printf("All done in environment %08x.\n", vireo_getid());
}

void
umain(void* _ __attribute__((unused)))
{
	int i;
	for (i = 0; i < 3; i++) {
		vireo_create(yield_thread, NULL);
	}
	vireo_exit();
}

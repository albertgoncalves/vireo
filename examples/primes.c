#include <stdio.h>
#include <stdlib.h>

#include <vireo.h>

void
primeproc(void* _ __attribute__((unused)))
{
	int i, id, p;

	// fetch a prime from our left neighbor
	p = vireo_recv(NULL);
	printf("%d ", p);

	// fork a right neighbor to continue the chain
	id = vireo_create(primeproc, NULL);
	if (id == -1)
		exit(0);

	// filter out multiples of our prime
	while (1) {
		i = vireo_recv(NULL);
		if (i % p)
			vireo_send(id, i);
	}
}

void
umain(void* _ __attribute__((unused)))
{
	int i, id;
	// fork the first prime procss in the chain
	id = vireo_create(primeproc, NULL);

	// feed all the integers through
	for (i = 2; ; i++)
		vireo_send(id, i);
}

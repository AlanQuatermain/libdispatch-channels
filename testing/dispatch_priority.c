#include <stdio.h>
#include <dispatch/dispatch.h>
#include <dispatch/queue_private.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <TargetConditionals.h>

#include "dispatch_test.h"

int done = 0;

#define BLOCKS 128
#define PRIORITIES 3

#if TARGET_OS_EMBEDDED
#define LOOP_COUNT 2000000
#else
#define LOOP_COUNT 100000000
#endif

char *labels[PRIORITIES] = { "LOW", "DEFAULT", "HIGH" };
int priorities[PRIORITIES] = { DISPATCH_QUEUE_PRIORITY_LOW, DISPATCH_QUEUE_PRIORITY_DEFAULT, DISPATCH_QUEUE_PRIORITY_HIGH };

union {
	size_t count;
	char padding[64];
} counts[PRIORITIES];

#define ITERATIONS (size_t)(PRIORITIES * BLOCKS * 0.50)
size_t iterations = ITERATIONS;

void
histogram(void) {
	size_t maxcount = BLOCKS;
	size_t sc[PRIORITIES];
	
	size_t total = 0;
	
	size_t x,y;
	for (y = 0; y < PRIORITIES; ++y) {
		sc[y] = counts[y].count;
	}

	for (y = 0; y < PRIORITIES; ++y) {
		printf("%s: %ld\n", labels[y], sc[y]);
		total += sc[y];
		
		double fraction = (double)sc[y] / (double)maxcount;
		double value = fraction * (double)80;
		for (x = 0; x < 80; ++x) {
			printf("%s", (value > x) ? "*" : " ");
		}
		printf("\n");
	}
	
	test_long("blocks completed", total, ITERATIONS);
	test_long_less_than("high priority precedence", (long)sc[0], (long)sc[2]);
}

void
cpubusy(void* context)
{
	size_t *count = context;
	size_t iterdone;

	size_t idx;
	for (idx = 0; idx < LOOP_COUNT; ++idx) {
		if (done) break;
	}
	
	if ((iterdone = __sync_sub_and_fetch(&iterations, 1)) == 0) {
		__sync_add_and_fetch(&done, 1);
		__sync_add_and_fetch(count, 1);
		histogram();
		test_stop();
		exit(0);
	} else if (iterdone > 0) {
		__sync_add_and_fetch(count, 1);
	}
}

void
submit_work(dispatch_queue_t queue, void* context)
{
	int i;

	for (i = 0; i < BLOCKS; ++i) {
		dispatch_async_f(queue, context, cpubusy);
	}

#if USE_SET_TARGET_QUEUE
	dispatch_release(queue);
#endif
}

int
main(int argc __attribute__((unused)), char* argv[] __attribute__((unused)))
{
	dispatch_queue_t q[PRIORITIES];
	int i;

#if USE_SET_TARGET_QUEUE
	test_start("Dispatch Priority (Set Target Queue)");
	for(i = 0; i < PRIORITIES; i++) {
		q[i] = dispatch_queue_create(labels[i], NULL);
		test_ptr_notnull("q[i]", q[i]);
		assert(q[i]);
		dispatch_set_target_queue(q[i], dispatch_get_global_queue(priorities[i], 0));
		dispatch_queue_set_width(q[i], DISPATCH_QUEUE_WIDTH_MAX_LOGICAL_CPUS);
	}
#else
	test_start("Dispatch Priority");
	for(i = 0; i < PRIORITIES; i++) {
		q[i] = dispatch_get_global_queue(priorities[i], 0);
	}
#endif
	
	for(i = 0; i < PRIORITIES; i++) {
		submit_work(q[i], &counts[i].count);
	}

	dispatch_main();

	return 0;
}

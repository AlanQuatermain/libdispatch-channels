#include <dispatch/dispatch.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <libkern/OSAtomic.h>

#include "dispatch_test.h"
#include <Block.h>

void done(void *arg __unused) {
    sleep(1);
    test_stop();
}

int
main(void)
{
    __block dispatch_time_t time_a_min, time_a, time_a_max;
    __block dispatch_time_t time_b_min, time_b, time_b_max;
    __block dispatch_time_t time_c_min, time_c, time_c_max;


	test_start("Dispatch After");

    dispatch_async(dispatch_get_main_queue(), ^{
            time_a_min = dispatch_time(0,  5.5*NSEC_PER_SEC);
            time_a     = dispatch_time(0,   6*NSEC_PER_SEC);
            time_a_max = dispatch_time(0,  6.5*NSEC_PER_SEC);
            dispatch_after(time_a, dispatch_get_current_queue(), ^{
                    dispatch_time_t now_a = dispatch_time(0, 0);
                    test_long_less_than("can't finish faster than 5.5s", 0, now_a - time_a_min);
                    test_long_less_than("must finish faster than  6.5s", 0, time_a_max - now_a);
                    
                    time_b_min = dispatch_time(0,  1.5*NSEC_PER_SEC);
                    time_b     = dispatch_time(0,    2*NSEC_PER_SEC);
                    time_b_max = dispatch_time(0,  2.5*NSEC_PER_SEC);
                    dispatch_after(time_b, dispatch_get_current_queue(), ^{
                            dispatch_time_t now_b = dispatch_time(0, 0);
                            test_long_less_than("can't finish faster than 1.5s", 0, now_b - time_b_min);
                            test_long_less_than("must finish faster than  2.5s", 0, time_b_max - now_b);
                            
                            time_c_min = dispatch_time(0,  0*NSEC_PER_SEC);
                            time_c     = dispatch_time(0,  0*NSEC_PER_SEC);
                            time_c_max = dispatch_time(0,  .5*NSEC_PER_SEC);
                            dispatch_after(time_c, dispatch_get_current_queue(), ^{
                                    dispatch_time_t now_c = dispatch_time(0, 0);
                                    test_long_less_than("can't finish faster than 0s", 0, now_c - time_c_min);
                                    test_long_less_than("must finish faster than .5s", 0, time_c_max - now_c);

                                    dispatch_async_f(dispatch_get_current_queue(), NULL, done);
                                });
                        });
                });
        });
    
    dispatch_main();
    return 0;
}

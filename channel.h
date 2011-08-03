//
//  channel.h
//  libdispatch
//
//  Created by Jim Dovey on 11-08-03.
//  Copyright 2011 Jim Dovey. All rights reserved.
//

#ifndef __DISPATCH_CHANNEL__
#define __DISPATCH_CHANNEL__

#ifndef __DISPATCH_INDIRECT__
#error "Please #include <dispatch/dispatch.h> instead of this file directly."
#include <dispatch/base.h> // for HeaderDoc
#endif

DISPATCH_DECL(dispatch_channel);
DISPATCH_DECL(dispatch_channel_attr);

__BEGIN_DECLS

__OSX_AVAILABLE_STARTING(__MAC_10_7,__IPHONE_5_0)
DISPATCH_MALLOC DISPATCH_WARN_RESULT DISPATCH_NOTHROW
dispatch_channel_t
dispatch_channel_create(const char *label, dispatch_channel_attr_t attr);

enum {
	DISPATCH_CHANNEL_COPY_DATA,
	DISPATCH_CHANNEL_REFERENCE_DATA
};

__OSX_AVAILABLE_STARTING(__MAC_10_7,__IPHONE_5_0)
DISPATCH_MALLOC DISPATCH_WARN_RESULT DISPATCH_NOTHROW
dispatch_channel_attr_t
dispatch_channel_attr_create(const char *typelabel, size_t size, long reftype);

__OSX_AVAILABLE_STARTING(__MAC_10_7,__IPHONE_5_0)
DISPATCH_NONNULL_ALL DISPATCH_PURE DISPATCH_WARN_RESULT DISPATCH_NOTHROW
const char *
dispatch_channel_get_label(dispatch_channel_t channel);

__OSX_AVAILABLE_STARTING(__MAC_10_7,__IPHONE_5_0)
DISPATCH_NONNULL_ALL DISPATCH_NOTHROW
void
dispatch_channel_cancel(dispatch_channel_t channel);

__OSX_AVAILABLE_STARTING(__MAC_10_7,__IPHONE_5_0)
DISPATCH_NONNULL_ALL DISPATCH_NOTHROW
long
dispatch_channel_testcancel(dispatch_channel_t channel);

__OSX_AVAILABLE_STARTING(__MAC_10_7,__IPHONE_5_0)
DISPATCH_NONNULL1 DISPATCH_NOTHROW
long
dispatch_channel_wait(dispatch_channel_t channel, dispatch_time_t timeout);

#if __BLOCKS__
__OSX_AVAILABLE_STARTING(__MAC_10_7,__IPHONE_5_0)
DISPATCH_NONNULL1 DISPATCH_NOTHROW
void
dispatch_channel_set_read_handler(dispatch_channel_t channel, dispatch_queue_t queue, dispatch_block_t read_handler);
#endif

__OSX_AVAILABLE_STARTING(__MAC_10_7,__IPHONE_5_0)
DISPATCH_NONNULL_ALL DISPATCH_NOTHROW
void
dispatch_channel_set_read_handler_f(dispatch_channel_t channel, dispatch_queue_t queue, dispatch_function_t read_handler);

#if __BLOCKS__
__OSX_AVAILABLE_STARTING(__MAC_10_7,__IPHONE_5_0)
DISPATCH_NONNULL1 DISPATCH_NOTHROW
void
dispatch_channel_set_cancel_handler(dispatch_channel_t channel, dispatch_block_t cancel_handler);
#endif

__OSX_AVAILABLE_STARTING(__MAC_10_7,__IPHONE_5_0)
DISPATCH_NONNULL1 DISPATCH_NOTHROW
void
dispatch_channel_set_cancel_handler_f(dispatch_channel_t channel, dispatch_function_t cancel_handler);

__OSX_AVAILABLE_STARTING(__MAC_10_7,__IPHONE_5_0)
DISPATCH_NONNULL1 DISPATCH_NOTHROW
void
dispatch_channel_write_data(dispatch_channel_t channel, void *dataptr);

__END_DECLS

#endif	/* __DISPATCH_CHANNEL__ */

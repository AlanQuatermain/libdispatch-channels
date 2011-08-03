//
//  channel_internal.h
//  libdispatch
//
//  Created by Jim Dovey on 11-08-03.
//  Copyright 2011 Jim Dovey. All rights reserved.
//

#ifndef __DISPATCH_CHANNEL_INTERNAL__
#define __DISPATCH_CHANNEL_INTERNAL__

#ifndef __DISPATCH_INDIRECT__
#error "Please #include <dispatch/dispatch.h> instead of this file directly."
#include <dispatch/base.h> // for HeaderDoc
#endif

#include "source_internal.h"

struct dispatch_channel_vtable_s {
	DISPATCH_VTABLE_HEADER(dispatch_channel_s);
};

extern const struct dispatch_channel_vtable_s _dispatch_channel_kevent_vtable;

// dispatch_kevent_t comes from source_internal.h

#define DCF_CANCELLED 1u	// cancellation has been requested

struct dispatch_channel_s {
	DISPATCH_STRUCT_HEADER(dispatch_channel_s, dispatch_channel_vtable_s);
	DISPATCH_QUEUE_HEADER;
	// Instruments always copies DISPATCH_QUEUE_MIN_LABEL_SIZE, which is 64,
	// so the remainder of the structure must be big enough
	union {
		char _ds_pad[DISPATCH_QUEUE_MIN_LABEL_SIZE];
		struct {
			char dq_label[8];
			dispatch_kevent_t dc_dkev;
			dispatch_channel_attr_t attr;
			
			dispatch_function_t dc_handler_func;
			void *dc_handler_ctxt;
			
			void *dc_cancel_handler;
			
			unsigned int dc_is_installed:1,
			dc_is_armed:1,
			dc_cancel_is_block:1,
			dc_handler_is_block:1;
			
			unsigned int dc_atomic_flags;
			
			TAILQ_ENTRY(dispatch_channel_s) dc_list;
		};
	};
};

struct dispatch_channel_attr_vtable_s {
	DISPATCH_VTABLE_HEADER(dispatch_channel_attr_s);
};

struct dispatch_channel_attr_s {
	DISPATCH_STRUCT_HEADER(dispatch_channel_attr_s, dispatch_channel_attr_vtable_s);
	size_t data_size;
};

#endif	/* __DISPATCH_CHANNEL_INTERNAL__ */

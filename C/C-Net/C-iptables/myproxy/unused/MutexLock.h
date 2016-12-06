//
//  MutexLock.h
//  Proxy Server
//
//  Created by Matt Zanchelli on 2/27/14.
//  Copyright (c) 2014 Matt Zanchelli. All rights reserved.
//

#ifndef Proxy_Server_MutexLock_h
#define Proxy_Server_MutexLock_h

#include <pthread.h>

/// Execute this code with mutual exclusivity.
/// @param X The code to execute.
#define SYNCHRONOUS_BLOCK(X) \
{ \
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; \
	pthread_mutex_lock(&mutex); \
    { \
    	X; \
    } \
	pthread_mutex_unlock(&mutex); \
}

#endif

//
//  Statistics.c
//  Proxy Server
//
//  Created by Matt Zanchelli on 3/10/14.
//  Copyright (c) 2014 Matt Zanchelli. All rights reserved.
//

#include <stdio.h>

#include "Statistics.h"

#include "MutexLock.h"

void incrementNumberOfSuccessfulRequests()
{
	SYNCHRONOUS_BLOCK((*numberOfSuccessfulRequests)++);
}

void incrementNumberOfFilteredRequests()
{
	SYNCHRONOUS_BLOCK((*numberOfFilteredRequests)++);
}

void incrementNumberOfErroredRequests()
{
	SYNCHRONOUS_BLOCK((*numberOfErroredRequests)++);
}

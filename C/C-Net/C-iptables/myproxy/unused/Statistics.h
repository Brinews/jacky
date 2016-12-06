//
//  Statistics.h
//  Proxy Server
//
//  Created by Matt Zanchelli on 3/10/14.
//  Copyright (c) 2014 Matt Zanchelli. All rights reserved.
//

#ifndef Proxy_Server_Statistics_h
#define Proxy_Server_Statistics_h

/// The total number of successful requests.
unsigned int *numberOfSuccessfulRequests;

/// Increment @c numberOfSuccessfulRequests atomically.
void incrementNumberOfSuccessfulRequests();

/// The total number of filtered requests.
unsigned int *numberOfFilteredRequests;

/// Increment @c numberOfFilteredRequests atomically.
void incrementNumberOfFilteredRequests();

/// The total number of errored requests.
unsigned int *numberOfErroredRequests;

/// Increment @c numberOfErroredRequests atomically.
void incrementNumberOfErroredRequests();

#endif

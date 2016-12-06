//
//  SignalHandling.c
//  Proxy Server
//
//  Created by Matt Zanchelli on 2/20/14.
//  Copyright (c) 2014 Matt Zanchelli. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "SignalHandling.h"

#include "Filters.h"
#include "Statistics.h"

/*
 When your server receives a SIGUSR1 signal, your server should output a summary of what
 prefixes/suffixes it is filtering, as well as the set of statistics shown in the example below:
 
 bash$ ./proxy 8888 doubleclick.com slashdot.org
 monica.cs.rpi.edu: GET http://www.cs.rpi.edu/
 monica.cs.rpi.edu: GET http://www.cs.rpi.edu/gfx/backg5.jpg
 monica.cs.rpi.edu: GET http://www.cs.rpi.edu/gfx/logo.jpg
 monica.cs.rpi.edu: GET http://www.slashdot.org/foo/blah [FILTERED]
 monica.cs.rpi.edu: GET http://www.yahoo.com/images/annakornikova.jpg
 monica.cs.rpi.edu: GET http://fred.com/purchase.asp?prod=17&qty=102
 monica.cs.rpi.edu: HEAD http://www.slashdot.org/ [FILTERED]
 monica.cs.rpi.edu: POST http://www.fbi.gov/insecuresubmission.cgi
 Received SIGUSR1...reporting status:
 -- Processed 6 requests successfully
 -- Filtering: doubleclick.com; slashdot.org
 -- Filtered 2 requests
 -- Encountered 0 requests in error
 
 As shown above, display the number of requests successfully processed, meaning the number of
 requests that were not filtered and successfully pass through the proxy server. Also show the
 number of requests filtered, as well as the number of requests for which your proxy server
 responded with an error (e.g. HTTP error 403).
 */
void handleSIGUSR1()
{	
	// Received signal.
	printf("Received SIGUSR1...reporting status:\n");
	
	// Report the number of requests.
	if ( *numberOfSuccessfulRequests == 1 ) {
		printf("-- Processed %u request successfully.\n", *numberOfSuccessfulRequests);
	} else {
		printf("-- Processed %u requests successfully.\n", *numberOfSuccessfulRequests);
	}
	
	// Report the filters being used.
	printf("-- Filtering:");
	for ( unsigned int i=0; i<*filtersCount; ++i ) {
		printf(" %s;", filters[i]);
	}
	printf("\n");
	
	// Report the number of filtered requests.
	if ( *numberOfFilteredRequests == 1 ) {
		printf("-- Filtered %u request.\n", *numberOfFilteredRequests);
	} else {
		printf("-- Filtered %u requests.\n", *numberOfFilteredRequests);
	}
	
	// Report the number of requests that resulted in errors.
	if ( *numberOfErroredRequests == 1 ) {
		printf("-- Encountered %u request in error\n", *numberOfErroredRequests);
	} else {
		printf("-- Encountered %u requests in error\n", *numberOfErroredRequests);
	}
}

/*
 When your server receives a SIGUSR2 signal, your server should exit gracefully.
 Therefore, to shutdown your server, you would need to send it a signal via the shell as follows
 (assuming the server is process id 5555):
 
 bash$ kill -12 5555
 */
void handleSIGUSR2()
{
	printf("Received SIGUSR2...");
	exit(1);
}

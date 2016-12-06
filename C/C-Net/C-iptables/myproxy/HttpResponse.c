#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "HttpResponse.h"

#include "StringHelper.h"

HTTPResponse HTTPResponseCreate()
{
	return (HTTPResponse) malloc(sizeof(char *) * HTTPResponseHeaderFieldsCount);
}

void HTTPResponseFree(HTTPResponse response)
{
	for ( int i=0; i<HTTPResponseHeaderFieldsCount; ++i ) {
		if ( response[i] != NULL ) {
			free(response[i]);
		}
	}
	
	free(response);
}

HTTPResponseHeaderField HTTPResponseHeaderFieldForFieldNamed(HTTPResponseHeaderFieldName fieldName)
{
	if ( stringEquality(fieldName, HTTPResponseHeaderFieldName_Access_Control_Allow_Origin) ) {
		return HTTPResponseHeaderField_Access_Control_Allow_Origin;
	} else if ( stringEquality(fieldName, HTTPResponseHeaderFieldName_Accept_Ranges) ) {
		return HTTPResponseHeaderField_Accept_Ranges;
	} else if ( stringEquality(fieldName, HTTPResponseHeaderFieldName_Age) ) {
		return HTTPResponseHeaderField_Age;
	} else if ( stringEquality(fieldName, HTTPResponseHeaderFieldName_Allow) ) {
		return HTTPResponseHeaderField_Allow;
	} else if ( stringEquality(fieldName, HTTPResponseHeaderFieldName_Cache_Control) ) {
		return HTTPResponseHeaderField_Cache_Control;
	} else if ( stringEquality(fieldName, HTTPResponseHeaderFieldName_Connection) ) {
		return HTTPResponseHeaderField_Connection;
	} else if ( stringEquality(fieldName, HTTPResponseHeaderFieldName_Content_Encoding) ) {
		return HTTPResponseHeaderField_Content_Encoding;
	} else if ( stringEquality(fieldName, HTTPResponseHeaderFieldName_Content_Language) ) {
		return HTTPResponseHeaderField_Content_Language;
	} else if ( stringEquality(fieldName, HTTPResponseHeaderFieldName_Content_Length) ) {
		return HTTPResponseHeaderField_Content_Length;
	} else if ( stringEquality(fieldName, HTTPResponseHeaderFieldName_Content_Location) ) {
		return HTTPResponseHeaderField_Content_Location;
	} else if ( stringEquality(fieldName, HTTPResponseHeaderFieldName_Content_Type) ) {
		return HTTPResponseHeaderField_Content_Type;
	} else if ( stringEquality(fieldName, HTTPResponseHeaderFieldName_Date) ) {
		return HTTPResponseHeaderField_Date;
	} else if ( stringEquality(fieldName, HTTPResponseHeaderFieldName_ETag) ) {
		return HTTPResponseHeaderField_ETag;
	} else if ( stringEquality(fieldName, HTTPResponseHeaderFieldName_Expires) ) {
		return HTTPResponseHeaderField_Expires;
	} else if ( stringEquality(fieldName, HTTPResponseHeaderFieldName_Last_Modified) ) {
		return HTTPResponseHeaderField_Last_Modified;
	} else if ( stringEquality(fieldName, HTTPResponseHeaderFieldName_Link) ) {
		return HTTPResponseHeaderField_Link;
	} else if ( stringEquality(fieldName, HTTPResponseHeaderFieldName_Location) ) {
		return HTTPResponseHeaderField_Location;
	} else if ( stringEquality(fieldName, HTTPResponseHeaderFieldName_P3P) ) {
		return HTTPResponseHeaderField_P3P;
	} else if ( stringEquality(fieldName, HTTPResponseHeaderFieldName_Pragma) ) {
		return HTTPResponseHeaderField_Pragma;
	} else if ( stringEquality(fieldName, HTTPResponseHeaderFieldName_Proxy_Authenticate) ) {
		return HTTPResponseHeaderField_Proxy_Authenticate;
	} else if ( stringEquality(fieldName, HTTPResponseHeaderFieldName_Refresh) ) {
		return HTTPResponseHeaderField_Refresh;
	} else if ( stringEquality(fieldName, HTTPResponseHeaderFieldName_Retry_After) ) {
		return HTTPResponseHeaderField_Retry_After;
	} else if ( stringEquality(fieldName, HTTPResponseHeaderFieldName_Server) ) {
		return HTTPResponseHeaderField_Server;
	} else if ( stringEquality(fieldName, HTTPResponseHeaderFieldName_Set_Cookie) ) {
		return HTTPResponseHeaderField_Set_Cookie;
	} else if ( stringEquality(fieldName, HTTPResponseHeaderFieldName_Status) ) {
		return HTTPResponseHeaderField_Status;
	} else if ( stringEquality(fieldName, HTTPResponseHeaderFieldName_Strict_Transport_Security) ) {
		return HTTPResponseHeaderField_Strict_Transport_Security;
	} else if ( stringEquality(fieldName, HTTPResponseHeaderFieldName_Trailer) ) {
		return HTTPResponseHeaderField_Trailer;
	} else if ( stringEquality(fieldName, HTTPResponseHeaderFieldName_Transfer_Encoding) ) {
		return HTTPResponseHeaderField_Transfer_Encoding;
	} else if ( stringEquality(fieldName, HTTPResponseHeaderFieldName_Upgrade) ) {
		return HTTPResponseHeaderField_Upgrade;
	} else if ( stringEquality(fieldName, HTTPResponseHeaderFieldName_Vary) ) {
		return HTTPResponseHeaderField_Vary;
	} else if ( stringEquality(fieldName, HTTPResponseHeaderFieldName_Via) ) {
		return HTTPResponseHeaderField_Via;
	} else if ( stringEquality(fieldName, HTTPResponseHeaderFieldName_Warning) ) {
		return HTTPResponseHeaderField_Warning;
	} else if ( stringEquality(fieldName, HTTPResponseHeaderFieldName_WWW_Authenticate) ) {
		return HTTPResponseHeaderField_WWW_Authenticate;
	}
	
	// Conversion failed.
	return -1;
}

const char * const statusStringForStatusCode(int statusCode)
{
	switch (statusCode) {
		// 1xx Informational
		case 100: return "Continue";
		case 101: return "Switching Protocols";
		case 102: return "Processing";
		
		// 2xx Success
		case 200: return "OK";
		case 201: return "Created";
		case 202: return "Accepted";
		case 203: return "Non-Authoritative Information";
		case 204: return "No Content";
		case 205: return "Reset Content";
		case 206: return "Partial Content";
		case 207: return "Multi-Status";
		case 208: return "Already Reported";
		case 226: return "IM Used";
		
		// 3xx Redirection
		case 300: return "Multiple Choices";
		case 301: return "Moved Permanently";
		case 302: return "Found";
		case 303: return "See Other";
		case 304: return "Not Modified";
		case 305: return "Use Proxy";
		case 306: return "Switch Proxy";
		case 307: return "Temporary Redirect";
		case 308: return "Permanent Redirect";
		
		// 4xx Client Error
		case 400: return "Bad Request";
		case 401: return "Unauthorized";
		case 402: return "Payment Required";
		case 403: return "Forbidden";
		case 404: return "Not Found";
		case 405: return "Method Not Allowed";
		case 406: return "Not Acceptable";
		case 407: return "Proxy Authentication Required";
		case 408: return "Request Timeout";
		case 409: return "Conflict";
		case 410: return "Gone";
		case 411: return "Length Required";
		case 412: return "Precondition Failed";
		case 413: return "Request Entity Too Large";
		case 414: return "Request-URI Too Long";
		case 415: return "Unsupported Media Type";
		case 416: return "Requested Range Not Satisfiable";
		case 417: return "Expectation Failed";
		case 418: return "I'm a teapot";
		case 419: return "Authentication Timeout";
//		case 420: return "Method Failure"; // Spring Framework
//		case 420: return "Enhance Your Calm"; // Twitter
		case 422: return "Unprocessable Entity";
		case 423: return "Locked";
		case 424: return "Failed Dependency";
//		case 424: return "Method Failure";
		case 425: return "Unordered Collection";
		case 426: return "Upgrade Required";
		case 427: return "Precondition Required";
		case 429: return "Too Many Requests";
		case 431: return "Request Header Fields Too Large";
		case 440: return "Login Timeout"; // Microsoft
		case 444: return "No Response"; // Nginx
		case 449: return "Retry With"; // Microsoft
		case 450: return "Blocked by Windows Parental Controls"; // Microsoft
		case 451: return "Unavailable For Legal Reasons";
//		case 451: return "Recirect"; // Microsoft
		case 494: return "Request Header Too Large"; // Nginx
		case 495: return "Cert Error"; // Nginx
		case 496: return "No Cert"; // Nginx
		case 497: return "HTTP to HTTPS"; // Nginx
		case 499: return "Client Closed Request"; // Nginx
		
		// 5xx Server Error
		case 500: return "Internal Server Error";
		case 501: return "Not Implemented";
		case 502: return "Bad Gateway";
		case 503: return "Service Unavailable";
		case 504: return "Gateway Timeout";
		case 505: return "HTTP Version Not Supported";
		case 506: return "Variant Also Negotiates";
		case 507: return "Insufficient Storage";
		case 508: return "Loop Detected";
		case 509: return "Bandwidth Limit Exceeded";
		case 510: return "Not Extended";
		case 511: return "Network Authentication Required";
		case 520: return "Origin Error";
		case 522: return "Connection timed out";
		case 523: return "Proxy Decined Request";
		case 524: return "A timeout occured";
		case 598: return "Networking read timeout error";
		case 599: return "Network connect timeout error";
		
		default: return NULL;
	}
}

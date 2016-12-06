#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "HttpRequest.h"

#include "StringHelper.h"

HTTPRequest HTTPRequestCreate()
{
	return (HTTPRequest) calloc(HttpReqHeaderFieldsCount, sizeof(char *));
}

void HTTPRequestFree(HTTPRequest request)
{
	for ( int i=0; i<HttpReqHeaderFieldsCount; ++i ) {
		if ( request[i] != NULL ) {
			free(request[i]);
			request[i] = NULL;
		}
	}
	
	free(request);
}

bool validateRequest(HTTPRequest request)
{
	/* Request-Line = Method SP Request-URI SP HTTP-Version CRLF */
	char *Request_Line = request[HttpReqHeaderField_Request_Line];
	
	char *Method = NULL;
	char *MethodArgs = NULL;
	if ( !splitStringAtString(Request_Line, " ", &Method, &MethodArgs) ) {
		return false;
	}
	
	char *Request_URI = NULL;
	char *HTTP_Version = NULL;
	if ( !splitStringAtString(MethodArgs, " ", &Request_URI, &HTTP_Version) ) {
		return false;
	}
	
	if ( stringEquality(HTTP_Version, "HTTP/1.1") ) {
		// HTTP/1.1 is supported.
	} else if ( stringEquality(HTTP_Version, "HTTP/1.0") ) {
		// HTTP/1.0 is supported.
	} else {
		// Unsupported or invalid HTTP-Version.
		return false;
	}
	
	// Must include Host.
	if ( request[HttpReqHeaderField_Host] == NULL ) {
		return false;
	}
	
	// If POST, must include Content-Length.
	if ( stringEquality(Method, "POST") ) {
		if ( request[HttpReqHeaderField_Content_Length] == NULL ) {
			return false;
		}
	}
	
	return true;
}

char *requestStringFromRequest(HTTPRequest request)
{
	char *requestLine = request[HttpReqHeaderField_Request_Line];
	if ( !requestLine ) {
		return NULL;
	}
	
	// Modify request string to remove host from request uri.
	char *modifiedRequestLine = stringDuplicate(requestLine);
	char *URI_Start = strchr(modifiedRequestLine, (int) ' ') + 1;
	char *URI_Base_End = requestLine;

	URI_Base_End = strchr(URI_Base_End, (int) '/') + 1;
	URI_Base_End = strchr(URI_Base_End, (int) '/') + 1;
	URI_Base_End = strchr(URI_Base_End, (int) '/');

	if ( URI_Base_End == NULL ) {
		// Find the second space and then add '/' manually.
		char *secondSpace = strchr(modifiedRequestLine, (int) ' ');
		strcpy(URI_Start, "/");
		URI_Base_End = secondSpace;
	}
	strcpy(URI_Start, URI_Base_End);
	free(requestLine);
	requestLine = modifiedRequestLine;
	request[HttpReqHeaderField_Request_Line] = requestLine;
	
	// Start with the requestLine.
	char *string = stringDuplicate(requestLine);
	
	for ( int field=0; field<HttpReqHeaderFieldsCount; ++field ) {
		// Skip the request line.
		if ( field == HttpReqHeaderField_Request_Line ) {
			continue;
		}
		
		// Get the value of the field.
		char *fieldValue = request[field];
		if ( fieldValue == NULL || strlen(fieldValue) < 1 ) {
			continue;
		}
		
		const char *fieldName = HttpReqHeadFieldForField(field);
		
		unsigned long newLength = strlen(string) + strlen("\r\n") + strlen(fieldName) + strlen(": ") + strlen(fieldValue) + 1;

		char *newString = malloc(sizeof(char) * newLength);
		strcpy(newString, string);
		free(string);
		strcat(newString, "\r\n");
		strcat(newString, fieldName);
		strcat(newString, ": ");
		strcat(newString, fieldValue);
		
		string = newString;
	}
	
	// End the request.
	unsigned long newLength = strlen(string) + strlen("\r\n\r\n") + 1;
	char *newString = malloc(sizeof(char) * newLength);
	strcpy(newString, string);
	strcat(newString, "\r\n\r\n");
	free(string);
	string = newString;
	
	return string;
}

HttpReqHeaderField HttpReqHeaderFieldForFieldNamed(HttpReqHeadField fieldName)
{
	if ( stringEquality(fieldName, HttpReqHeadField_Accept) ) {
		return HttpReqHeaderField_Accept;
	} else if ( stringEquality(fieldName, HttpReqHeadField_Accept_Charset) ) {
		return HttpReqHeaderField_Accept_Charset;
	} else if ( stringEquality(fieldName, HttpReqHeadField_Accept_Encoding) ) {
		return HttpReqHeaderField_Accept_Encoding;
	} else if ( stringEquality(fieldName, HttpReqHeadField_Accept_Language) ) {
		return HttpReqHeaderField_Accept_Language;
	} else if ( stringEquality(fieldName, HttpReqHeadField_Accept_Datetime) ) {
		return HttpReqHeaderField_Accept_Datetime;
	} else if ( stringEquality(fieldName, HttpReqHeadField_Authorization) ) {
		return HttpReqHeaderField_Authorization;
	} else if ( stringEquality(fieldName, HttpReqHeadField_Cache_Control) ) {
		return HttpReqHeaderField_Cache_Control;
	} else if ( stringEquality(fieldName, HttpReqHeadField_Connection) ) {
		return HttpReqHeaderField_Connection;
	} else if ( stringEquality(fieldName, HttpReqHeadField_Proxy_Connection) ) {
		return HttpReqHeaderField_Proxy_Connection;
	} else if ( stringEquality(fieldName, HttpReqHeadField_Cookie) ) {
		return HttpReqHeaderField_Cookie;
	} else if ( stringEquality(fieldName, HttpReqHeadField_Content_Length) ) {
		return HttpReqHeaderField_Content_Length;
	} else if ( stringEquality(fieldName, HttpReqHeadField_Content_MD5) ) {
		return HttpReqHeaderField_Content_MD5;
	} else if ( stringEquality(fieldName, HttpReqHeadField_Content_Type) ) {
		return HttpReqHeaderField_Content_Type;
	} else if ( stringEquality(fieldName, HttpReqHeadField_Date) ) {
		return HttpReqHeaderField_Date;
	} else if ( stringEquality(fieldName, HttpReqHeadField_Expect) ) {
		return HttpReqHeaderField_Expect;
	} else if ( stringEquality(fieldName, HttpReqHeadField_From) ) {
		return HttpReqHeaderField_From;
	} else if ( stringEquality(fieldName, HttpReqHeadField_Host) ) {
		return HttpReqHeaderField_Host;
	} else if ( stringEquality(fieldName, HttpReqHeadField_If_Match) ) {
		return HttpReqHeaderField_If_Match;
	} else if ( stringEquality(fieldName, HttpReqHeadField_If_Modified_Since) ) {
		return HttpReqHeaderField_If_Modified_Since;
	} else if ( stringEquality(fieldName, HttpReqHeadField_If_None_Match) ) {
		return HttpReqHeaderField_If_None_Match;
	} else if ( stringEquality(fieldName, HttpReqHeadField_If_Range) ) {
		return HttpReqHeaderField_If_Range;
	} else if ( stringEquality(fieldName, HttpReqHeadField_If_Unmodified_Since) ) {
		return HttpReqHeaderField_If_Unmodified_Since;
	} else if ( stringEquality(fieldName, HttpReqHeadField_Max_Forwards) ) {
		return HttpReqHeaderField_Max_Forwards;
	} else if ( stringEquality(fieldName, HttpReqHeadField_Origin) ) {
		return HttpReqHeaderField_Origin;
	} else if ( stringEquality(fieldName, HttpReqHeadField_Pragma) ) {
		return HttpReqHeaderField_Pragma;
	} else if ( stringEquality(fieldName, HttpReqHeadField_Proxy_Authorization) ) {
		return HttpReqHeaderField_Proxy_Authorization;
	} else if ( stringEquality(fieldName, HttpReqHeadField_Pragma) ) {
		return HttpReqHeaderField_Pragma;
	} else if ( stringEquality(fieldName, HttpReqHeadField_Proxy_Authorization) ) {
		return HttpReqHeaderField_Proxy_Authorization;
	} else if ( stringEquality(fieldName, HttpReqHeadField_Range) ) {
		return HttpReqHeaderField_Range;
	} else if ( stringEquality(fieldName, HttpReqHeadField_Referer) ) {
		return HttpReqHeaderField_Referer;
	} else if ( stringEquality(fieldName, HttpReqHeadField_TE) ) {
		return HttpReqHeaderField_TE;
	} else if ( stringEquality(fieldName, HttpReqHeadField_User_Agent) ) {
		return HttpReqHeaderField_User_Agent;
	} else if ( stringEquality(fieldName, HttpReqHeadField_Via) ) {
		return HttpReqHeaderField_Via;
	} else if ( stringEquality(fieldName, HttpReqHeadField_Warning) ) {
		return HttpReqHeaderField_Warning;
	}
	
	// Conversion failed.
	return -1;
}

const char *HttpReqHeadFieldForField(HttpReqHeaderField field)
{
	switch (field) {
		case HttpReqHeaderField_Accept: return HttpReqHeadField_Accept;
		case HttpReqHeaderField_Accept_Charset: return HttpReqHeadField_Accept_Charset;
		case HttpReqHeaderField_Accept_Encoding: return HttpReqHeadField_Accept_Encoding;
		case HttpReqHeaderField_Accept_Language: return HttpReqHeadField_Accept_Language;
		case HttpReqHeaderField_Accept_Datetime: return HttpReqHeadField_Accept_Datetime;
		case HttpReqHeaderField_Authorization: return HttpReqHeadField_Authorization;
		case HttpReqHeaderField_Cache_Control: return HttpReqHeadField_Cache_Control;
		case HttpReqHeaderField_Connection: return HttpReqHeadField_Connection;
		case HttpReqHeaderField_Proxy_Connection: return HttpReqHeadField_Proxy_Connection;
		case HttpReqHeaderField_Cookie: return HttpReqHeadField_Cookie;
		case HttpReqHeaderField_Content_Length: return HttpReqHeadField_Content_Length;
		case HttpReqHeaderField_Content_MD5: return HttpReqHeadField_Content_MD5;
		case HttpReqHeaderField_Content_Type: return HttpReqHeadField_Content_Type;
		case HttpReqHeaderField_Date: return HttpReqHeadField_Date;
		case HttpReqHeaderField_Expect: return HttpReqHeadField_Expect;
		case HttpReqHeaderField_From: return HttpReqHeadField_From;
		case HttpReqHeaderField_Host: return HttpReqHeadField_Host;
		case HttpReqHeaderField_If_Match: return HttpReqHeadField_If_Match;
		case HttpReqHeaderField_If_Modified_Since: return HttpReqHeadField_If_Modified_Since;
		case HttpReqHeaderField_If_None_Match: return HttpReqHeadField_If_None_Match;
		case HttpReqHeaderField_If_Range: return HttpReqHeadField_If_Range;
		case HttpReqHeaderField_If_Unmodified_Since: return HttpReqHeadField_If_Unmodified_Since;
		case HttpReqHeaderField_Max_Forwards: return HttpReqHeadField_Max_Forwards;
		case HttpReqHeaderField_Origin: return HttpReqHeadField_Origin;
		case HttpReqHeaderField_Pragma: return HttpReqHeadField_Pragma;
		case HttpReqHeaderField_Proxy_Authorization: return HttpReqHeadField_Proxy_Authorization;
		case HttpReqHeaderField_Range: return HttpReqHeadField_Range;
		case HttpReqHeaderField_Referer: return HttpReqHeadField_Referer;
		case HttpReqHeaderField_TE: return HttpReqHeadField_TE;
		case HttpReqHeaderField_User_Agent: return HttpReqHeadField_User_Agent;
		case HttpReqHeaderField_Via: return HttpReqHeadField_Via;
		case HttpReqHeaderField_Warning: return HttpReqHeadField_Warning;
		default: return NULL;
	}
}

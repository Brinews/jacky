
#ifndef _HTTPRequest_h
#define _HTTPRequest_h

#include "defs.h"

typedef char ** HTTPRequest;

HTTPRequest HTTPRequestCreate();

void HTTPRequestFree(HTTPRequest request);

/// Validate an instance of HTTPRequest.
bool validateRequest(HTTPRequest request);

/// Create a request string from a given HTTP Request.
char *requestStringFromRequest(HTTPRequest request);

/// A header field name for an HTTP Request.
typedef const char * const HttpReqHeadField;

static HttpReqHeadField HttpReqHeadField_Accept = "Accept";
static HttpReqHeadField HttpReqHeadField_Accept_Charset = "Accept-Charset";
static HttpReqHeadField HttpReqHeadField_Accept_Encoding = "Accept-Encoding";
static HttpReqHeadField HttpReqHeadField_Accept_Language = "Accept-Language";
static HttpReqHeadField HttpReqHeadField_Accept_Datetime = "Accept-Datetime";
static HttpReqHeadField HttpReqHeadField_Authorization = "Authorization";
static HttpReqHeadField HttpReqHeadField_Cache_Control = "Cache-Control";
static HttpReqHeadField HttpReqHeadField_Connection = "Connection";
static HttpReqHeadField HttpReqHeadField_Proxy_Connection = "Proxy-Connection";
static HttpReqHeadField HttpReqHeadField_Cookie = "Cookie";
static HttpReqHeadField HttpReqHeadField_Content_Length = "Content-Length";
static HttpReqHeadField HttpReqHeadField_Content_MD5 = "Content-MD5";
static HttpReqHeadField HttpReqHeadField_Content_Type = "Content-Type";
static HttpReqHeadField HttpReqHeadField_Date = "Date";
static HttpReqHeadField HttpReqHeadField_Expect = "Expect";
static HttpReqHeadField HttpReqHeadField_From = "From";
static HttpReqHeadField HttpReqHeadField_Host = "Host";
static HttpReqHeadField HttpReqHeadField_If_Match = "If-Match";
static HttpReqHeadField HttpReqHeadField_If_Modified_Since = "If-Modified-Since";
static HttpReqHeadField HttpReqHeadField_If_None_Match = "If-None-Match";
static HttpReqHeadField HttpReqHeadField_If_Range = "If-Range";
static HttpReqHeadField HttpReqHeadField_If_Unmodified_Since = "If-Unmodified-Since";
static HttpReqHeadField HttpReqHeadField_Max_Forwards = "Max-Forwards";
static HttpReqHeadField HttpReqHeadField_Origin = "Origin";
static HttpReqHeadField HttpReqHeadField_Pragma = "Pragma";
static HttpReqHeadField HttpReqHeadField_Proxy_Authorization = "Proxy-Authorization";
static HttpReqHeadField HttpReqHeadField_Range = "Range";
static HttpReqHeadField HttpReqHeadField_Referer = "Referer";
static HttpReqHeadField HttpReqHeadField_TE = "TE";
static HttpReqHeadField HttpReqHeadField_User_Agent = "User-Agent";
static HttpReqHeadField HttpReqHeadField_Via = "Via";
static HttpReqHeadField HttpReqHeadField_Warning = "Warning";

/// Enumerated header fields for an HTTP Request.
typedef enum {
	HttpReqHeaderField_Request_Line,
	HttpReqHeaderField_Accept,
	HttpReqHeaderField_Accept_Charset,
	HttpReqHeaderField_Accept_Encoding,
	HttpReqHeaderField_Accept_Language,
	HttpReqHeaderField_Accept_Datetime,
	HttpReqHeaderField_Authorization,
	HttpReqHeaderField_Cache_Control,
	HttpReqHeaderField_Connection,
	HttpReqHeaderField_Proxy_Connection,
	HttpReqHeaderField_Cookie,
	HttpReqHeaderField_Content_Length,
	HttpReqHeaderField_Content_MD5,
	HttpReqHeaderField_Content_Type,
	HttpReqHeaderField_Date,
	HttpReqHeaderField_Expect,
	HttpReqHeaderField_From,
	HttpReqHeaderField_Host,
	HttpReqHeaderField_If_Match,
	HttpReqHeaderField_If_Modified_Since,
	HttpReqHeaderField_If_None_Match,
	HttpReqHeaderField_If_Range,
	HttpReqHeaderField_If_Unmodified_Since,
	HttpReqHeaderField_Max_Forwards,
	HttpReqHeaderField_Origin,
	HttpReqHeaderField_Pragma,
	HttpReqHeaderField_Proxy_Authorization,
	HttpReqHeaderField_Range,
	HttpReqHeaderField_Referer,
	HttpReqHeaderField_TE,
	HttpReqHeaderField_User_Agent,
	HttpReqHeaderField_Via,
	HttpReqHeaderField_Warning,
	
	HttpReqHeaderFieldsCount
} HttpReqHeaderField;

// Get the HttpReqHeaderField enum value for an HttpReqHeadField.
HttpReqHeaderField HttpReqHeaderFieldForFieldNamed(HttpReqHeadField fieldName);

// Get the HttpReqHeadField for an HttpReqHeaderField.
const char *HttpReqHeadFieldForField(HttpReqHeaderField field);

#endif

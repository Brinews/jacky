#ifndef _HTTPResponse_h
#define _HTTPResponse_h

/// The header fields of ab HTTP Response.
typedef char ** HTTPResponse;

/// Create an instance of HTTPResponse.
HTTPResponse HTTPResponseCreate();

/// Deallocate an instance of HTTPResponse.
void HTTPResponseFree(HTTPResponse response);

/// A header field name for an HTTP Response.
typedef const char * const HTTPResponseHeaderFieldName;

static HTTPResponseHeaderFieldName HTTPResponseHeaderFieldName_Access_Control_Allow_Origin = "Access-Control-Allow-Origin";
static HTTPResponseHeaderFieldName HTTPResponseHeaderFieldName_Accept_Ranges = "Accept-Ranges";
static HTTPResponseHeaderFieldName HTTPResponseHeaderFieldName_Age = "Age";
static HTTPResponseHeaderFieldName HTTPResponseHeaderFieldName_Allow = "Allow";
static HTTPResponseHeaderFieldName HTTPResponseHeaderFieldName_Cache_Control = "Cache-Control";
static HTTPResponseHeaderFieldName HTTPResponseHeaderFieldName_Connection = "Connection";
static HTTPResponseHeaderFieldName HTTPResponseHeaderFieldName_Content_Encoding = "Content-Encoding";
static HTTPResponseHeaderFieldName HTTPResponseHeaderFieldName_Content_Language = "Content-Language";
static HTTPResponseHeaderFieldName HTTPResponseHeaderFieldName_Content_Length = "Content-Length";
static HTTPResponseHeaderFieldName HTTPResponseHeaderFieldName_Content_Location = "Content-Location";
static HTTPResponseHeaderFieldName HTTPResponseHeaderFieldName_Content_MD5 = "Content-MD5";
static HTTPResponseHeaderFieldName HTTPResponseHeaderFieldName_Content_Disposition = "Content-Disposition";
static HTTPResponseHeaderFieldName HTTPResponseHeaderFieldName_Content_Range = "Content-Range";
static HTTPResponseHeaderFieldName HTTPResponseHeaderFieldName_Content_Type = "Content-Type";
static HTTPResponseHeaderFieldName HTTPResponseHeaderFieldName_Date = "Date";
static HTTPResponseHeaderFieldName HTTPResponseHeaderFieldName_ETag = "ETag";
static HTTPResponseHeaderFieldName HTTPResponseHeaderFieldName_Expires = "Expires";
static HTTPResponseHeaderFieldName HTTPResponseHeaderFieldName_Last_Modified = "Last-Modified";
static HTTPResponseHeaderFieldName HTTPResponseHeaderFieldName_Link = "Link";
static HTTPResponseHeaderFieldName HTTPResponseHeaderFieldName_Location = "Location";
static HTTPResponseHeaderFieldName HTTPResponseHeaderFieldName_P3P = "P3P";
static HTTPResponseHeaderFieldName HTTPResponseHeaderFieldName_Pragma = "Pragma";
static HTTPResponseHeaderFieldName HTTPResponseHeaderFieldName_Proxy_Authenticate = "Proxy-Authenticate";
static HTTPResponseHeaderFieldName HTTPResponseHeaderFieldName_Refresh = "Refresh";
static HTTPResponseHeaderFieldName HTTPResponseHeaderFieldName_Retry_After = "Retry-After";
static HTTPResponseHeaderFieldName HTTPResponseHeaderFieldName_Server = "Server";
static HTTPResponseHeaderFieldName HTTPResponseHeaderFieldName_Set_Cookie = "Set-Cookie";
static HTTPResponseHeaderFieldName HTTPResponseHeaderFieldName_Status = "Status";
static HTTPResponseHeaderFieldName HTTPResponseHeaderFieldName_Strict_Transport_Security = "Strict-Transport-Security";
static HTTPResponseHeaderFieldName HTTPResponseHeaderFieldName_Trailer = "Trailer";
static HTTPResponseHeaderFieldName HTTPResponseHeaderFieldName_Transfer_Encoding = "Transfer-Encoding";
static HTTPResponseHeaderFieldName HTTPResponseHeaderFieldName_Upgrade = "Upgrade";
static HTTPResponseHeaderFieldName HTTPResponseHeaderFieldName_Vary = "Vary";
static HTTPResponseHeaderFieldName HTTPResponseHeaderFieldName_Via = "Via";
static HTTPResponseHeaderFieldName HTTPResponseHeaderFieldName_Warning = "Warning";
static HTTPResponseHeaderFieldName HTTPResponseHeaderFieldName_WWW_Authenticate = "WWW-Authenticate";

/// Enumerated header fields for an HTTP Response.
typedef enum {
	HTTPResponseHeaderField_Status_Line,
	HTTPResponseHeaderField_Access_Control_Allow_Origin,
	HTTPResponseHeaderField_Accept_Ranges,
	HTTPResponseHeaderField_Age,
	HTTPResponseHeaderField_Allow,
	HTTPResponseHeaderField_Cache_Control,
	HTTPResponseHeaderField_Connection,
	HTTPResponseHeaderField_Content_Encoding,
	HTTPResponseHeaderField_Content_Language,
	HTTPResponseHeaderField_Content_Length,
	HTTPResponseHeaderField_Content_Location,
	HTTPResponseHeaderField_Content_MD5,
	HTTPResponseHeaderField_Content_Disposition,
	HTTPResponseHeaderField_Content_Range,
	HTTPResponseHeaderField_Content_Type,
	HTTPResponseHeaderField_Date,
	HTTPResponseHeaderField_ETag,
	HTTPResponseHeaderField_Expires,
	HTTPResponseHeaderField_Last_Modified,
	HTTPResponseHeaderField_Link,
	HTTPResponseHeaderField_Location,
	HTTPResponseHeaderField_P3P,
	HTTPResponseHeaderField_Pragma,
	HTTPResponseHeaderField_Proxy_Authenticate,
	HTTPResponseHeaderField_Refresh,
	HTTPResponseHeaderField_Retry_After,
	HTTPResponseHeaderField_Server,
	HTTPResponseHeaderField_Set_Cookie,
	HTTPResponseHeaderField_Status,
	HTTPResponseHeaderField_Strict_Transport_Security,
	HTTPResponseHeaderField_Trailer,
	HTTPResponseHeaderField_Transfer_Encoding,
	HTTPResponseHeaderField_Upgrade,
	HTTPResponseHeaderField_Vary,
	HTTPResponseHeaderField_Via,
	HTTPResponseHeaderField_Warning,
	HTTPResponseHeaderField_WWW_Authenticate,
	
	// The total number of HTTPResponseHeaderField
	HTTPResponseHeaderFieldsCount
} HTTPResponseHeaderField;

// Get the HTTPResponseHeaderField enum value for an HTTPResponseHeaderFieldName.
HTTPResponseHeaderField HTTPResponseHeaderFieldForFieldNamed(HTTPResponseHeaderFieldName fieldName);

// Look up the status string for a particular status code.
const char * const statusStringForStatusCode(int statusCode);

#endif

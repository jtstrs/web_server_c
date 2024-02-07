#ifndef HTTP_REQUEST
#define HTTP_REQUEST

#include "../containers/list.h"
#include "common.h"

#define MAX_REQUEST_LINE_SIZE 256
#define MAX_REQUEST_HEADER_SECTION_SIZE 2048
#define MAX_HEADER_SIZE 256
#define MAX_URI_SIZE 256
#define MAX_HEADER_VALUE_SIZE 256

typedef enum ErrorStatus {
    Ok,
    MaliciousMethod,
    UnsupportedVersion,
    GenericError
} ErrorStatus;

typedef struct HttpHeader {
    char title[MAX_HEADER_SIZE + 1];
    char value[MAX_HEADER_VALUE_SIZE + 1];
} HttpHeader;


typedef struct HttpRequest {
    HttpMethod method;
    char uri[MAX_URI_SIZE + 1];
    HttpVersion version;

    List *headers;
} HttpRequest;

HttpRequest *parse_request(char *request_buffer);

char *get_request_uri(HttpRequest *request);
HttpVersion get_request_version(HttpRequest *request);
HttpMethod get_request_method(HttpRequest *request);


#endif
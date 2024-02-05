#ifndef HTTP_REQUEST
#define HTTP_REQUEST

#include "common.h"

enum ErrorStatus {
    Ok,
    MaliciousMethod,
    UnsupportedVersion,
    GenericError
};

typedef enum ErrorStatus ErrorStatus;

typedef struct HttpRequest HttpRequest;

HttpRequest *parse_request(char *request_buffer);

char *get_request_uri(HttpRequest *request);
HttpVersion get_request_version(HttpRequest *request);
HttpMethod get_request_method(HttpRequest *request);


#endif
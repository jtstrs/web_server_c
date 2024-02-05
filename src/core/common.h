#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>

#define handle_error(msg)   \
    do {                    \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0);

#define INVALID_DESCRIPTOR -1
#define ERROR_STATUS -1
#define ALWAYS 1
#define BACKLOG_QUEUE 10
#define HOST_NAME_LENGTH 64

enum HttpMethod {
    OPTIONS,
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    TRACE,
    CONNECT,
    UNSUPPORTED_METHOD,
    UNKNOWN_METHOD
};

enum HttpVersion {
    HTTP_1,
    HTTP_1_1,
    HTTP_2,
    UNSUPPORTED_VERSION
};

enum HttpStatusCodes {
    CONTINUE = 100,
    SWITCHING_PROTOCOLS = 101,
    OK = 200,
    CREATED = 201,
    ACCEPTED = 202,
    NON_AUTHORITATIVE_INFORMATION = 203,
    NO_CONTENT = 204,
    RESET_CONTENT = 205,
    PARTIAL_CONTENT = 206,
    MULTIPLE_CHOICES = 300,
    MOVED_PERMANENTLY = 301,
    FOUND = 302,
    SEE_OTHER = 303,
    NOT_MODIFIED = 304,
    USE_PROXY = 305,
    TEMPORARY_REDIRECT = 307,
    BAD_REQUEST = 400,
    UNAUTHORIZED = 401,
    PAYMENT_REQUIRED = 402,
    FORBIDDEN = 403,
    NOT_FOUND = 404,
    METHOD_NOT_ALLOWED = 405,
    NOT_ACCEPTABLE = 406,
    PROXY_AUTH_REQUIRED = 407,
    REQUEST_TIME_OUT = 408,
    CONFLICT = 409,
    GONE = 410,
    LENGTH_REQUIRED = 411,
    PRECONDITION_FAILED = 412,
    REQUEST_ENTITY_TOO_LARGE = 413,
    REQUEST_URI_TOO_LARGE = 414,
    UNSUPPORTED_MEDIA_TYPE = 415,
    REQUESTED_RANGE_NOT_SATISFIABLE = 416,
    EXPECTATION_FAILED = 417,
    INTERNAL_SERVER_ERROR = 500,
    NOT_IMPLEMENTED = 501,
    BAD_GATEWAY = 502,
    SERVICE_UNAVAIABLE = 503,
    GATEWAY_TIME_OUT = 504,
    HTTP_VERSION_NOT_SUPPORTED = 505
};

typedef enum HttpStatusCodes HttpStatusCodes;
typedef enum HttpVersion HttpVersion;
typedef enum HttpMethod HttpMethod;

HttpMethod str_to_http_method(char *method_buffer);
char *http_method_to_str(HttpMethod method);

HttpVersion str_to_http_version(char *version_buffer);

#endif
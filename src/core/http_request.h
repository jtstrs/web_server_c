#ifndef HTTP_REQUEST
#define HTTP_REQUEST

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

enum ErrorStatus {
    Ok,
    MaliciousMethod,
    UnsupportedVersion,
    GenericError
};

typedef enum HttpVersion HttpVersion;
typedef enum HttpMethod HttpMethod;
typedef enum ErrorStatus ErrorStatus;

typedef struct HttpRequest HttpRequest;

HttpRequest *parse_request(char *request_buffer);

char *get_request_uri(HttpRequest *request);
HttpMethod get_request_method(HttpRequest *request);

#endif
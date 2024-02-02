#include "http_request.h"

#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "log.h"

static const char *request_delims = "\n\r: ";

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
    UnsupportedMethod,
    UnsupportedVersion,
    GenericError
};

typedef enum HttpVersion HttpVersion;
typedef enum HttpMethod HttpMethod;
typedef enum ErrorStatus ErrorStatus;

HttpMethod str_to_http_method(char *method_buffer) {
    if (strcmp(method_buffer, "OPTIONS") == 0) {
        return UNSUPPORTED_METHOD;
    } else if (strcmp(method_buffer, "GET") == 0) {
        return GET;
    } else if (strcmp(method_buffer, "HEAD") == 0) {
        return UNSUPPORTED_METHOD;
    } else if (strcmp(method_buffer, "POST") == 0) {
        return UNSUPPORTED_METHOD;
    } else if (strcmp(method_buffer, "PUT") == 0) {
        return UNSUPPORTED_METHOD;
    } else if (strcmp(method_buffer, "DELETE") == 0) {
        return UNSUPPORTED_METHOD;
    } else if (strcmp(method_buffer, "TRACE") == 0) {
        return UNSUPPORTED_METHOD;
    } else if (strcmp(method_buffer, "CONNECT") == 0) {
        return UNSUPPORTED_METHOD;
    }
    return UNKNOWN_METHOD;
}

HttpVersion str_to_http_version(char *version_buffer) {
    if (strcmp(version_buffer, "HTTP/1.0")) {
        return HTTP_1;
    } else if (strcmp(version_buffer, "HTTP/1.1")) {
        return HTTP_1_1;
    } else if (strcmp(version_buffer, "HTTP/2.0")) {
        return HTTP_2;
    }
    return UNSUPPORTED_VERSION;
}

struct HttpRequest {
    HttpMethod method;
    char *uri;
    HttpVersion version;
};

ErrorStatus parse_request_line(HttpRequest *request, char *tokenized) {

    if (!tokenized) {
        return GenericError;
    }

    char *method_token = tokenized;

    log_message(DEBUG_LEVEL, "Request method is: %s", method_token);

    HttpMethod method = str_to_http_method(method_token);

    if (method == UNKNOWN_METHOD) {
        return MaliciousMethod;
    }

    if (method == UNSUPPORTED_METHOD) {
        return UnsupportedMethod;
    }

    // TODO: Add uri validation
    char *uri_token = strtok(NULL, request_delims);

    if (!uri_token) {
        return GenericError;
    }
    log_message(DEBUG_LEVEL, "Request uri is: %s", uri_token);

    char *http_version_token = strtok(NULL, request_delims);

    if (!http_version_token) {
        return GenericError;
    }
    log_message(DEBUG_LEVEL, "Request http version is: %s", http_version_token);

    HttpVersion version = str_to_http_version(http_version_token);

    if (version == UNSUPPORTED_VERSION) {
        return UnsupportedVersion;
    }

    request->method = method;
    request->version = version;
    request->uri = uri_token;

    return Ok;
}

HttpRequest *parse_request(char *request_buffer) {
    HttpRequest *request = (HttpRequest *) malloc(sizeof(HttpRequest));

    if (!request) {
        log_message(ERROR_LEVEL, "Couldnt allocate memory for request structure\n");
        return NULL;
    }

    log_message(DEBUG_LEVEL, "Parsing request. Request content:\n%s\n", request_buffer);

    char *tokenized = strtok(request_buffer, request_delims);

    ErrorStatus err = parse_request_line(request, tokenized);

    if (err != Ok) {
        log_message(ERROR_LEVEL, "Error hapenned. Error code: %d\n", err);
        free(request);
        return NULL;
    }

    return request;
}

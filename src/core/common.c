#include "common.h"

#include <string.h>

HttpMethod str_to_http_method(char *method_buffer) {
    if (strcmp(method_buffer, "OPTIONS") == 0) {
        return OPTIONS;
    } else if (strcmp(method_buffer, "GET") == 0) {
        return GET;
    } else if (strcmp(method_buffer, "HEAD") == 0) {
        return HEAD;
    } else if (strcmp(method_buffer, "POST") == 0) {
        return POST;
    } else if (strcmp(method_buffer, "PUT") == 0) {
        return PUT;
    } else if (strcmp(method_buffer, "DELETE") == 0) {
        return DELETE;
    } else if (strcmp(method_buffer, "TRACE") == 0) {
        return TRACE;
    } else if (strcmp(method_buffer, "CONNECT") == 0) {
        return CONNECT;
    }
    return UNKNOWN_METHOD;
}

char *http_method_to_str(HttpMethod method) {
    switch (method) {
        case OPTIONS:
            return "OPTIONS";
        case GET:
            return "GET";
        case HEAD:
            return "HEAD";
        case POST:
            return "POST";
        case PUT:
            return "PUT";
        case DELETE:
            return "DELETE";
        case TRACE:
            return "TRACE";
        case CONNECT:
            return "CONNECT";

        // Fallthrough
        case UNSUPPORTED_METHOD:
        case UNKNOWN_METHOD:
        default:
            return NULL;
    }
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
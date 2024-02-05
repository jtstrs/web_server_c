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

char *http_version_to_str(HttpVersion version) {
    switch (version) {
        case HTTP_1:
            return "HTTP/1.0";
        case HTTP_1_1:
            return "HTTP/1.1";
        case HTTP_2:
            return "HTTP/2.0";
        // [Fallthrought]
        case UNSUPPORTED_VERSION:
        default:
            return NULL;
    }
}

char *http_status_to_str(HttpStatusCode code) {
    switch (code) {
        case CONTINUE:
            return "CONTINUE";
        case SWITCHING_PROTOCOLS:
            return "SWITCHING PROTOCOLS";
        case OK:
            return "OK";
        case CREATED:
            return "CREATED";
        case ACCEPTED:
            return "ACCEPTED";
        case NON_AUTHORITATIVE_INFORMATION:
            return "NON AUTHORITATIVE INFORMATION";
        case NO_CONTENT:
            return "NO CONTENT";
        case RESET_CONTENT:
            return "RESET CONTENT";
        case PARTIAL_CONTENT:
            return "PARTIAL CONTENT";
        case MULTIPLE_CHOICES:
            return "MULTIPLE CHOICES";
        case MOVED_PERMANENTLY:
            return "MOVED PERMANENTLY";
        case FOUND:
            return "FOUND";
        case SEE_OTHER:
            return "SEE OTHER";
        case NOT_MODIFIED:
            return "NOT MODIFIED";
        case USE_PROXY:
            return "USE PROXY";
        case TEMPORARY_REDIRECT:
            return "TEMPORARY REDIRECT";
        case BAD_REQUEST:
            return "BAD REQUEST";
        case UNAUTHORIZED:
            return "UAUTHORIZED";
        case PAYMENT_REQUIRED:
            return "PAYMENT REQUIRED";
        case FORBIDDEN:
            return "FORBIDDEN";
        case NOT_FOUND:
            return "NOT FOUND";
        case METHOD_NOT_ALLOWED:
            return "METHOD NOT ALLOWED";
        case NOT_ACCEPTABLE:
            return "NOT ACCEPTABLE";
        case PROXY_AUTH_REQUIRED:
            return "PROXY AUTH REQUIRED";
        case REQUEST_TIME_OUT:
            return "REQUEST TIME OUT";
        case CONFLICT:
            return "CONFLICT";
        case GONE:
            return "GONE";
        case LENGTH_REQUIRED:
            return "LENGTH REQUIRED";
        case PRECONDITION_FAILED:
            return "PRECONDITION FAILED";
        case REQUEST_ENTITY_TOO_LARGE:
            return "REQUEST ENTITY TOO LARGE";
        case REQUEST_URI_TOO_LARGE:
            return "REQUEST URI TOO LARGE";
        case UNSUPPORTED_MEDIA_TYPE:
            return "UNSUPPORTED MEDIA TYPE";
        case REQUESTED_RANGE_NOT_SATISFIABLE:
            return "REQUESTED RANGE NOT SATISFIABLE";
        case EXPECTATION_FAILED:
            return "EXPECTATION FAILED";
        case INTERNAL_SERVER_ERROR:
            return "INTERNAL SERVER ERROR";
        case NOT_IMPLEMENTED:
            return "NOT IMPLEMENTED";
        case BAD_GATEWAY:
            return "BAD GATEWAY";
        case SERVICE_UNAVAIABLE:
            return "SERVICE UNAVAILABLE";
        case GATEWAY_TIME_OUT:
            return "GATEWAY TIME OUT";
        case HTTP_VERSION_NOT_SUPPORTED:
            return "HTTP VERSION NOT SUPPORTED";
        case UNKNOWN:
        default:
            return NULL;
    }
}
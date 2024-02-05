#include "http_request.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../containers/list.h"
#include "log.h"

#define MAX_REQUEST_LINE_SIZE 256
#define MAX_REQUEST_HEADER_SECTION_SIZE 2048
#define MAX_HEADER_SIZE 256
#define MAX_URI_SIZE 256
#define MAX_HEADER_VALUE_SIZE 256

struct HttpHeader {
    char title[MAX_HEADER_SIZE + 1];
    char value[MAX_HEADER_VALUE_SIZE + 1];
};

typedef struct HttpHeader HttpHeader;

struct HttpRequest {
    HttpMethod method;
    char uri[MAX_URI_SIZE + 1];
    HttpVersion version;

    List *headers;
};


char *get_request_uri(HttpRequest *request) {
    if (!request) {
        return NULL;
    }
    return request->uri;
}

HttpMethod get_request_method(HttpRequest *request) {
    if (!request) {
        return UNKNOWN_METHOD;
    }
    return request->method;
}

ErrorStatus parse_request_line(HttpRequest *request, char *request_line) {

    static const char *request_line_delims = " ";

    if (!request_line) {
        return GenericError;
    }

    char *tokenized = strtok(request_line, request_line_delims);

    char *method_token = tokenized;

    log_message(DEBUG_LEVEL, "Request method is: %s", method_token);

    HttpMethod method = str_to_http_method(method_token);

    if (method == UNKNOWN_METHOD) {
        return MaliciousMethod;
    }

    // TODO: Add uri validation
    char *uri_token = strtok(NULL, request_line_delims);

    if (!uri_token) {
        return GenericError;
    }
    log_message(DEBUG_LEVEL, "Request uri is: %s", uri_token);

    char *http_version_token = strtok(NULL, request_line_delims);

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
    strcpy(request->uri, uri_token);

    return Ok;
}

ErrorStatus parse_request_header(HttpRequest *request, char *request_headers) {
    // We couldnt split header by '\n\r: tokens
    // because Header: <address>:<port> use cases possible
    static const char *request_headers_delims = "\r\n";
    static const char header_middle_marker = ':';
    static const char header_end_marker = '\0';

    List *headers_list = create_list(NULL);

    if (!headers_list) {
        return GenericError;
    }

    for (char *header_line = strtok(request_headers, request_headers_delims); header_line;
         header_line = strtok(NULL, request_headers_delims)) {

        log_message(DEBUG_LEVEL, "Trying parse header: %s", header_line);

        // +1 because we do not need : character
        char *header_middle = strchr(header_line, header_middle_marker) + 1;

        if (!header_middle) {
            log_message(DEBUG_LEVEL, "Couldnt find header_middle ptr. Skip");
            continue;
        }

        char header_title_buffer[MAX_HEADER_SIZE + 1];
        memset(header_title_buffer, 0, MAX_HEADER_SIZE + 1);

        // Same as above
        const int32_t header_size = header_middle - header_line - 1;

        if (header_size <= 0 || header_size > MAX_HEADER_SIZE) {
            log_message(DEBUG_LEVEL, "Incorrect header size. Skip");
            continue;
        }

        strncpy(header_title_buffer, header_line, header_size);

        char *header_end = strchr(header_line, header_end_marker);

        if (!header_end) {
            log_message(DEBUG_LEVEL, "Couldnt find header end ptr. Skip");
            continue;
        }

        char header_value_buffer[MAX_HEADER_SIZE + 1];
        memset(header_value_buffer, 0, MAX_HEADER_SIZE + 1);

        const int32_t header_value_size = header_end - header_middle;

        if (header_value_size <= 0 || header_value_size > MAX_HEADER_SIZE) {
            log_message(DEBUG_LEVEL, "Incorrect header value size. Skip");
            continue;
        }

        strncpy(header_value_buffer, header_middle, header_value_size);

        HttpHeader *header = (HttpHeader *) malloc(sizeof(HttpHeader));

        strcpy(header->title, header_title_buffer);
        strcpy(header->value, header_value_buffer);

        push_list_item(headers_list, header);
    }

    request->headers = headers_list;
    return Ok;
}

HttpRequest *parse_request(char *request_buffer) {
    HttpRequest *request = (HttpRequest *) malloc(sizeof(HttpRequest));

    if (!request) {
        log_message(ERROR_LEVEL, "Couldnt allocate memory for request structure\n");
        return NULL;
    }

    log_message(DEBUG_LEVEL, "Parsing request. Request content:\n%s\n", request_buffer);

    const char *request_buffer_iterator = request_buffer;

    char request_line[MAX_REQUEST_LINE_SIZE + 1];
    memset(request_line, 0, MAX_REQUEST_LINE_SIZE + 1);

    const char request_line_delimeter = '\n';
    const char *request_line_end = strchr(request_buffer_iterator, request_line_delimeter);

    if (!request_line_end) {
        log_message(ERROR_LEVEL, "Couldnt find request line in request conten\n");
        free(request);
        return NULL;
    }
    const int32_t request_line_size = request_line_end - request_buffer_iterator;

    if (request_line_size <= 0 || request_line_size > MAX_REQUEST_LINE_SIZE) {
        log_message(ERROR_LEVEL, "Incorrect request line size\n");
        free(request);
        return NULL;
    }

    strncpy(request_line, request_buffer_iterator, request_line_size);

    ErrorStatus err = parse_request_line(request, request_line);

    if (err != Ok) {
        log_message(ERROR_LEVEL, "Error hapenned while parsing request line. Error code: %d\n", err);
        free(request);
        return NULL;
    }

    request_buffer_iterator = request_line_end;

    char request_header_section[MAX_REQUEST_HEADER_SECTION_SIZE + 1];
    memset(request_header_section, 0, MAX_REQUEST_HEADER_SECTION_SIZE + 1);

    const char *marker_request_end = "\r\n\r\n";
    const char *request_buffer_end = strstr(request_buffer_iterator, marker_request_end);

    if (!request_buffer_iterator) {
        log_message(ERROR_LEVEL, "Cannot find end of request");
        free(request);
        return NULL;
    }

    const int32_t request_headers_size = request_buffer_end - request_buffer_iterator;

    if (request_headers_size <= 0 || request_headers_size > MAX_REQUEST_HEADER_SECTION_SIZE) {
        log_message(ERROR_LEVEL, "Incorrect request headers section size");
        free(request);
        return NULL;
    }

    strncpy(request_header_section, request_buffer_iterator, request_headers_size);

    err = parse_request_header(request, request_header_section);

    if (err != Ok) {
        log_message(ERROR_LEVEL, "Error happened while parsing request headers. Error code: %d\n", err);
        free(request);
        return NULL;
    }

    return request;
}

void release_request(HttpRequest *request) {
    if (request) {
        release_list(request->headers);
    }
    free(request);
}

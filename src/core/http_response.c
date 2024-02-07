#include "http_response.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RESPONSE_BUFFER_SIZE MAX_RESPONSE_BODY_SIZE + 2048
#define MAX_STATUS_LINE_SIZE 256
#define MAX_STATUS_CODE_SIZE 4
#define MAX_REASON_PHRASE_SIZE 128

void delete_header(void *header) {
    // TODO!!!!
    exit(1);
}

HttpResponse *create_response() {
    HttpResponse *response = (HttpResponse *) malloc(sizeof(HttpResponse));

    if (!response) {
        return NULL;
    }

    response->response_headers = create_list(delete_header);

    if (!response->response_headers) {
        free(response);
        return NULL;
    }
    memset(response->response_body, 0, MAX_RESPONSE_BODY_SIZE);

    return response;
}

void release_response(HttpResponse *response) {

    if (response) {
        release_list(response->response_headers);
    }
    free(response);
}

char *serialize_status_line(HttpResponse *response) {
    static char status_line[MAX_STATUS_LINE_SIZE + 1];
    memset(status_line, 0, MAX_STATUS_LINE_SIZE);

    char *http_version = http_version_to_str(response->version);

    if (!http_version) {
        return NULL;
    }

    char *reason_phrase = http_status_to_str(response->code);

    snprintf(status_line, MAX_STATUS_LINE_SIZE, "%s %d %s", http_version, response->code, reason_phrase);

    return status_line;
}

char *serialize_headers(HttpResponse *response) {
    return "";
}

char *serialize_response(HttpResponse *response) {
    if (!response) {
        return "";
    }

    static char response_buffer[MAX_RESPONSE_BUFFER_SIZE + 1];
    memset(response_buffer, 0, MAX_RESPONSE_BUFFER_SIZE);

    char *status_line = serialize_status_line(response);
    char *response_headers = serialize_headers(response);

    snprintf(response_buffer, MAX_RESPONSE_BUFFER_SIZE, "%s\r\n%s\r\n\r\n%s",
             status_line, response_headers, response->response_body);
    return response_buffer;
}

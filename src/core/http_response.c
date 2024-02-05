#include "http_response.h"
#include <stdio.h>
#include <string.h>

#define MAX_RESPONSE_BUFFER_SIZE 2048
#define MAX_STATUS_LINE_SIZE 256
#define MAX_STATUS_CODE_SIZE 4
#define MAX_REASON_PHRASE_SIZE 128

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
    char *response_body = "";

    snprintf(response_buffer, MAX_RESPONSE_BUFFER_SIZE, "%s\r\n%s\r\n\r\n%s",
             status_line, response_headers, response_body);
    return response_buffer;
}
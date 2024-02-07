#ifndef HTTP_RESPONSE
#define HTTP_RESPONSE

#include "../containers/list.h"
#include "common.h"

#define MAX_RESPONSE_BUFFER_SIZE MAX_RESPONSE_BODY_SIZE + 2048
#define MAX_STATUS_LINE_SIZE 256
#define MAX_STATUS_CODE_SIZE 4
#define MAX_REASON_PHRASE_SIZE 128
#define MAX_RESPONSE_BODY_SIZE 8192

struct HttpResponse {
    HttpStatusCode code;
    HttpVersion version;
    List *response_headers;

    char response_body[MAX_RESPONSE_BODY_SIZE + 1];
};

typedef struct HttpResponse HttpResponse;

HttpResponse *create_response();
void release_response(HttpResponse *response);
char *serialize_response(HttpResponse *response);

#endif
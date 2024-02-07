#ifndef HTTP_RESPONSE
#define HTTP_RESPONSE

#include "../containers/list.h"
#include "common.h"

struct HttpResponse {
    HttpStatusCode code;
    HttpVersion version;
    List *response_headers;
};

typedef struct HttpResponse HttpResponse;

HttpResponse *create_response();
void release_response(HttpResponse *response);
char *serialize_response(HttpResponse *response);

#endif
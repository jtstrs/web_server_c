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
char *serialize_response(HttpResponse *response);

#endif
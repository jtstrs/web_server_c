#ifndef HTTP_REQUEST
#define HTTP_REQUEST

struct HttpRequest;
typedef struct HttpRequest HttpRequest;

HttpRequest *parse_request(char *request_buffer);

#endif
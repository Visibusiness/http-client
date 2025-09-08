#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <sstream>
#include "json.hpp"
using json = nlohmann::json;
#define SERVER_IP "63.32.125.183" 
#define SERVER_PORT 8081 

struct HttpResponse {
    json headers;
    json body;
};

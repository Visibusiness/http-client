#ifndef _HELPERS_
#define _HELPERS_

#define BUFLEN 4096
#define LINELEN 1000

#include <string>
#include <map>
#include "json.hpp"
#include <sstream>
#include <map>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <netdb.h>
#include <arpa/inet.h>

using json = nlohmann::json;

void parse_http_response(const std::string& response, json& headers_json, json& body_json);

bool check_http_error(const json& headers_json);

int open_connection(char *host_ip, int portno, int ip_type, int socket_type, int flag);

void close_connection(int sockfd);

void send_to_server(int sockfd, char *message);

char *receive_from_server(int sockfd);


std::string build_http_request(const std::string& method, const std::string& url, const std::string& host,
    const std::map<std::string, std::string>& headers = {}, const std::string& body = "");

void error(const char *msg);

#endif

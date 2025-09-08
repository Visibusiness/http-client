#include "helper.hpp"
#include "buffer.hpp"

using json = nlohmann::json;
using namespace std;

#define HEADER_TERMINATOR "\r\n\r\n"
#define HEADER_TERMINATOR_SIZE (sizeof(HEADER_TERMINATOR) - 1)
#define CONTENT_LENGTH "Content-Length: "
#define CONTENT_LENGTH_SIZE (sizeof(CONTENT_LENGTH) - 1)

bool check_http_error(const json& headers_json) {
    std::string status = headers_json.value("status", "");
    if (status.find("200") == std::string::npos && status.find("201") == std::string::npos) {
        std::cout << "ERROR: HTTP " << status << std::endl;
        return true;
    }
    return false;
}

void parse_http_response(const std::string& response, json& headers_json, json& body_json) {
    size_t pos = response.find("\r\n\r\n");
    headers_json = json::object();
    body_json = json::object();
    if (pos != std::string::npos) {
        std::string headers_str = response.substr(0, pos);
        std::string body_str = response.substr(pos + 4);
        std::istringstream header_stream(headers_str);
        std::string line;
        if (std::getline(header_stream, line)) {
            headers_json["status"] = line;
        }
        while (std::getline(header_stream, line)) {
            if (line.empty() || line == "\r") continue;
            size_t delim = line.find(":");
            if (delim != std::string::npos) {
                std::string key = line.substr(0, delim);
                std::string value = line.substr(delim + 1);
                value.erase(0, value.find_first_not_of(" \t\r"));
                headers_json[key] = value;
            }
        }
        body_json = json::parse(body_str);
    }
}

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int open_connection(char *host_ip, int portno, int ip_type, int socket_type, int flag)
{
    struct sockaddr_in serv_addr;
    int sockfd = socket(ip_type, socket_type, flag);
    if (sockfd < 0)
        error("ERROR opening socket");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = ip_type;
    serv_addr.sin_port = htons(portno);
    inet_aton(host_ip, &serv_addr.sin_addr);

    /* connect the socket */
    if (connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    return sockfd;
}

void close_connection(int sockfd)
{
    close(sockfd);
}

void send_to_server(int sockfd, char *message)
{
    int bytes, sent = 0;
    int total = strlen(message);

    do
    {
        bytes = write(sockfd, message + sent, total - sent);
        if (bytes < 0) {
            error("ERROR writing message to socket");
        }

        if (bytes == 0) {
            break;
        }

        sent += bytes;
    } while (sent < total);
}

char *receive_from_server(int sockfd)
{
    char response[BUFLEN];
    buffer buffer = buffer_init();
    int header_end = 0;
    int content_length = 0;

    do {
        int bytes = read(sockfd, response, BUFLEN);

        if (bytes < 0){
            error("ERROR reading response from socket");
        }

        if (bytes == 0) {
            break;
        }

        buffer_add(&buffer, response, (size_t) bytes);
        
        header_end = buffer_find(&buffer, HEADER_TERMINATOR, HEADER_TERMINATOR_SIZE);

        if (header_end >= 0) {
            header_end += HEADER_TERMINATOR_SIZE;
            
            int content_length_start = buffer_find_insensitive(&buffer, CONTENT_LENGTH, CONTENT_LENGTH_SIZE);
            
            if (content_length_start < 0) {
                continue;           
            }

            content_length_start += CONTENT_LENGTH_SIZE;
            content_length = strtol(buffer.data + content_length_start, NULL, 10);
            break;
        }
    } while (1);
    size_t total = content_length + (size_t) header_end;
    
    while (buffer.size < total) {
        int bytes = read(sockfd, response, BUFLEN);

        if (bytes < 0) {
            error("ERROR reading response from socket");
        }

        if (bytes == 0) {
            break;
        }

        buffer_add(&buffer, response, (size_t) bytes);
    }
    buffer_add(&buffer, "", 1);
    return buffer.data;
}

std::string build_http_request(const std::string& method, const std::string& url, const std::string& host,
    const std::map<std::string, std::string>& headers, const std::string& body) {
    std::ostringstream req;
    req << method << " " << url << " HTTP/1.1\r\n";
    req << "Host: " << host << "\r\n";
    for (const auto& h : headers) {
        req << h.first << ": " << h.second << "\r\n";
    }
    if (!body.empty()) {
        req << "Content-Length: " << body.size() << "\r\n";
    }
    req << "\r\n";
    if (!body.empty()) {
        req << body;
    }
    return req.str();
}

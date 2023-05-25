//
// Created by StudyHole on 21.05.2023.
//

#ifndef SERVER_HTTP_UTILS_H
#define SERVER_HTTP_UTILS_H

#include <string>
#include <map>
#include "../json/json.hpp"

struct HTTPPayload {
    std::string route;
    std::string method;
    std::map<std::string, std::string> headers;
    std::vector<char> body;
};

// "HTTP/1.1 200 OK\nContent-Length:" + std::to_string(response_body.length()) + "\n\n" + response_body;

struct HTTPResponse {
    std::string status;
    int status_code;
    std::map<std::string, std::string> headers;
    std::vector<char> data;
};


class http_utils {
public:
    static HTTPPayload parse_http_payload(std::vector<char> payload);
    static Json parse_json(std::vector<char> payload);
};


#endif //SERVER_HTTP_UTILS_H

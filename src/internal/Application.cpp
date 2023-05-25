//
// Created by StudyHole on 23.05.2023.
//

#include "../pkg/http_utlis/http_utils.cpp"
#include "../pkg/shali_server/shali_server.cpp"
#include "Application.h"

HTTPResponse test_handler(HTTPPayload payload) {
    std::map <std::string, std::string> headers;
    //    //Content-Length:" + std::to_string(response_body.length()) +
    std::string response_str = "Hello from the C++ test handler";
    std::vector<char> data(response_str.begin(), response_str.end());
    headers["Content-Length"] = std::to_string(data.size());
    return HTTPResponse{"OK", 200, headers, data};
}

void Application::run() {
    ShaliServer server = ShaliServer("3490", 10);
    server.map_handler("/aboba", test_handler);
    server.http_listen();
}
/*
** server.c -- a stream socket server demo
*/
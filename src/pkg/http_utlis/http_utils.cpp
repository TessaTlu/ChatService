//
// Created by StudyHole on 21.05.2023.
//

#include "http_utils.h"
#include <iostream>
#include "../json/json.hpp"
#include "../utils/utils.cpp"


Json http_utils::parse_json(std::vector<char> payload) {
    std::string payload_str(reinterpret_cast<const char *>(&payload[0]), payload.size());
    Json j{payload_str};
    return j;
}

HTTPPayload http_utils::parse_http_payload(std::vector<char> payload) {
    std::string payload_str(reinterpret_cast<const char *>(&payload[0]), payload.size());
    std::map<std::string, std::string> headers;
    std::string method;
    std::string route;
    std::string headers_str;
    std::string response_str;
    std::cout << "_______________________________" << std::endl;
    auto first_line = payload_str.substr(0, payload_str.find(" HTTP"));
    method = first_line.substr(0, first_line.find('/'));
    route = first_line.substr(first_line.find(method) + method.length(), first_line.length() - 1);
    std::cout << "Method: " << method << std::endl;
    std::cout << "Route: " << route << std::endl;
    headers_str = payload_str.substr(payload_str.find('\n') + 1, payload_str.find("\n\n"));
    headers_str = headers_str.substr(0, headers_str.find_last_of("\n")-1);
    std::cout << "Headers:" << std::endl;
    auto headers_vector = utils::split(headers_str, "\n");
    for (auto &i: headers_vector) {
        auto header_name = i.substr(0, i.find(':'));
        if (header_name.empty()) continue;
        auto header_value = i.substr(i.find(':')+1, i.length()-1);
        headers[header_name] = header_value;
    }

    for (const auto &header: headers) {
        std::cout << header.first << ":" << header.second << "\n";
    }
    std::string body_str = payload_str.substr(payload_str.find_last_of("\n\n")+1, payload_str.length()-1);
    std::cout <<"body:" << body_str <<std::endl;
    std::cout << "__________________" << std::endl;
    std::vector<char> body(body_str.begin(), body_str.end());
    return HTTPPayload{route, method, headers, body};
}

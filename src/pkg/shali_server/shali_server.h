//
// Created by StudyHole on 22.05.2023.
//

#ifndef SERVER_SHALI_SERVER_H
#define SERVER_SHALI_SERVER_H
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <csignal>
#include <mutex>
#include <string>
#include <vector>
#include <map>
#include <functional>



class ShaliServer {
private:
    const char *port{};
    int connection_number{};
    int socketFileDescriptor{}, new_fd{};
    bool listen_status = false;
    std::map<std::string, std::function<HTTPResponse(HTTPPayload)>> handlers;
    void main_handler();
    HTTPResponse handle(HTTPPayload payload);
    static void *get_in_addr(struct sockaddr *sa);
    static void sigchld_handler(int s);
public:
    void map_handler(const std::string& route, std::function<HTTPResponse(HTTPPayload)> handler);
    explicit ShaliServer();
    explicit ShaliServer(const std::string& port, int connection_number_);
    void http_listen();
    void set_listen(bool listen_);
};


#endif //SERVER_SHALI_SERVER_H

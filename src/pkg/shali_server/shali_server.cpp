//
// Created by StudyHole on 22.05.2023.
//

#include "shali_server.h"
#include "../http_utlis/http_utils.h"
#include <string>
#include <utility>

ShaliServer::ShaliServer(const std::string &port_, int connection_number_) {
    port = port_.c_str();
    connection_number = connection_number_;
}

void ShaliServer::http_listen() {
    this->set_listen(true);
    struct addrinfo hints{}, *servinfo, *p;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    struct sigaction sa{};
    int yes=1;
    hints.ai_flags = AI_PASSIVE; // use my IP
    int rv;
    if ((rv = getaddrinfo(nullptr, port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return;
    }

    // loop through all the results and bind to the first we can
    for (p = servinfo; p != nullptr; p = p->ai_next) {
        if ((socketFileDescriptor = socket(p->ai_family, p->ai_socktype,
                                           p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(socketFileDescriptor, SOL_SOCKET, SO_REUSEADDR, &yes,
                       sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(socketFileDescriptor, p->ai_addr, p->ai_addrlen) == -1) {
            close(socketFileDescriptor);
            perror("server: bind");
            continue;
        }

        break;
    }

    if (p == nullptr) {
        fprintf(stderr, "server: failed to bind\n");
        return;
    }

    freeaddrinfo(servinfo);

    if (listen(socketFileDescriptor, connection_number) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, nullptr) == -1) {
        perror("sigaction");
        exit(1);
    }
    while (this->listen_status) {
        main_handler();
    }
}

void ShaliServer::main_handler() {
    struct sockaddr_storage their_addr{};
    char s[INET6_ADDRSTRLEN];
    socklen_t sin_size;
    sin_size = sizeof their_addr;
    std::vector<char> msg_buf;
    msg_buf.resize(5000);
    new_fd = accept(socketFileDescriptor, (struct sockaddr *) &their_addr, &sin_size);
    if (new_fd == -1) {
        perror("accept");
        return;
    }
    auto bytes = recv(new_fd, &msg_buf[0], msg_buf.size(), 0);
    if (bytes != -1) msg_buf.resize(bytes);
    auto http_payload = http_utils::parse_http_payload(msg_buf);
    auto client_payload = std::string(msg_buf.begin(), msg_buf.end());
    inet_ntop(their_addr.ss_family,
              get_in_addr((struct sockaddr *) &their_addr),
              s, sizeof s);
    auto http_response = handle(http_payload);
    std::cout<<(std::string(http_response.data.begin(), http_response.data.end()))<<std::endl;
    std::string headers_string;
    for (const auto& p : http_response.headers) {
        headers_string += p.first + ": " + p.second + "\n";
    }
    std::string response_string(http_response.data.begin(), http_response.data.end());
    std::string response =
            "HTTP/1.1 " + std::to_string(http_response.status_code) + " " + http_response.status + "\n" +  headers_string + "\n" + response_string;
    std::cout<<response<<std::endl;
    if (!fork()) {
        if (send(new_fd, response.c_str(), response.length(), 0) == -1)
            perror("send");
        printf("200");
    }
    close(new_fd);
}

void *ShaliServer::get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *) sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *) sa)->sin6_addr);
}

void ShaliServer::sigchld_handler(int s) {
    while (waitpid(-1, nullptr, WNOHANG) > 0);
}

void ShaliServer::set_listen(bool listen_) {
    listen_status = listen_;
}

void ShaliServer::map_handler(const std::string& route, std::function<HTTPResponse(HTTPPayload)> handler) {
    handlers[route] = std::move(handler);
}

HTTPResponse ShaliServer::handle(HTTPPayload payload) {
    std::vector<char> data;
    if (payload.route[payload.route.length()-1] == '/')
        payload.route=payload.route.substr(0, payload.route.length()-1);
    if (!handlers.count(payload.route)) return HTTPResponse{"404 not found", 404, {}, data};
    return handlers[payload.route](payload);
}

ShaliServer::ShaliServer() = default;

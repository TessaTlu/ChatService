//
// Created by StudyHole on 23.05.2023.
//
#include <vector>
#include <string>

struct Message {
    int UserID;
    int MessageID;
    std::string text;
};

struct Chat {
    std::vector<int> users;
    std::vector<Message> message_history;
};


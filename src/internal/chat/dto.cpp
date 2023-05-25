//
// Created by StudyHole on 23.05.2023.
//
#include <string>

struct SendMessageParams {
    int ChatID;
    std::string message;
};

struct SendMessageResponse {
    int status;
};
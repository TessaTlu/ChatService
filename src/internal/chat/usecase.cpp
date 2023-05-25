//
// Created by StudyHole on 23.05.2023.
//

#include "dto.cpp"

class UseCase {
public:
    virtual SendMessageResponse SendMessage(SendMessageParams params) = 0;
};
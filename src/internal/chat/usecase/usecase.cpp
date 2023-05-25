//
// Created by StudyHole on 23.05.2023.
//

#include "../usecase.cpp"

class UC : UseCase {
private:
public:
    SendMessageResponse SendMessage(SendMessageParams params);
};

SendMessageResponse UC::SendMessage(SendMessageParams params) {
    return {};
}

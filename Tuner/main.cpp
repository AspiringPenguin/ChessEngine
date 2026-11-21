#include <iostream>
#include <string>

int main() {
    std::string inp;
    std::getline(std::cin, inp);

    if (inp == "extractPositions") {
        std::cout << "Implemented in Python Project." << std::endl;
    }
    else if (inp == "tuneK") {
        std::cout << "Not implemented" << std::endl;
    }
    else if (inp == "tuneEval") {
        std::cout << "Not implemented" << std::endl;
    }
    else {
        std::cout << "Unrecognised command." << std::endl;
    }

    return 0;
}
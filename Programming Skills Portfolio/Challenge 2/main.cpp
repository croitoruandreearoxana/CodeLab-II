#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>

struct Joke {
    std::string setup;
    std::string punchline;
};

std::vector<Joke> loadJokes(const std::string& filename) {
    std::vector<Joke> jokes;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filename << "\n";
        return jokes;
    }

    std::string line;
    while (std::getline(file, line)) {
        size_t qmarkPos = line.find('?');
        if (qmarkPos != std::string::npos) {
            std::string setup = line.substr(0, qmarkPos + 1);
            std::string punchline = line.substr(qmarkPos + 1);
            jokes.push_back({setup, punchline});
        }
    }
    return jokes;
}

int main() {
    std::vector<Joke> jokes = loadJokes("randomJokes.txt");
    if (jokes.empty()) {
        std::cout << "No jokes found or unable to read randomJokes.txt\n";
        return 1;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(0, jokes.size() - 1);

    std::cout << "Type 'Alexa, tell me a joke' to hear a joke or 'quit' to exit.\n";

    std::string command;
    while (true) {
        std::cout << "\nYou: ";
        std::getline(std::cin, command);

        if (command == "quit" || command == "exit") {
            std::cout << "Alexa: Goodbye!\n";
            break;
        }

        if (command == "Alexa, tell me a joke") {
            Joke selected = jokes[dist(gen)];
            std::cout << "Alexa: " << selected.setup << "\n";
            std::cout << "[Press Enter to see punchline...]";
            std::string enterKey;
            std::getline(std::cin, enterKey);
            std::cout << "Alexa: " << selected.punchline << "\n";
        } else {
            std::cout << "Alexa: Please say 'Alexa, tell me a joke' or 'quit'.\n";
        }
    }

    return 0;
}
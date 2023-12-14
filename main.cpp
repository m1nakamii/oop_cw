#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <ncurses.h>

#define MAX_WORD_LENGTH 50

class NinjaGame {
public:
    NinjaGame();
    void run();
    ~NinjaGame();

private:
    void printWord(const char* word, const char* input);
    void playGame(int minLen, int maxLen, int numRounds);
    void updateHighScore(int score);

    std::vector<std::string> words;
    int numWords;
    char input[MAX_WORD_LENGTH];
};

NinjaGame::NinjaGame() : numWords(0) {
    initscr();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    wbkgd(stdscr, COLOR_PAIR(1));

    // Load words from a file
    std::ifstream file("ninja_txt/words.txt");
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file" << std::endl;
        endwin();
        exit(1);
    }

    std::string word;
    while (std::getline(file, word)) {
        words.push_back(word);
        numWords++;
    }

    file.close();

    srand(time(NULL));
}

void NinjaGame::printWord(const char* word, const char* input) {
    clear();
    printw("%s\n%s", word, input);
    refresh();
}

void NinjaGame::playGame(int minLen, int maxLen, int numRounds) {
    int score = 0;
    std::vector<bool> used(numWords, false);

    time_t startTime = time(nullptr);

    for (int i = 0; i < numRounds; i++) {
        int wordIndex;
        int len;

        do {
            wordIndex = rand() % numWords;
            len = words[wordIndex].length();
        } while (used[wordIndex] || len < minLen || len > maxLen);

        used[wordIndex] = true;
        printWord(words[wordIndex].c_str(), "");

        int ch, pos = 0;
        while (1) {
            ch = getch();
            if (ch == KEY_BACKSPACE || ch == 127) {
                if (pos > 0) {
                    pos--;
                    input[pos] = '\0';
                }
            } else if (ch == KEY_ENTER || ch == '\n') {
                if (strcasecmp(input, words[wordIndex].c_str()) == 0) {
                    score++;
                } else {
                    printw("Incorrect!\n");
                }
                break;
            } else if (isalpha(ch)) {
                if (pos < MAX_WORD_LENGTH - 1) {
                    input[pos] = ch;
                    pos++;
                    input[pos] = '\0';
                }
            }
            printWord(words[wordIndex].c_str(), input);
        }
    }

    time_t endTime = time(nullptr);
    double elapsedTime = difftime(endTime, startTime);
    int wordsPerMinute = static_cast<int>((score * 60.0) / elapsedTime);

    printw("Score: %d\n", score);
    printw("Time: %.2f seconds\n", elapsedTime);
    printw("Words per Minute: %d\n", wordsPerMinute);

    updateHighScore(score);
}

void NinjaGame::updateHighScore(int score) {
    // Read high score from file
    int highScore = 0;
    std::ifstream file("ninja_txt/highscore.txt");
    if (file.is_open()) {
        file >> highScore;
        file.close();
    }

    // Check if current score is higher than high score
    if (score > highScore) {
        // Update high score
        highScore = score;

        // Write new high score to file
        std::ofstream outFile("ninja_txt/highscore.txt");
        if (outFile.is_open()) {
            outFile << highScore;
            outFile.close();
        }

        // Display message about new high score
        printw("New high score: %d!\n", highScore);
    } else {
        // Display message about current high score
        printw("Current high score: %d\n", highScore);
    }

    // Wait for user input before exiting
    printw("\nPress any key to exit...");
    getch();
}

void NinjaGame::run() {
    printw("Welcome to Ninja Game!\n");
    printw("Try to type the displayed words quickly and accurately.\n");
    printw("Press Enter after typing each word.\n\n");

    printw("Select difficulty level:\n");
    printw("1. Easy\n");
    printw("2. Medium\n");
    printw("3. Hard\n");
    printw("5. Exit\n");
    int choice;
     while (1) {
        choice = getch() - '0';
        if (choice >= 1 && choice <= 3) {
            break;
        } else if (choice == 5) {
            endwin();
            exit(0);
        }
    }

    int minWordLength, maxWordLength, numrounds;

    switch (choice) {
        case 1:
            minWordLength = 3;
            maxWordLength = 8;
            numrounds = 10;
            break;
        case 2:
            minWordLength = 8;
            maxWordLength = 12;
            numrounds = 15;
            break;
        case 3:
            minWordLength = 12;
            maxWordLength = 20;
            numrounds = 20;
            break;
        default:
            minWordLength = 3;
            maxWordLength = 5;
    }
    printw("\n");
    printw("Get ready...\n");
    refresh();
    napms(2000); // Sleep for 2000 milliseconds (2 seconds)

    playGame(minWordLength, maxWordLength, numrounds);
}

NinjaGame::~NinjaGame() {
    endwin();
}

int main() {
    NinjaGame game;
    game.run();

    return 0;
}
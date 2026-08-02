#include "ConsoleUI.h"
#include <iostream>
using namespace std;

static void clearInputLine() {
    char ch = ' ';
    while (ch != '\n' && cin.get(ch)) {
    }
}

ConsoleUI::ConsoleUI() {
}

int ConsoleUI::readInt(int minValue, int maxValue) const {
    int value = minValue;
    bool valid = false;

    while (!valid) {
        cout << "> ";
        if (cin >> value) {
            if (value >= minValue && value <= maxValue) {
                valid = true;
            } else {
                cout << "Please enter a number between " << minValue
                     << " and " << maxValue << "." << endl;
            }
        } else {
            cin.clear();
            clearInputLine();
            cout << "Invalid input. Please enter a whole number." << endl;
        }
    }

    clearInputLine();
    return value;
}

string ConsoleUI::difficultyName(int difficulty) const {
    if (difficulty == 0) {
        return "EASY";
    }
    if (difficulty == 2) {
        return "HARD";
    }
    return "NORMAL";
}

int ConsoleUI::showMainMenu() const {
    cout << endl;
    cout << "===== QUIZ ARENA =====" << endl;
    cout << "1) New game" << endl;
    cout << "2) Saved games" << endl;
    cout << "3) View leaderboard" << endl;
    cout << "4) Quit" << endl;
    return readInt(1, 4);
}

int ConsoleUI::askDifficulty(const int lives[], const int targets[]) const {
    cout << endl;
    cout << "Choose difficulty:" << endl;

    cout << "1) Easy   (" << lives[0] << " lives, target " << targets[0] << ")" << endl;

    cout << "2) Normal (" << lives[1] << " lives, target " << targets[1] << ")" << endl;

    cout << "3) Hard   (" << lives[2] << " lives, target " << targets[2] << ")" << endl;

    return readInt(1, 3) - 1;
}
void ConsoleUI::showQuestion(const Question& question, int number, int total) const {
    cout << endl;
    cout << "Question " << number << " of " << total << ":" << endl;
    cout << question.getText() << endl;

    int count = question.getOptionCount();
    for (int i = 0; i < count; i++) {
        cout << "  " << (i + 1) << ") " << question.getOption(i) << endl;
    }
}

int ConsoleUI::askAnswerOrCommand(int optionCount) const {
    cout << "Enter your answer (1-" << optionCount
         << "), or 0 to save and return to the menu." << endl;
    int choice = readInt(0, optionCount);
    return (choice == 0) ? -1 : choice;
}

void ConsoleUI::showAnswerResult(bool correct, const string& correctText,
                                 int pointsGained, int livesRemaining) const {
    if (correct) {
        cout << "Correct! +" << pointsGained << " points." << endl;
    } else {
        cout << "Wrong! The correct answer was: " << correctText
             << ". You lost a life (lives left: " << livesRemaining << ")." << endl;
    }
}

void ConsoleUI::showStatus(const Player& player) const {
    cout << endl;
    cout << "[ " << player.getName()
         << " | Score: " << player.getScore()
         << " | Lives: " << player.getLives() << " ]" << endl;
}

void ConsoleUI::showGameOver(bool won, const Player& player, int targetScore) const {
    cout << endl;
    cout << "==============================" << endl;
    if (won) {
        cout << "YOU WIN! You reached the target score." << endl;
    } else {
        cout << "GAME OVER. You ran out of lives." << endl;
    }
    cout << "Final score: " << player.getScore()
         << " (target " << targetScore << ")" << endl;
    cout << "==============================" << endl;
}

void ConsoleUI::showLeaderboard(const Leaderboard& board) const {
    cout << endl;
    cout << "===== LEADERBOARD =====" << endl;

    if (board.getCount() == 0) {
        cout << "No scores yet. Be the first!" << endl;
        return;
    }

    vector<string> names;
    board.getSortedNames(names);

    cout << "#\tNAME\tEASY\tNORMAL\tHARD\tTOTAL" << endl;
    cout << "\t\tSCORE\tSCORE\tSCORE\tSCORE" << endl;

    for (int rank = 0; rank < (int)names.size(); rank++) {
        LeaderboardEntry entry;
        board.getEntry(names[rank], entry);

        cout << (rank + 1) << "\t" << names[rank];
        for (int mode = 0; mode < LEADERBOARD_MODE_COUNT; mode++) {
            cout << "\t";
            if (entry.getScore(mode) < 0) {
                cout << "X";
            } else {
                cout << entry.getScore(mode);
            }
        }
        cout << "\t" << entry.getTotalScore() << endl;
    }
}

void ConsoleUI::showSaveSlots(const SaveManager& saves,
                              const int totalQuestionsPerMode[SAVE_MODE_COUNT]) const {
    cout << endl;
    cout << "===== SAVED GAMES =====" << endl;

    if (saves.getCount() == 0) {
        cout << "No saved games." << endl;
        return;
    }

    for (int i = 0; i < saves.getCount(); i++) {
        const SaveSlot& playersave = saves.getSlot(i);
        cout << (i + 1) << ") " << playersave.getName() << endl;
        for (int mode = 0; mode < SAVE_MODE_COUNT; mode++) {
            cout << "   " << difficultyName(mode) << ": ";
            if (!playersave.hasMode(mode)) {
                cout << "X" << endl;
            } else {
                cout << "score " << playersave.getScore(mode)
                     << " | lives " << playersave.getLives(mode)
                     << " | Q " << (playersave.getCurrentIndex(mode) + 1) << "/"
                     << totalQuestionsPerMode[mode]
                     << endl;
            }
        }
    }
}

int ConsoleUI::askSavesAction() const {
    cout << endl;
    cout << "Choose an action:" << endl;
    cout << "1) Load a save" << endl;
    cout << "2) Delete a save" << endl;
    cout << "3) Back to menu" << endl;
    return readInt(1, 3);
}

int ConsoleUI::askSaveNumber(int count) const {
    cout << "Enter the save number (1-" << count << "), or 0 to cancel." << endl;
    return readInt(0, count);
}

int ConsoleUI::askContinueOrNew() const {
    cout << endl;
    cout << "Choose an action for this mode:" << endl;
    cout << "1) Continue saved progress" << endl;
    cout << "2) Start a new run" << endl;
    return readInt(1, 2);
}

string ConsoleUI::askSaveNameNotTaken(const SaveManager& saves) const {
    while (true) {
        cout << "Enter your name (this will also be your save name): ";
        string name;
        getline(cin, name);
        if (name.empty()) {
            name = "Player";
        }
        if (!saves.hasName(name)) {
            return name;
        }
        cout << "That name is already used by a saved game. Please choose another name." << endl;
    }
}

void ConsoleUI::showMessage(const string& message) const {
    cout << message << endl;
}

void ConsoleUI::showError(const string& message) const {
    cout << "ERROR: " << message << endl;
}


//new github test
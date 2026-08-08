#include "ConsoleUI.h"
#include <iostream>
using namespace std;

// Discard the rest of the current input line after reading a number.
static void clearInputLine() {
    char ch = ' ';
    while (ch != '\n' && cin.get(ch)) {
    }
}

ConsoleUI::ConsoleUI() {
}

// Read one integer in range and reject invalid input.
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

    // Leave cin ready for a later getline, such as the player name.
    clearInputLine();
    return value;
}

// Convert a difficulty index to the label shown in menus.
string ConsoleUI::difficultyName(int difficulty) const {
    if (difficulty == 0) {
        return "EASY";
    }
    if (difficulty == 2) {
        return "HARD";
    }
    return "NORMAL";
}

// Show the main menu and return the chosen option.
int ConsoleUI::showMainMenu() const {
    cout << endl;
    cout << "===== QUIZ ARENA =====" << endl;
    cout << "1) New game" << endl;
    cout << "2) Saved games" << endl;
    cout << "3) View leaderboard" << endl;
    cout << "4) Quit" << endl;
    return readInt(1, 4);
}

// Ask for a difficulty and return it as a 0 based index.
int ConsoleUI::askDifficulty(const int lives[], const int targets[]) const {
    // Missing arrays: do not print lives/targets, use Normal.
    if (lives == nullptr || targets == nullptr) {
        showError("Difficulty data is missing. Using Normal.");
        return 1; // Normal
    }

    cout << endl;
    cout << "Choose difficulty:" << endl;

    cout << "1) Easy   (" << lives[0] << " lives, target " << targets[0] << ")" << endl;

    cout << "2) Normal (" << lives[1] << " lives, target " << targets[1] << ")" << endl;

    cout << "3) Hard   (" << lives[2] << " lives, target " << targets[2] << ")" << endl;

    // Menu choices are 1 based. The rest of the game uses 0 based difficulty.
    return readInt(1, 3) - 1;
}

// Print one question and its options through the Question interface.
void ConsoleUI::showQuestion(const Question& question, int number, int total) const {
    cout << endl;
    cout << "Question " << number << " of " << total << ":" << endl;
    cout << question.getText() << endl;

    // Print options through the base interface so MC and TF use the same code.
    int count = question.getOptionCount();
    for (int i = 0; i < count; i++) {
        cout << "  " << (i + 1) << ") " << question.getOption(i) << endl;
    }
}

// Read an answer, or -1 if the player chose to save and leave.
int ConsoleUI::askAnswerOrCommand(int optionCount) const {
    cout << "Enter your answer (1-" << optionCount
         << "), or 0 to save and return to the menu." << endl;
    int choice = readInt(0, optionCount);
    return (choice == 0) ? -1 : choice;
}

// Tell the player whether the last answer was correct.
void ConsoleUI::showAnswerResult(bool correct, const string& correctText,
                                 int pointsGained, int livesRemaining) const {
    if (correct) {
        cout << "Correct! +" << pointsGained << " points." << endl;
    } else {
        cout << "Wrong! The correct answer was: " << correctText
             << ". You lost a life (lives left: " << livesRemaining << ")." << endl;
    }
}

// Print the current name, score, and lives.
void ConsoleUI::showStatus(const Player& player) const {
    cout << endl;
    cout << "[ " << player.getName()
         << " | Score: " << player.getScore()
         << " | Lives: " << player.getLives() << " ]" << endl;
}

// Print the final result of a finished run.
void ConsoleUI::showGameOver(bool won, const Player& player, int targetScore) const {
    cout << endl;
    cout << "==============================" << endl;

    // Three end states: win, out of lives, or finished below the target.
    if (won) {
        cout << "YOU WIN!" << endl;
        cout << "You completed all questions and reached the target score." << endl;
    } else if (!player.isAlive()) {
        cout << "YOU LOST!" << endl;
        cout << "You ran out of lives before completing all questions." << endl;
    } else {
        cout << "YOU LOST!" << endl;
        cout << "You completed all questions but did not reach the target score." << endl;
    }

    cout << "Final score: " << player.getScore()
         << " (target " << targetScore << ")" << endl;

    if (player.isAlive()) {
        cout << "Lives remaining: " << player.getLives() << endl;
    }

    if (won) {
        cout << "Great job!" << endl;
    } else {
        cout << "Better luck next time!" << endl;
    }

    cout << "==============================" << endl;
}

// Print the leaderboard sorted by total score.
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
        for (int difficulty = 0; difficulty < numOfDifficulties; difficulty++) {
            cout << "\t";
            // A negative score means this difficulty was never played.
            if (entry.getScore(difficulty) < 0) {
                cout << "X";
            } else {
                cout << entry.getScore(difficulty);
            }
        }
        cout << "\t" << entry.getTotalScore() << endl;
    }
}

// Print every saved player and the progress stored for each difficulty.
void ConsoleUI::showSaveSlots(const SaveManager& saves,
                              const int totalQuestionsPerDifficulty[numOfDifficulties]) const {
    cout << endl;
    cout << "===== SAVED GAMES =====" << endl;

    if (saves.getCount() == 0) {
        cout << "No saved games." << endl;
        return;
    }

    for (int i = 0; i < saves.getCount(); i++) {
        const SaveSlot& playerSave = saves.getSlot(i);
        cout << (i + 1) << ") " << playerSave.getName() << endl;

        for (int difficulty = 0; difficulty < numOfDifficulties; difficulty++) {
            cout << "   " << difficultyName(difficulty) << ": ";

            if (!playerSave.doneDifficulty(difficulty)) {
                cout << "X" << endl;
            } else {
                cout << "score " << playerSave.getScore(difficulty)
                     << " | lives " << playerSave.getLives(difficulty);

                // Missing array: still show score/lives, skip question progress.
                if (totalQuestionsPerDifficulty != nullptr) {
                    int currentQuestion = playerSave.getCurrentIndex(difficulty) + 1;
                    if (currentQuestion > totalQuestionsPerDifficulty[difficulty]) {
                        currentQuestion = totalQuestionsPerDifficulty[difficulty];
                    }

                    cout << " | Q " << currentQuestion << "/"
                         << totalQuestionsPerDifficulty[difficulty];
                }

                cout << endl;
            }
        }
    }
}

// Ask whether to load a save, delete a save, or go back.
int ConsoleUI::askSavesAction() const {
    cout << endl;
    cout << "Choose an action:" << endl;
    cout << "1) Load a save" << endl;
    cout << "2) Delete a save" << endl;
    cout << "3) Back to menu" << endl;
    return readInt(1, 3);
}

// Ask which save slot to use, or 0 to cancel.
int ConsoleUI::askSaveNumber(int count) const {
    cout << "Enter the save number (1-" << count << "), or 0 to cancel." << endl;
    return readInt(0, count);
}

// Ask whether to continue a saved difficulty or start that difficulty again.
int ConsoleUI::askContinueOrNew() const {
    cout << endl;
    cout << "Choose an action for this difficulty:" << endl;
    cout << "1) Continue saved progress" << endl;
    cout << "2) Start a new run" << endl;
    return readInt(1, 2);
}

// Ask for a player name that is not already used by a save.
string ConsoleUI::askSaveNameNotTaken(const SaveManager& saves) const {
    while (true) {
        cout << "Enter your name (this will also be your save name): ";
        string name;
        getline(cin, name);
        if (name.empty()) {
            name = "Player";
        }
        if (!saves.nameTaken(name)) {
            return name;
        }
        cout << "That name is already used by a saved game. Please choose another name." << endl;
    }
}

// Print a general message.
void ConsoleUI::showMessage(const string& message) const {
    cout << message << endl;
}

// Print an error message.
void ConsoleUI::showError(const string& message) const {
    cout << "ERROR: " << message << endl;
}

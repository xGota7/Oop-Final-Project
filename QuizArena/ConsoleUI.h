#ifndef CONSOLE_UI_H
#define CONSOLE_UI_H

#include "Question.h"
#include "Player.h"
#include "Leaderboard.h"
#include "SaveManager.h"
#include <string>
using namespace std;

// All terminal input and output for the game.
// Displays data from Question, Player, Leaderboard, and SaveManager.
// Contains no scoring rules and does not control the game loop.
class ConsoleUI {
private:
    int readInt(int minValue, int maxValue) const;
    string difficultyName(int difficulty) const;

public:
    ConsoleUI();

    int showMainMenu() const;
    int askDifficulty(const int lives[], const int targets[]) const;

    void showQuestion(const Question& question, int number, int total) const;

    // Returns a 1 based option index, or -1 to save and return to the menu.
    int askAnswerOrCommand(int optionCount) const;

    void showAnswerResult(bool correct, const string& correctText,
                          int pointsGained, int livesRemaining) const;
    void showStatus(const Player& player) const;
    void showGameOver(bool won, const Player& player, int targetScore) const;
    void showLeaderboard(const Leaderboard& board) const;

    void showSaveSlots(const SaveManager& saves,
                       const int totalQuestionsPerDifficulty [numOfDifficulties]) const;
    int askSavesAction() const;
    int askSaveNumber(int count) const;
    int askContinueOrNew() const;
    string askSaveNameNotTaken(const SaveManager& saves) const;

    void showMessage(const string& message) const;
    void showError(const string& message) const;
};

#endif

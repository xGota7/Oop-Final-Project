#ifndef QUIZ_GAME_H
#define QUIZ_GAME_H

#include "Question.h"
#include "Player.h"
#include "Leaderboard.h"
#include "ConsoleUI.h"
#include <string>
#include <vector>
using namespace std;

const int DIFFICULTY_EASY = 0;
const int DIFFICULTY_NORMAL = 1;
const int DIFFICULTY_HARD = 2;

class QuizGame {
private:
    vector<Question*> m_questions;

    Player m_player;
    Leaderboard m_leaderboard;

    int m_difficulty;
    int m_targetScore;
    int m_startLives;
    int m_currentIndex;

    void addQuestion(Question* question);
    void clearQuestions();
    void copyFrom(const QuizGame& other);

    void applyDifficulty(int difficulty);
    string questionFileForDifficulty(int difficulty) const;
    int countQuestionsInFile(const string& path) const;

    void startNewGame(ConsoleUI& ui);
    void openSavesMenu(ConsoleUI& ui);
    void playSession(ConsoleUI& ui);
    void saveCurrentGame(ConsoleUI& ui);
    bool updateSaveSlot();

    bool isWin() const;

public:
    QuizGame();
    QuizGame(const QuizGame& other);
    QuizGame& operator=(const QuizGame& other);
    ~QuizGame();

    bool loadQuestions(const string& path);
    bool loadQuestionsForDifficulty(int difficulty);

    void run(ConsoleUI& ui);

    int getQuestionCount() const;
    int getQuestionCountForDifficulty(int difficulty) const;
};

#endif

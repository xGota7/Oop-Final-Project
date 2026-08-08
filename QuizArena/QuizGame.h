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

// Game controller: rules, question list, player session, saves, and leaderboard.
// Questions are stored as Question* so different types share one container.
// All input and output is done through ConsoleUI.
class QuizGame {
private:
    static const int LIVES[3];
    static const int TARGETS[3];
    vector<Question*> m_questions; // owned pointers, freed by clearQuestions()

    Player m_player;
    Leaderboard m_leaderboard;
    bool m_leaderboardCorrupted; // true when loadFromFile failed

    int m_difficulty;
    int m_targetScore;
    int m_startLives;
    int m_questionIndex;

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

    // Rule of Three: this class owns heap memory through m_questions.
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

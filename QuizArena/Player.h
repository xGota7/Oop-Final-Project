#ifndef PLAYER_H
#define PLAYER_H

#include <string>
using namespace std;

// Session state of the human player: name, score, and remaining lives.
// QuizGame owns the rules. ConsoleUI owns all input and output.
class Player {
private:
    string m_name;
    int m_score;
    int m_lives;

public:
    Player();
    Player(const string& name, int lives);

    void setName(const string& name);
    void setScore(int score);
    void setLives(int lives);

    void addScore(int points);
    void loseLife();

    const string& getName() const;
    int getScore() const;
    int getLives() const;
    bool isAlive() const;
};

#endif

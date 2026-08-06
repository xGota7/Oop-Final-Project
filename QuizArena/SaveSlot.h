#ifndef SAVE_SLOT_H
#define SAVE_SLOT_H

#include <string>
using namespace std;

const int numOfDifficulties = 3;  

// One player's saved progress for Easy, Normal, and Hard.
// Each difficulty can be missing, so a player may have data in only some difficulties.
class SaveSlot {
private:
    string m_name;
    bool m_doneDifficulty[numOfDifficulties];
    int m_score[numOfDifficulties];
    int m_lives[numOfDifficulties];
    int m_currentIndex[numOfDifficulties];

public:
    SaveSlot();

    void setName(const string& name);
    const string& getName() const;

    bool doneDifficulty(int difficulty) const;
    void setDoneDifficulty(int difficulty, bool done);

    int getScore(int difficulty) const;
    void setScore(int difficulty, int score);
    int getLives(int difficulty) const;
    void setLives(int difficulty, int lives);
    int getCurrentIndex(int difficulty) const;
    void setCurrentIndex(int difficulty, int currentIndex);
};

#endif

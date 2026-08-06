#include "SaveSlot.h"

// Create an empty slot with no progress in any difficulty.
SaveSlot::SaveSlot() {
    m_name = "";
    for (int i = 0; i < numOfDifficulties; i++) {
        m_doneDifficulty[i] = false;
        m_score[i] = 0;
        m_lives[i] = 0;
        m_currentIndex[i] = 0;
    }
}

// Set the player name stored in this slot.
void SaveSlot::setName(const string& name) {
    m_name = name;
}

// Return the player name stored in this slot.
const string& SaveSlot::getName() const {
    return m_name;
}

// Return true if this slot has saved progress for the given difficulty.
bool SaveSlot::doneDifficulty(int difficulty) const {
    return m_doneDifficulty[difficulty];
}

// Mark whether this slot contains progress for the given difficulty.
void SaveSlot::setDoneDifficulty(int difficulty, bool done) {
    m_doneDifficulty[difficulty] = done;
}

// Return the saved score for one difficulty.
int SaveSlot::getScore(int difficulty) const {
    return m_score[difficulty];
}

// Store the score for one difficulty.
void SaveSlot::setScore(int difficulty, int score) {
    m_score[difficulty] = score;
}

// Return the saved lives for one difficulty.
int SaveSlot::getLives(int difficulty) const {
    return m_lives[difficulty];
}

// Store the lives for one difficulty.
void SaveSlot::setLives(int difficulty, int lives) {
    m_lives[difficulty] = lives;
}

// Return the next question index saved for one difficulty.
int SaveSlot::getCurrentIndex(int difficulty) const {
    return m_currentIndex[difficulty];
}

// Store the next question index for one difficulty.
void SaveSlot::setCurrentIndex(int difficulty, int currentIndex) {
    m_currentIndex[difficulty] = currentIndex;
}

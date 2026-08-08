#include "LeaderboardEntry.h"

static const int NOT_PLAYED = -1;

// Create an entry with no scores in any difficulty.
LeaderboardEntry::LeaderboardEntry()
    : m_lastOrder(0) {
    for (int difficulty = 0; difficulty < LeaderNumOfDifficulties; difficulty++) {
        m_scores[difficulty] = NOT_PLAYED;
    }
}

// Return the best score stored for one difficulty.
int LeaderboardEntry::getScore(int difficulty) const {
    return m_scores[difficulty];
}

// Store the best score for one difficulty.
void LeaderboardEntry::setScore(int difficulty, int score) {
    m_scores[difficulty] = score;
}

// Return when this player last finished a counted game.
int LeaderboardEntry::getLastOrder() const {
    return m_lastOrder;
}

// Store the order value used to break ties.
void LeaderboardEntry::setLastOrder(int order) {
    m_lastOrder = order;
}

// Sum only difficulties that were actually played.
int LeaderboardEntry::getTotalScore() const {
    int total = 0;
    for (int difficulty = 0; difficulty < LeaderNumOfDifficulties; difficulty++) {
        if (m_scores[difficulty] > NOT_PLAYED) {
            total += m_scores[difficulty];
        }
    }
    return total;
}

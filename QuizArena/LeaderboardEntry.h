#ifndef LEADERBOARD_ENTRY_H
#define LEADERBOARD_ENTRY_H

#include <string>
using namespace std;

const int LeaderNumOfDifficulties = 3;

// Best score a single player reached in each difficulty.
// A score of -1 means the player has never played that difficulty.
// m_lastOrder is used only to break ties between equal totals.
class LeaderboardEntry {
private:
    int m_scores[LeaderNumOfDifficulties];
    int m_lastOrder;

public:
    LeaderboardEntry();

    int getScore(int difficulty) const;
    void setScore(int difficulty, int score);

    int getLastOrder() const;
    void setLastOrder(int order);

    int getTotalScore() const;
};

#endif

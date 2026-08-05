#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

const int LEADERBOARD_MODE_COUNT = 3;

// Best score a single player reached in each difficulty mode.
// A score of -1 means the player has never played that mode.
// m_lastOrder is used only to break ties between equal totals.
class LeaderboardEntry {
private:
    int m_scores[LEADERBOARD_MODE_COUNT];
    int m_lastOrder;

public:
    LeaderboardEntry();

    int getScore(int mode) const;
    void setScore(int mode, int score);

    int getLastOrder() const;
    void setLastOrder(int order);

    int getTotalScore() const;
};

// Stores every player's best score per mode, keyed by player name.
// unordered_map is used because lookup and update by name is the main operation.
class Leaderboard {
private:
    unordered_map<string, LeaderboardEntry> m_entries;
    int m_nextOrder;

public:
    Leaderboard();

    void submitResult(const string& name, int score, int mode);

    bool saveToFile(const string& path) const;
    bool loadFromFile(const string& path);

    int getCount() const;

    // Fill names with players sorted by total score, then by most recent play.
    void getSortedNames(vector<string>& names) const;

    // Copy one player's data into out. Returns false if the name is unknown.
    bool getEntry(const string& name, LeaderboardEntry& out) const;
};

#endif

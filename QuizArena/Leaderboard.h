#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

const int LEADERBOARD_MODE_COUNT = 3;

// The best score a single player reached in each difficulty mode.
// A score of -1 means the player has never played that mode.
// m_lastOrder records when the player last finished a game, used only to
// break ties between equal totals on the leaderboard.
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

// Keeps every player's best score per mode, keyed by the player's name.
// A hash map is used because the main operation is "find this player by name
// and keep their better result", which the map does without a manual search.
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

    // Fills 'names' with player names sorted for display: highest total first,
    // ties broken by whoever played most recently.
    void getSortedNames(vector<string>& names) const;

    // Copies one player's data into 'out'. Returns false if the name is unknown.
    bool getEntry(const string& name, LeaderboardEntry& out) const;
};

#endif

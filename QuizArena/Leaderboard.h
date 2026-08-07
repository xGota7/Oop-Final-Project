#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include "LeaderboardEntry.h"
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

// Stores every player's best score per difficulty, keyed by player name.
// unordered_map is used because lookup and update by name is the main operation.
class Leaderboard {
private:
    unordered_map<string, LeaderboardEntry> m_entries;
    int m_nextOrder;

public:
    Leaderboard();

    void submitResult(const string& name, int score, int difficulty);

    bool saveToFile(const string& path) const;
    bool loadFromFile(const string& path);

    int getCount() const;

    // Fill names with players sorted by total score, then by most recent play.
    void getSortedNames(vector<string>& names) const;

    // Copy one player's data into foundEntry. Returns false if the name is unknown.
    bool getEntry(const string& name, LeaderboardEntry& foundEntry) const;
};

#endif

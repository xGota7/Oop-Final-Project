#include "Leaderboard.h"
#include <fstream>

// Create an empty leaderboard.
Leaderboard::Leaderboard() {
    m_nextOrder = 0;
}

// Keep the better score for this difficulty and mark the player as most recent.
void Leaderboard::submitResult(const string& name, int score, int difficulty) {
    if (difficulty < 0 || difficulty >= LeaderNumOfDifficulties) {
        return;
    }

    m_nextOrder++;

    unordered_map<string, LeaderboardEntry>::iterator foundPlayerEntry = m_entries.find(name);
    if (foundPlayerEntry == m_entries.end()) {
        LeaderboardEntry entry;
        entry.setScore(difficulty, score);
        entry.setLastOrder(m_nextOrder);
        m_entries[name] = entry;
        return;
    }

    foundPlayerEntry->second.setLastOrder(m_nextOrder);
    if (score > foundPlayerEntry->second.getScore(difficulty)) {
        foundPlayerEntry->second.setScore(difficulty, score);
    }
}

// Write every leaderboard entry to a text file.
bool Leaderboard::saveToFile(const string& path) const {
    ofstream out(path.c_str());
    if (!out.is_open()) {
        return false;
    }
    unordered_map<string, LeaderboardEntry>::const_iterator currentPlayerEntry;
    for (currentPlayerEntry = m_entries.begin(); currentPlayerEntry != m_entries.end(); ++currentPlayerEntry) {
        out << "ENTRY" << '\n'
            << currentPlayerEntry->first << '\n'
            << currentPlayerEntry->second.getLastOrder();
        for (int difficulty = 0; difficulty < LeaderNumOfDifficulties; difficulty++) {
            out << " " << currentPlayerEntry->second.getScore(difficulty);
        }
        out << '\n' << "END" << '\n';
    }
    out.close();
    return true;
}

// Load leaderboard entries from a text file.
bool Leaderboard::loadFromFile(const string& path) {
    ifstream in(path.c_str());
    if (!in.is_open()) {
        return false;
    }

    m_entries.clear();
    m_nextOrder = 0;

    string line;
    while (getline(in, line)) {
        if (line != "ENTRY") {
            continue;
        }

        string name;
        if (!getline(in, name)) {
            break;
        }

        int lastOrder = 0;
        if (!(in >> lastOrder)) {
            break;
        }

        LeaderboardEntry entry;
        entry.setLastOrder(lastOrder);

        bool ok = true;
        for (int difficulty = 0; difficulty < LeaderNumOfDifficulties; difficulty++) {
            int score = 0;
            if (!(in >> score)) {
                ok = false;
                break;
            }
            entry.setScore(difficulty, score);
        }

        string endMark;
        if (!ok || !(in >> endMark) || endMark != "END") {
            break;
        }

        m_entries[name] = entry;
        if (lastOrder > m_nextOrder) {
            m_nextOrder = lastOrder;
        }
    }

    in.close();
    return true;
}

// Return how many players are on the leaderboard.
int Leaderboard::getCount() const {
    return (int)m_entries.size();
}

// Sort player names by total score, then by most recent play.
void Leaderboard::getSortedNames(vector<string>& names) const {
    names.clear();

    unordered_map<string, LeaderboardEntry>::const_iterator currentPlayerEntry;
    for (currentPlayerEntry = m_entries.begin(); currentPlayerEntry != m_entries.end(); ++currentPlayerEntry) {
        names.push_back(currentPlayerEntry->first);
    }

    for (int i = 0; i < (int)names.size() - 1; i++) {
        int best = i;
        for (int j = i + 1; j < (int)names.size(); j++) {
            const LeaderboardEntry& candidate = m_entries.find(names[j])->second;
            const LeaderboardEntry& leadingPlayer = m_entries.find(names[best])->second;

            if (candidate.getTotalScore() > leadingPlayer.getTotalScore()
                || (candidate.getTotalScore() == leadingPlayer.getTotalScore()
                    && candidate.getLastOrder() > leadingPlayer.getLastOrder())) {
                best = j;
            }
        }
        if (best != i) {
            string tmp = names[i];
            names[i] = names[best];
            names[best] = tmp;
        }
    }
}

// Copy one entry by name into foundEntry.
bool Leaderboard::getEntry(const string& name, LeaderboardEntry& foundEntry) const {
    unordered_map<string, LeaderboardEntry>::const_iterator foundPlayerEntry = m_entries.find(name);
    if (foundPlayerEntry == m_entries.end()) {
        return false;
    }
    foundEntry = foundPlayerEntry->second;
    return true;
}

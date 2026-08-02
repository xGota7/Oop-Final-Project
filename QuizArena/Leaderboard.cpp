#include "Leaderboard.h"
#include <fstream>

static const int NOT_PLAYED = -1;

LeaderboardEntry::LeaderboardEntry() {
    m_lastOrder = 0;
    for (int mode = 0; mode < LEADERBOARD_MODE_COUNT; mode++) {
        m_scores[mode] = NOT_PLAYED;
    }
}

int LeaderboardEntry::getScore(int mode) const {
    return m_scores[mode];
}

void LeaderboardEntry::setScore(int mode, int score) {
    m_scores[mode] = score;
}

int LeaderboardEntry::getLastOrder() const {
    return m_lastOrder;
}

void LeaderboardEntry::setLastOrder(int order) {
    m_lastOrder = order;
}

int LeaderboardEntry::getTotalScore() const {
    int total = 0;
    for (int mode = 0; mode < LEADERBOARD_MODE_COUNT; mode++) {
        if (m_scores[mode] > NOT_PLAYED) {
            total += m_scores[mode];
        }
    }
    return total;
}

Leaderboard::Leaderboard() {
    m_nextOrder = 0;
}

void Leaderboard::submitResult(const string& name, int score, int mode) {
    if (mode < 0 || mode >= LEADERBOARD_MODE_COUNT) {
        return;
    }

    m_nextOrder++;

    unordered_map<string, LeaderboardEntry>::iterator foundPlayerEntry = m_entries.find(name);
    if (foundPlayerEntry == m_entries.end()) {
        LeaderboardEntry entry;
        entry.setScore(mode, score);
        entry.setLastOrder(m_nextOrder);
        m_entries[name] = entry;
        return;
    }

    foundPlayerEntry->second.setLastOrder(m_nextOrder);
    if (score > foundPlayerEntry->second.getScore(mode)) {
        foundPlayerEntry->second.setScore(mode, score);
    }
}

bool Leaderboard::saveToFile(const string& path) const {
    ofstream out(path.c_str());
    if (!out.is_open()) {
        return false;
    }

    unordered_map<string, LeaderboardEntry>::const_iterator currentPlayerEntry;
    for (currentPlayerEntry = m_entries.begin(); currentPlayerEntry != m_entries.end(); ++currentPlayerEntry) {
        out << "ENTRY" << '\n'
            << currentPlayerEntry->first << '\n'
            << currentPlayerEntry->second.getLastOrder() << '\n';
        for (int mode = 0; mode < LEADERBOARD_MODE_COUNT; mode++) {
            out << currentPlayerEntry->second.getScore(mode) << '\n';
        }
        out << "END" << '\n';
    }

    out.close();
    return true;
}

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

        int last = 0;
        if (!(in >> last)) {
            break;
        }

        LeaderboardEntry entry;
        entry.setLastOrder(last);

        bool ok = true;
        for (int mode = 0; mode < LEADERBOARD_MODE_COUNT; mode++) {
            int score = 0;
            if (!(in >> score)) {
                ok = false;
                break;
            }
            entry.setScore(mode, score);
        }

        string endMark;
        if (!ok || !(in >> endMark) || endMark != "END") {
            break;
        }

        m_entries[name] = entry;
        if (last > m_nextOrder) {
            m_nextOrder = last;
        }
    }

    in.close();
    return true;
}

int Leaderboard::getCount() const {
    return (int)m_entries.size();
}

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
            const LeaderboardEntry& current = m_entries.find(names[best])->second;

            if (candidate.getTotalScore() > current.getTotalScore()
                || (candidate.getTotalScore() == current.getTotalScore()
                    && candidate.getLastOrder() > current.getLastOrder())) {
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

bool Leaderboard::getEntry(const string& name, LeaderboardEntry& out) const {
    unordered_map<string, LeaderboardEntry>::const_iterator foundPlayerEntry = m_entries.find(name);
    if (foundPlayerEntry == m_entries.end()) {
        return false;
    }
    out = foundPlayerEntry->second;
    return true;
}

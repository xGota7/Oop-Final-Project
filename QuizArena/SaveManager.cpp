#include "SaveManager.h"
#include <fstream>

static const char* MODE_LABELS[SAVE_MODE_COUNT] = {"EASY", "NORMAL", "HARD"};

// Create an empty slot with no progress in any mode.
SaveSlot::SaveSlot() {
    m_name = "";
    for (int i = 0; i < SAVE_MODE_COUNT; i++) {
        m_hasMode[i] = false;
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

// Return true if this slot has saved progress for the given mode.
bool SaveSlot::hasMode(int mode) const {
    return m_hasMode[mode];
}

// Mark whether this slot contains progress for the given mode.
void SaveSlot::setHasMode(int mode, bool has) {
    m_hasMode[mode] = has;
}

// Return the saved score for one mode.
int SaveSlot::getScore(int mode) const {
    return m_score[mode];
}

// Store the score for one mode.
void SaveSlot::setScore(int mode, int score) {
    m_score[mode] = score;
}

// Return the saved lives for one mode.
int SaveSlot::getLives(int mode) const {
    return m_lives[mode];
}

// Store the lives for one mode.
void SaveSlot::setLives(int mode, int lives) {
    m_lives[mode] = lives;
}

// Return the next question index saved for one mode.
int SaveSlot::getCurrentIndex(int mode) const {
    return m_currentIndex[mode];
}

// Store the next question index for one mode.
void SaveSlot::setCurrentIndex(int mode, int currentIndex) {
    m_currentIndex[mode] = currentIndex;
}

// Remember which file this manager reads and writes.
SaveManager::SaveManager(const string& path) {
    m_path = path;
}

// Read one difficulty block from the save file into a slot.
static bool readModeBlock(ifstream& in, SaveSlot& slot, int mode) {
    string label;
    int active = 0;

    if (!(in >> label) || label != MODE_LABELS[mode]) {
        return false;
    }
    if (!(in >> active)) {
        return false;
    }

    slot.setHasMode(mode, active != 0);
    if (active == 0) {
        return true;
    }

    int score = 0;
    int lives = 0;
    int currentIndex = 0;
    if (!(in >> score) || !(in >> lives) || !(in >> currentIndex)) {
        return false;
    }

    slot.setScore(mode, score);
    slot.setLives(mode, lives);
    slot.setCurrentIndex(mode, currentIndex);
    return true;
}

// Load every SAVE block from the file into memory.
bool SaveManager::load() {
    m_slots.clear();

    ifstream in(m_path.c_str());
    if (!in.is_open()) {
        return false;
    }

    string line;
    while (getline(in, line)) {
        if (line != "SAVE") {
            continue;
        }

        SaveSlot slot;
        string name;
        string endMark;

        if (!getline(in, name)) {
            break;
        }
        slot.setName(name);

        bool ok = true;
        for (int mode = 0; mode < SAVE_MODE_COUNT && ok; mode++) {
            ok = readModeBlock(in, slot, mode);
        }
        if (!ok || !(in >> endMark) || endMark != "END") {
            break;
        }

        m_slots.push_back(slot);
    }

    in.close();
    return true;
}

// Write every in memory slot back to the save file.
bool SaveManager::store() const {
    ofstream out(m_path.c_str());
    if (!out.is_open()) {
        return false;
    }

    for (int i = 0; i < (int)m_slots.size(); i++) {
        const SaveSlot& playerSlot = m_slots[i];
        out << "SAVE" << '\n' << playerSlot.getName() << '\n';

        for (int mode = 0; mode < SAVE_MODE_COUNT; mode++) {
            out << MODE_LABELS[mode] << " "
                << (playerSlot.hasMode(mode) ? 1 : 0);

            if (playerSlot.hasMode(mode)) {
                out << " " << playerSlot.getScore(mode)
                    << " " << playerSlot.getLives(mode)
                    << " " << playerSlot.getCurrentIndex(mode);
            }

            out << '\n';
        }

        out << "END" << '\n';
    }

    out.close();
    return true;
}

// Return how many saved players are stored.
int SaveManager::getCount() const {
    return (int)m_slots.size();
}

// Return one saved player by index.
const SaveSlot& SaveManager::getSlot(int index) const {
    return m_slots[index];
}

// Return true if a save already uses this player name.
bool SaveManager::hasName(const string& name) const {
    return findIndexByName(name) >= 0;
}

// Find a save by player name, or return -1 if it does not exist.
int SaveManager::findIndexByName(const string& name) const {
    for (int i = 0; i < (int)m_slots.size(); i++) {
        if (m_slots[i].getName() == name) {
            return i;
        }
    }
    return -1;
}

// Insert a new slot or replace the existing slot with the same name.
void SaveManager::upsertSlot(const SaveSlot& slot) {
    int index = findIndexByName(slot.getName());
    if (index >= 0) {
        m_slots[index] = slot;
    } else {
        m_slots.push_back(slot);
    }
}

// Remove one save slot by index.
bool SaveManager::removeSlot(int index) {
    if (index < 0 || index >= (int)m_slots.size()) {
        return false;
    }
    m_slots.erase(m_slots.begin() + index);
    return true;
}

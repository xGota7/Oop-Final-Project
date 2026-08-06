#include "SaveManager.h"
#include <fstream>

static const char* DIFFICULTY_LABELS[numOfDifficulties] = {"EASY", "NORMAL", "HARD"};

// Remember which file this manager reads and writes.
SaveManager::SaveManager(const string& path) {
    m_path = path;
}

// Read one difficulty block from the save file into a slot.
static bool readDifficultyBlock(ifstream& in, SaveSlot& slot, int difficulty) {
    string label;
    int active = 0;

    if (!(in >> label) || label != DIFFICULTY_LABELS[difficulty]) {
        return false;
    }
    if (!(in >> active)) {
        return false;
    }

    slot.setDoneDifficulty(difficulty, active != 0);
    if (active == 0) {
        return true;
    }

    int score = 0;
    int lives = 0;
    int currentIndex = 0;
    if (!(in >> score) || !(in >> lives) || !(in >> currentIndex)) {
        return false;
    }

    slot.setScore(difficulty, score);
    slot.setLives(difficulty, lives);
    slot.setCurrentIndex(difficulty, currentIndex);
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
        for (int difficulty = 0; difficulty < numOfDifficulties && ok; difficulty++) {
            ok = readDifficultyBlock(in, slot, difficulty);
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

        for (int difficulty = 0; difficulty < numOfDifficulties; difficulty++) {
            out << DIFFICULTY_LABELS[difficulty] << " "
                << (playerSlot.doneDifficulty(difficulty) ? 1 : 0);

            if (playerSlot.doneDifficulty(difficulty)) {
                out << " " << playerSlot.getScore(difficulty)
                    << " " << playerSlot.getLives(difficulty)
                    << " " << playerSlot.getCurrentIndex(difficulty);
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
bool SaveManager::nameTaken(const string& name) const {
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
void SaveManager::update_insertSlot(const SaveSlot& slot) {
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

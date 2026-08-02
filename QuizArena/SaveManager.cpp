#include "SaveManager.h"
#include <fstream>

static const char* MODE_LABELS[SAVE_MODE_COUNT] = {"EASY", "NORMAL", "HARD"};

SaveSlot::SaveSlot() {
    m_name = "";
    for (int i = 0; i < SAVE_MODE_COUNT; i++) {
        m_hasMode[i] = false;
        m_score[i] = 0;
        m_lives[i] = 0;
        m_currentIndex[i] = 0;
    }
}

void SaveSlot::setName(const string& name) {
    m_name = name;
}

const string& SaveSlot::getName() const {
    return m_name;
}

bool SaveSlot::hasMode(int mode) const {
    return m_hasMode[mode];
}

void SaveSlot::setHasMode(int mode, bool has) {
    m_hasMode[mode] = has;
}

int SaveSlot::getScore(int mode) const {
    return m_score[mode];
}

void SaveSlot::setScore(int mode, int score) {
    m_score[mode] = score;
}

int SaveSlot::getLives(int mode) const {
    return m_lives[mode];
}

void SaveSlot::setLives(int mode, int lives) {
    m_lives[mode] = lives;
}

int SaveSlot::getCurrentIndex(int mode) const {
    return m_currentIndex[mode];
}

void SaveSlot::setCurrentIndex(int mode, int currentIndex) {
    m_currentIndex[mode] = currentIndex;
}

SaveManager::SaveManager(const string& path) {
    m_path = path;
}

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

int SaveManager::getCount() const {
    return (int)m_slots.size();
}

const SaveSlot& SaveManager::getSlot(int index) const {
    return m_slots[index];
}

bool SaveManager::hasName(const string& name) const {
    return findIndexByName(name) >= 0;
}

int SaveManager::findIndexByName(const string& name) const {
    for (int i = 0; i < (int)m_slots.size(); i++) {
        if (m_slots[i].getName() == name) {
            return i;
        }
    }
    return -1;
}

void SaveManager::upsertSlot(const SaveSlot& slot) {
    int index = findIndexByName(slot.getName());
    if (index >= 0) {
        m_slots[index] = slot;
    } else {
        m_slots.push_back(slot);
    }
}

bool SaveManager::removeSlot(int index) {
    if (index < 0 || index >= (int)m_slots.size()) {
        return false;
    }
    m_slots.erase(m_slots.begin() + index);
    return true;
}

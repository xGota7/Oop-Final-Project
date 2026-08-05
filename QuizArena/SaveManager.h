#ifndef SAVE_MANAGER_H
#define SAVE_MANAGER_H

#include <string>
#include <vector>
using namespace std;

const int SAVE_MODE_COUNT = 3;

// One player's saved progress for Easy, Normal, and Hard.
// Each mode can be missing, so a player may have data in only some modes.
class SaveSlot {
private:
    string m_name;
    bool m_hasMode[SAVE_MODE_COUNT];
    int m_score[SAVE_MODE_COUNT];
    int m_lives[SAVE_MODE_COUNT];
    int m_currentIndex[SAVE_MODE_COUNT];

public:
    SaveSlot();

    void setName(const string& name);
    const string& getName() const;

    bool hasMode(int mode) const;
    void setHasMode(int mode, bool has);

    int getScore(int mode) const;
    void setScore(int mode, int score);
    int getLives(int mode) const;
    void setLives(int mode, int lives);
    int getCurrentIndex(int mode) const;
    void setCurrentIndex(int mode, int currentIndex);
};

// Loads and stores all save slots from a text file.
// QuizGame uses this class for persistence. It does no input or output to the console.
class SaveManager {
private:
    vector<SaveSlot> m_slots;
    string m_path;

public:
    SaveManager(const string& path);

    bool load();
    bool store() const;

    int getCount() const;
    const SaveSlot& getSlot(int index) const;

    bool hasName(const string& name) const;
    int findIndexByName(const string& name) const;

    void upsertSlot(const SaveSlot& slot);
    bool removeSlot(int index);
};

#endif

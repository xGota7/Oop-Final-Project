#ifndef SAVE_MANAGER_H
#define SAVE_MANAGER_H

#include "SaveSlot.h"
#include <string>
#include <vector>
using namespace std;

// Loads and stores all save slots from a text file.
// QuizGame uses this class for persistence. It does no input or output to the console.
class SaveManager {
private:
    vector<SaveSlot> m_slots;
    string m_path;

public:
    SaveManager(const string& path);

    bool load();  // false when save file content is corrupted
    bool store() const; 

    int getCount() const; 
    const SaveSlot& getSlot(int index) const;  

    bool nameTaken(const string& name) const; 
    int findIndexByName(const string& name) const;  

    void update_insertSlot(const SaveSlot& slot); 
    bool removeSlot(int index);
};

#endif

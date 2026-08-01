# Prompt Log — Quiz Arena

Chronological log of prompts sent to **Cursor** during development. Each entry: what we asked, what the agent said, our decision.

We used Cursor in Plan mode for design and in Agent mode for implementation. All AI use is logged here as required by the assignment.

---

## Prompt 1 — Plan mode, first UML

**We asked:**
> We want a text quiz game in C++. Before code, sketch a minimal class design. Classes: Question abstract base, 
MultipleChoiceQuestion, Player, SaveSlot, LeaderboardEntry, QuizGame, ConsoleUI. Separate UI from game logic. Use STL, no globals.

**Cursor said:**
Suggested the hierarchy in Version 1 of UML_DIAGRAM.md. Said QuizGame should hold questions in a vector of base pointers. 
Said SaveSlot and LeaderboardEntry are enough for now without manager classes.

**Decision: ACCEPTED** — we used this as Step 1.


---

## Prompt 3 — display inside Question

**Cursor suggested:**
Put a display method on Question that prints with cout.

**We asked:**
> Why put cout inside Question? We want UI separate.

**Cursor said:**
Agreed. ConsoleUI should read question data and print it.

**Decision: REJECTED** — placing cout inside Question would mix the model with the user interface. 
We kept all display operations inside ConsoleUI

---

## Prompt 7 — scoring

**Cursor suggested:**
Subtract points when the answer is wrong.

**We asked:**
> Can we only lose a life on wrong answer and keep score going up?

**Cursor said:**
Confirmed that wrong answers could reduce lives without changing the score and changed the game logic accordingly.

**Decision: MODIFIED** — wrong answer costs one life only. +10 per correct answer.

---

## Prompt 9 — Rule of Three

**We asked:**
> QuizGame owns dynamically allocated questions through vector<Question*>. 
Is a destructor enough, or do we also need a copy constructor and assignment operator? 
Explain how deep copying should work when the vector contains different derived question types.

**Cursor said:**
Yes. Add virtual clone in each question type. Implement destructor, copy ctor, and operator= on QuizGame.

**Decision: ACCEPTED**

---

## Prompt 12 — SaveManager

**We asked:**
> Add SaveManager. It should list saves, load a selected save, delete a selected save, 
and write the save data to saves.txt.

**Cursor suggested:**
Implemented the save manager but initially used atoi() to convert strings read from the file into integers.

**Our follow up:**
We did not learn atoi() in the course. Replace it with direct file-stream reading using >>.

**Decision: MODIFIED** —we kept the save-manager design but replaced atoi() 
with the stream extraction operator because it matched the techniques taught in the course.

---

## Prompt 15 — Leaderboard class

**We asked:**
> Add Leaderboard class. Store entries in unordered_map by player name. Save and load from file.

**We asked Cursor:**
> Why map and not vector for leaderboard?

**Cursor said:**
earching by player name is the main leaderboard operation. 
An unordered_map allows entries to be accessed using the player name as a key, 
while a vector would require searching the entries one by one

**Decision: ACCEPTED** — unordered_map for Leaderboard. vector stays for questions and save list.

---

## Prompt 16 — LeaderboardRow

**Cursor suggested:**
Add LeaderboardRow class for display rows.

**We asked:**
> Do we need a third class? Map already has name and entry.

**Decision: REJECTED** — the class would duplicate information already stored in the unordered_map. 
ConsoleUI can display the existing entries without another data class

---

## Prompt 20 — timer on leaderboard

**Cursor suggested:**
Track play time and show TIME column. Use ctime.

**Decision: REJECTED** — time tracking was not required and would add unnecessary state, file data, and formatting. We kept the ranking based on score, 
with the most recently played result as the tie-breaker.

---

## Summary of rejections

| What Cursor suggested | Why we said no |
|---------------------|----------------|
| display on Question | breaks UI separation |
| subtract points on wrong | we use lives only |
| exceptions for missing file | bool is enough |
| atoi in file parsing | not taught, use >> |
| struct for data classes | use class with getters |
| LeaderboardRow | redundant with map |
| play timer and ctime | removed for simplicity |
| iomanip, limits, sstream | keep libraries minimal |

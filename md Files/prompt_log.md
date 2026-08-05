# Prompt Log — Quiz Arena

This log records the meaningful prompts and follow-up discussions that affected the design and implementation of **Quiz Arena**. Related follow-up prompts are grouped under the same entry so the development process remains chronological without listing every small formatting request.

We used Cursor in **Plan mode** for design and in **Agent mode** for implementation. Each entry includes what we asked, a summary of the agent's response, and our final decision

Prompt numbers follow the original development sequence. Some numbers are omitted because they referred to minor implementation or formatting requests that did not significantly affect the project's design.

For more information, visit our GitHub repository: https://github.com/xGota7/Oop-Final-Project

---

## Prompt 1 — Plan mode, first UML

**We asked:**
> We want a text quiz game in C++. Before code, sketch a minimal class design. Classes: Question abstract base, MultipleChoiceQuestion, Player, SaveSlot, LeaderboardEntry, QuizGame, ConsoleUI. Separate UI from game logic. Use STL, no globals.

**Cursor said:**
Suggested the first class hierarchy and recommended that `QuizGame` store the questions through a `vector<Question*>`. It also suggested beginning with small data classes and adding manager classes when their responsibilities became clearer.

**Decision: ACCEPTED** — this became our initial class design. It gave us the required abstract base class, inheritance, polymorphism, encapsulation, and separation between the game controller and the user interface.

---

## Prompt 2 — Designing the abstract `Question` class

**We asked:**
> Now we do Question class. Please make a plan to implement Question as an abstract base with text, points, and pure virtual getOptionCount, getOption, checkAnswer, and clone.

**Cursor asked us to choose:**
- whether `clone()` should return `Question*` immediately or be added later
- whether `m_text` and `m_points` should be `protected` or private
- whether the destructor should be virtual

**Cursor said:**
Recommended four pure virtual methods, a virtual destructor, protected common fields, and `Question* clone() const` so `QuizGame` could later deep-copy different derived question types.

**Decision: ACCEPTED** — the interface matched the project requirement for an abstract class and prepared the hierarchy for polymorphic copying. We used a raw `Question*` return because smart pointers were not allowed by the course rules.

---

## Prompt 3 — `display()` inside `Question`

**Cursor suggested:**
Adding a `display()` method to `Question` that prints the question text and options using `cout`.

**We asked:**
> Why put cout inside Question? We want UI separate.

**Cursor said:**
Agreed that `Question` should not handle input or output. It changed the plan so `ConsoleUI` reads `getText()`, `getOptionCount()`, and `getOption()` and performs all printing.

**Decision: REJECTED** — placing `cout` inside `Question` would mix the model with the user interface. We kept all display operations inside `ConsoleUI`.

---

## Prompt 4 — `MultipleChoiceQuestion` storage and indexing

**We asked:**
> Add MultipleChoiceQuestion with four options and correct index. Store it in vector of Question* in QuizGame.

**Cursor asked us to choose:**
- fixed `string m_options[4]` or `vector<string>`
- a 0-based or 1-based correct-answer index
- returning an empty string or throwing an exception for an invalid option index

**Cursor said:**
Recommended a fixed array because every multiple-choice question has exactly four options. It also recommended storing the correct index as 0-based internally while converting the 1-based file value during loading.

**Decision: ACCEPTED** — the fixed array was simpler and avoided unnecessary dynamic storage. The file remains easy to read, while the internal indexing matches C++ arrays.

---

## Prompt 5 — Separating `ConsoleUI` from game logic

**We asked:**
> I want to add a ConsoleUI class for our quiz game. All user input and output must live here. ConsoleUI should not contain game rules or scoring logic.

**Cursor asked us to choose:**
- one private `readInt(min, max)` helper or repeated input code in every menu
- `showQuestion(const Question&, number, total)` or passing the question data separately

**Cursor said:**
Recommended one reusable numeric-input helper and passing a `const Question&` to `showQuestion`, allowing the UI to call the virtual question interface.

**Decision: ACCEPTED** — this removed repeated input-validation code and preserved polymorphism without placing any printing inside the question classes.

---

## Prompt 6 — Adding `TrueFalseQuestion`

**We asked:**
> Add a second derived class called TrueFalseQuestion. It should work through the same Question interface and generate the True and False options instead of storing them.

**Cursor said:**
Suggested storing the correct answer as a boolean, returning two options through `getOption()`, and implementing `checkAnswer()` and `clone()` like the other concrete question type. It also updated the question loader to create a `TrueFalseQuestion` when it reads a `TF` block.

**Decision: ACCEPTED** — this gave the project two concrete question types inside the same `vector<Question*>` and demonstrated runtime polymorphism. Generating the fixed True and False labels also avoided storing redundant option strings.

---

## Prompt 7 — Wrong-answer scoring

**Cursor suggested:**
Subtracting points when the answer is wrong.

**We asked:**
> Can we only lose a life on a wrong answer and keep the score going up?

**Cursor said:**
Presented both alternatives. It also pointed out that subtracting points without losing lives meant the loop would normally end only when the questions ran out. It then changed the game logic so a wrong answer calls `loseLife()` and does not change the score.

**Decision: MODIFIED** — a correct answer adds points, while a wrong answer costs one life only. This made the score easier to understand and allowed `isAlive()` to serve as a real lose condition.

---

## Prompt 9 — Rule of Three and polymorphic copying

**We asked:**
> QuizGame owns dynamically allocated questions through vector<Question*>. Is a destructor enough, or do we also need a copy constructor and assignment operator? Explain how deep copying should work when the vector contains different derived question types.

**Cursor said:**
A destructor alone was not enough if a `QuizGame` object could be copied. It recommended a virtual `clone()` in every question type, a copy constructor, an assignment operator with a self-assignment check, and a helper that deletes the old questions before copying.

**Decision: ACCEPTED** — `QuizGame` implements the full Rule of Three. Each derived question clones its real type, preventing shared pointers, double deletion, and shallow copies.

---

## Prompt 12 — `SaveManager`, parsing, and missing files

**We asked:**
> Add SaveManager. It should list saves, load a selected save, delete a selected save, and write the save data to saves.txt.

**Cursor initially suggested:**
Using `atoi()` to convert lines from the file into integers and considered throwing an exception when `saves.txt` did not exist.

**Our follow-up:**
> We did not learn atoi() in the course. Replace it with direct file-stream reading using >>.

During review of the plan, we also chose the proposed `bool`-return option for a missing `saves.txt` instead of the exception alternative.

**Cursor said:**
Replaced `atoi()` with checked stream extraction and used a `bool` return for missing-file handling. It kept `vector<SaveSlot>` because the save menu accesses slots by position.

**Decision: MODIFIED** — we kept the manager design but changed the parsing and error handling to match the course. A missing file is an expected condition, so the caller displays “No saved games” instead of using `try`/`catch`.

---

## Prompt 14 — Data classes: `class` or `struct`

**We asked:**
> Great job. Now make a plan for LeaderboardEntry: player name and best score, one row of data.

**Cursor presented:**
- a `class` with private `m_` fields and getters/setters
- a shorter `struct` with public fields

It made the same type of suggestion while planning `SaveSlot`.

**Cursor said:**
The `struct` required less code, but the `class` version matched `Player`, the course naming style, and the encapsulation requirement.

**Decision: ACCEPTED WITH OUR PREFERENCE** — we used classes with private members for both `LeaderboardEntry` and `SaveSlot`. We rejected public data fields so all project data types followed the same encapsulated style.

---

## Prompt 15 — Choosing the leaderboard container

**Cursor initially suggested:**
Using a `vector<LeaderboardEntry>` to store the leaderboard entries.

**We asked:**
> Since leaderboard entries are searched and updated by player name, would an `unordered_map<string, LeaderboardEntry>` be more suitable than a vector?

**Cursor said:**
A vector would require scanning the entries one by one to find a player. An `unordered_map` allows direct lookup and update using the player name as the key. It also fits the main leaderboard operation better, while vectors remain suitable for questions and saved games because those are accessed by position.

**Decision: MODIFIED** — we changed Cursor's original `vector` design to `unordered_map<string, LeaderboardEntry>`. We accepted the revised implementation because the container was chosen according to the required access pattern.

---

## Prompt 16 — Redundant `LeaderboardRow`

**Cursor suggested:**
Adding a separate `LeaderboardRow` class for display-ready leaderboard rows.

**We asked:**
> Do we need a third class? The map already has the name and the entry.

**Cursor said:**
The display could instead receive sorted player names and read each existing `LeaderboardEntry`.

**Decision: REJECTED** — `LeaderboardRow` duplicated information already stored in the map. `ConsoleUI` displays the existing entries without another data class.

---

## Prompt 18 — Difficulty levels and question files

**We asked:**
> Now let's make a plan to add Easy, Normal, and Hard. Different lives and target score for each mode.

**Cursor first suggested:**
Three modes using constant arrays for lives and targets, while all modes continued using one shared `questions.txt`.

**During the same planning sequence, we compared:**
- one shared question file
- separate question files for Easy, Normal, and Hard

**Cursor said:**
A shared file was simpler, but separate files allowed each mode to contain a genuinely different question bank. It proposed `questionFileForDifficulty()` to select the correct path.

**Decision: MODIFIED** — we accepted the centralized difficulty arrays but changed the file design to:
- `questions_easy.txt`
- `questions_normal.txt`
- `questions_hard.txt`

This made difficulty affect both the settings and the question content.

---

## Prompt 20 — Timer on the leaderboard

**Cursor suggested:**
Tracking play time, saving elapsed time, adding a TIME column, and using `ctime`.

**We reviewed it and asked for removal:**
The timer was not required and affected `QuizGame`, save files, leaderboard entries, sorting, and UI formatting.

**Cursor said:**
Removed the timer fields, time-formatting functions, `ctime`, and the TIME column. It used `m_lastOrder` as the leaderboard tie-breaker instead.

**Decision: REJECTED / REMOVED** — time tracking added too much complexity for a feature that was not required. The final ranking uses scores and the most recent play order.

---

## Summary of Important Changes

| Agent suggestion or question | Final decision | Reason |
|---|---|---|
| `display()` inside `Question` | Rejected | It would break the separation between the model and the user interface |
| Subtract points on a wrong answer | Modified | A wrong answer now removes one life without changing the score |
| Full Rule of Three for `QuizGame` | Accepted | Required for safe deep copying of dynamically allocated questions |
| `atoi()` for file parsing | Rejected | It was not taught in the course, so we used stream extraction with `>>` |
| Exception for a missing save file | Rejected | A missing file is an expected case and can be handled with a `bool` return |
| Public `struct` data holders | Rejected | We used classes with private members to preserve encapsulation |
| `vector` for the leaderboard | Modified | Changed to `unordered_map` because entries are searched and updated by player name |
| Extra `LeaderboardRow` class | Rejected | It would duplicate information already stored in the leaderboard map |
| One question file for all modes | Modified | Separate question files allow each difficulty to use different questions |
| Timer and TIME column | Rejected | The feature was not required and added unnecessary complexity |

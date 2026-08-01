# Reflection Report — Quiz Arena

**Course:** 31695 Object-Oriented Programming — Final Project  
**Tool:** Cursor (AI coding assistant in the Cursor IDE)

---

## What we built

Quiz Arena is a text quiz game. The player answers math questions from a file. Correct answers add points. Wrong answers cost lives. The game has a menu, save and load, three difficulty levels, and a leaderboard that persists between runs.

The design uses an abstract Question class, two derived types, a QuizGame controller, Player, SaveManager with SaveSlot, Leaderboard with LeaderboardEntry, and ConsoleUI for all I/O.

---

## What Cursor did well

Cursor was fast at generating the repetitive, standard parts of the code. Once we agreed on a class, it produced headers, getters, and file read/write quickly. It followed our naming style after we set it: m_ members, include guards, virtual destructor on the base class.

When we asked in Plan mode why something should be a certain way, it gave useful comparisons. That helped us decide between vector and unordered_map for the leaderboard.

---

## Where we had to correct Cursor

- It wanted cout inside Question. We kept printing in ConsoleUI only.
- It wanted to subtract points on wrong answers. We use lives instead.
- It suggested exceptions when a save file is missing. We use bool because an empty save list is normal.
- It only added a destructor first for QuizGame. We asked for full Rule of Three and virtual clone on questions.
- It suggested atoi for files. We use >> because we did not learn atoi.
- It used struct with public fields. We changed to class with private members.
- It added a timer and TIME column. We removed all time tracking to keep the project simpler.
- It added LeaderboardRow for display. We dropped it and read map entries directly in ConsoleUI.
- It wanted iomanip and limits. We used tabs and a simple input clear loop.

Most grading points came from us checking each suggestion against course rules and the assignment brief.

---

## How Cursor changed our workflow

We learned to start in Plan mode and sketch classes before Act mode. When we asked for code too early, we spent more time undoing choices.

The container question was the best example. Cursor defaults to vector for everything. We kept vector for questions and saves because we access by position. We used unordered_map for the leaderboard because we access by player name. Explaining that difference in the presentation was important.

---

## What we would do differently

- Send the full list of course rules at the start: no auto, no smart pointers, bool for errors, manual memory.
- Agree on the full class diagram before the first implementation prompt.
- Test save and load round-trip earlier instead of only playing manually.

---

## Honest summary

Cursor saved time on repetitive code. It did not replace understanding. We can explain every class, every container choice, and every rejected suggestion. That was the goal of the assignment.

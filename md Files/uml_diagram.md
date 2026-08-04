# UML Class Diagram — Quiz Arena

This file documents the current class structure of **Quiz Arena** according to the latest
`.h` and `.cpp` files.

---

## Mermaid Class Diagram

```mermaid
classDiagram
    class Question {
        <<abstract>>
        #m_text : string
        #m_points : int
        +Question(text : const string&, points : int)
        +~Question() virtual
        +getOptionCount() const : int
        +getOption(index : int) const : string
        +checkAnswer(index : int) const : bool
        +clone() const : Question*
        +getText() const : const string&
        +getPoints() const : int
    }

    note for Question "Pure virtual: getOptionCount, getOption, checkAnswer, clone"

    class MultipleChoiceQuestion {
        -m_options : string[4]
        -m_correctIndex : int
        +MultipleChoiceQuestion(text : const string&, points : int, options : const string[], correctIndex : int)
        +~MultipleChoiceQuestion()
        +getOptionCount() const : int
        +getOption(index : int) const : string
        +checkAnswer(index : int) const : bool
        +clone() const : Question*
    }

    class TrueFalseQuestion {
        -m_correctIsTrue : bool
        +TrueFalseQuestion(text : const string&, points : int, correctIsTrue : bool)
        +~TrueFalseQuestion()
        +getOptionCount() const : int
        +getOption(index : int) const : string
        +checkAnswer(index : int) const : bool
        +clone() const : Question*
    }

    class Player {
        -m_name : string
        -m_score : int
        -m_lives : int
        +Player()
        +Player(name : const string&, lives : int)
        +setName(name : const string&) void
        +setScore(score : int) void
        +setLives(lives : int) void
        +addScore(points : int) void
        +loseLife() void
        +getName() const : const string&
        +getScore() const : int
        +getLives() const : int
        +isAlive() const : bool
    }

    class LeaderboardEntry {
        -m_scores : int[3]
        -m_lastOrder : int
        +LeaderboardEntry()
        +getScore(mode : int) const : int
        +setScore(mode : int, score : int) void
        +getLastOrder() const : int
        +setLastOrder(order : int) void
        +getTotalScore() const : int
    }

    class Leaderboard {
        -m_entries : unordered_map~string, LeaderboardEntry~
        -m_nextOrder : int
        +Leaderboard()
        +submitResult(name : const string&, score : int, mode : int) void
        +saveToFile(path : const string&) const : bool
        +loadFromFile(path : const string&) bool
        +getCount() const : int
        +getSortedNames(names : vector~string~&) const : void
        +getEntry(name : const string&, out : LeaderboardEntry&) const : bool
    }

    class SaveSlot {
        -m_name : string
        -m_hasMode : bool[3]
        -m_score : int[3]
        -m_lives : int[3]
        -m_currentIndex : int[3]
        +SaveSlot()
        +setName(name : const string&) void
        +getName() const : const string&
        +hasMode(mode : int) const : bool
        +setHasMode(mode : int, has : bool) void
        +getScore(mode : int) const : int
        +setScore(mode : int, score : int) void
        +getLives(mode : int) const : int
        +setLives(mode : int, lives : int) void
        +getCurrentIndex(mode : int) const : int
        +setCurrentIndex(mode : int, currentIndex : int) void
    }

    class SaveManager {
        -m_slots : vector~SaveSlot~
        -m_path : string
        +SaveManager(path : const string&)
        +load() bool
        +store() const : bool
        +getCount() const : int
        +getSlot(index : int) const : const SaveSlot&
        +hasName(name : const string&) const : bool
        +findIndexByName(name : const string&) const : int
        +upsertSlot(slot : const SaveSlot&) void
        +removeSlot(index : int) bool
    }

    class QuizGame {
        -LIVES : const int[3]$
        -TARGETS : const int[3]$
        -m_questions : vector~Question*~
        -m_player : Player
        -m_leaderboard : Leaderboard
        -m_difficulty : int
        -m_targetScore : int
        -m_startLives : int
        -m_currentIndex : int
        -addQuestion(question : Question*) void
        -clearQuestions() void
        -copyFrom(other : const QuizGame&) void
        -applyDifficulty(difficulty : int) void
        -questionFileForDifficulty(difficulty : int) const : string
        -countQuestionsInFile(path : const string&) const : int
        -startNewGame(ui : ConsoleUI&) void
        -openSavesMenu(ui : ConsoleUI&) void
        -playSession(ui : ConsoleUI&) void
        -saveCurrentGame(ui : ConsoleUI&) void
        -updateSaveSlot() bool
        -isWin() const : bool
        +QuizGame()
        +QuizGame(other : const QuizGame&)
        +operator=(other : const QuizGame&) : QuizGame&
        +~QuizGame()
        +loadQuestions(path : const string&) bool
        +loadQuestionsForDifficulty(difficulty : int) bool
        +run(ui : ConsoleUI&) void
        +getQuestionCount() const : int
        +getQuestionCountForDifficulty(difficulty : int) const : int
    }

    class ConsoleUI {
        -readInt(minValue : int, maxValue : int) const : int
        -difficultyName(difficulty : int) const : string
        +ConsoleUI()
        +showMainMenu() const : int
        +askDifficulty(lives : const int[], targets : const int[]) const : int
        +showQuestion(question : const Question&, number : int, total : int) const : void
        +askAnswerOrCommand(optionCount : int) const : int
        +showAnswerResult(correct : bool, correctText : const string&, pointsGained : int, livesRemaining : int) const : void
        +showStatus(player : const Player&) const : void
        +showGameOver(won : bool, player : const Player&, targetScore : int) const : void
        +showLeaderboard(board : const Leaderboard&) const : void
        +showSaveSlots(saves : const SaveManager&, totalQuestionsPerMode : const int[3]) const : void
        +askSavesAction() const : int
        +askSaveNumber(count : int) const : int
        +askContinueOrNew() const : int
        +askSaveNameNotTaken(saves : const SaveManager&) const : string
        +showMessage(message : const string&) const : void
        +showError(message : const string&) const : void
    }

    Question <|-- MultipleChoiceQuestion : inherits
    Question <|-- TrueFalseQuestion : inherits
    
    QuizGame "1" *-- "0..*" Question : owns questions in vector
    QuizGame "1" *-- "1" Player : contains
    QuizGame "1" *-- "1" Leaderboard : contains
    
    Leaderboard "1" *-- "0..*" LeaderboardEntry : stores entries in map
    SaveManager "1" *-- "0..*" SaveSlot : stores slots in vector
    
    QuizGame --> SaveManager : uses
    QuizGame --> ConsoleUI : uses
    
    ConsoleUI --> Question : reads
    ConsoleUI --> Player : reads
    ConsoleUI --> Leaderboard : reads
    ConsoleUI --> SaveManager : reads
```

---

## ASCII Fallback

```text
                         +------------------------+
                         |       Question         |  <<abstract>>
                         |------------------------|
                         | # m_text : string      |
                         | # m_points : int       |
                         |------------------------|
                         | + getOptionCount() = 0 |
                         | + getOption(i) = 0     |
                         | + checkAnswer(i) = 0   |
                         | + clone() = 0          |
                         +------------------------+
                                   /_\
                                    |  inheritance
                 +------------------+------------------+
                 |                                     |
   +---------------------------+        +---------------------------+
   | MultipleChoiceQuestion    |        | TrueFalseQuestion         |
   |---------------------------|        |---------------------------|
   | - m_options[4] : string   |        | - m_correctIsTrue : bool  |
   | - m_correctIndex : int    |        |                           |
   +---------------------------+        +---------------------------+

   +---------------------------------------------------------------+
   |                           QuizGame                            |
   |---------------------------------------------------------------|
   | - LIVES[3] / TARGETS[3] : static const int                   |
   | - m_questions : vector<Question*>                            |
   | - m_player : Player                                          |
   | - m_leaderboard : Leaderboard                                |
   | - m_difficulty / m_targetScore / m_startLives : int          |
   | - m_currentIndex : int                                       |
   |---------------------------------------------------------------|
   | + loadQuestions(path)                                        |
   | + loadQuestionsForDifficulty(difficulty)                     |
   | + run(ui)                                                    |
   | + getQuestionCount()                                         |
   | + getQuestionCountForDifficulty(difficulty)                  |
   | + copy constructor / assignment / destructor                 |
   +---------------------------------------------------------------+
      | owns 0..*     | contains 1    | contains 1    \ uses
      v               v               v                v
 [ Question* ]    [ Player ]     [ Leaderboard ]  [ SaveManager ]
                                  map<string,Entry> vector<SaveSlot>

                                      [ ConsoleUI ]
                                     all console I/O
```

---

## Relationship Descriptions

| Relationship | Type | Meaning in the current code |
|---|---|---|
| `MultipleChoiceQuestion` → `Question` | Inheritance | A multiple-choice question is a concrete `Question`. |
| `TrueFalseQuestion` → `Question` | Inheritance | A true/false question is a concrete `Question`. |
| `QuizGame` → `Question` | Composition / ownership | Stores `vector<Question*>`, creates/clones the objects and deletes them in `clearQuestions()` and the destructor. |
| `QuizGame` → `Player` | Composition | `m_player` is a value member whose lifetime is tied to the game. |
| `QuizGame` → `Leaderboard` | Composition | `m_leaderboard` is a value member whose lifetime is tied to the game. |
| `Leaderboard` → `LeaderboardEntry` | Composition | Entries are stored directly as values inside `unordered_map<string, LeaderboardEntry>`. |
| `SaveManager` → `SaveSlot` | Composition | Slots are stored directly as values inside `vector<SaveSlot>`. |
| `QuizGame` → `MultipleChoiceQuestion` / `TrueFalseQuestion` | Dependency | `loadQuestions()` creates the concrete question objects. |
| `QuizGame` → `SaveManager` / `SaveSlot` | Dependency | Creates save managers locally and reads or updates save slots. |
| `QuizGame` → `ConsoleUI` | Dependency | Receives `ConsoleUI&` and uses it for all user interaction. |
| `ConsoleUI` → model classes | Dependency | Reads `Question`, `Player`, `Leaderboard`, `LeaderboardEntry`, `SaveManager` and `SaveSlot` data for display; it owns none of them. |

---

## Current Constants Reflected by the Code

| Difficulty | Lives | Target score | Question file |
|---|---:|---:|---|
| Easy | 5 | 85 | `questions_easy.txt` |
| Normal | 4 | 90 | `questions_normal.txt` |
| Hard | 3 | 95 | `questions_hard.txt` |

Both question types currently award **5 points** per correct answer.

---

## Where Each Required OOP Concept Appears

| Requirement | Where in the current code |
|---|---|
| Abstract base class + pure virtual methods | `Question`; `getOptionCount`, `getOption`, `checkAnswer` and `clone` are pure virtual. |
| Inheritance | `MultipleChoiceQuestion` and `TrueFalseQuestion` inherit publicly from `Question`. |
| Polymorphism | `QuizGame::m_questions` is `vector<Question*>`; the game calls virtual methods through base pointers. |
| Dynamic memory ownership | `QuizGame` creates, clones and deletes the `Question` objects. |
| Rule of Three | `QuizGame` has a destructor, copy constructor and assignment operator; copying uses `clone()`. |
| Encapsulation | Data members are private or protected and accessed through methods. |
| Separation of concerns | Game logic in `QuizGame`, console I/O in `ConsoleUI`, saves in `SaveManager`, scores in `Leaderboard`, player state in `Player`. |
| STL containers | `vector<Question*>`, `vector<SaveSlot>` and `unordered_map<string, LeaderboardEntry>`. |
| Null pointer safety | Checks before adding, cloning, deleting and using question pointers. |
| No global game state | Player, score, lives, saves, leaderboard data and question progress are stored inside objects. |

---

## Notes for the Presentation Slide

- Place `Question` above `MultipleChoiceQuestion` and `TrueFalseQuestion` with inheritance arrows pointing to `Question`.
- Place `QuizGame` in the centre as the controller.
- Use filled diamonds from `QuizGame` to `Question`, `Player` and `Leaderboard`.
- Use a filled diamond from `Leaderboard` to `LeaderboardEntry` and from `SaveManager` to `SaveSlot`.
- Use dashed dependency arrows from `QuizGame` to `ConsoleUI` and `SaveManager`.
- Mark the four polymorphic methods in `Question` as pure virtual.
- Do not add a timer; the current design has no timer fields.

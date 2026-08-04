## About the Game

Quiz Arena is a terminal quiz game written in C++.

The player answers two types of questions: multiple choice questions with four possible answers, and true or false questions.

The game includes three difficulty levels. Each difficulty uses a different question file, so the questions become harder as the difficulty increases. 
The difficulty also changes the number of lives and the target score.

| Difficulty | Lives | Target Score |
|------------|-------|--------------|
| Easy       |   5   |      85      |
| Normal	 |   4   |      90      |
| Hard       |   3   |      95      |	


A correct answer adds 5 points. A wrong answer removes one life and shows the correct answer.

To win, the player must complete all questions, stay alive, and reach the target score of the selected difficulty. 
The player loses if all lives are lost or if all questions are completed without reaching the target score.

Only winning results are added to the leaderboard. A losing result or a result below the target score is not recorded.

The leaderboard keeps only the best score achieved by each player for each difficulty. 
If a player completes the same difficulty again with a lower score, the previous higher score remains. 
The leaderboard also displays the total of the player's best scores from all difficulty levels.

Each player can save separate progress for Easy, Normal, and Hard and continue the game later.

## How to Build and Run

Open the `QuizArena` folder and make sure MSYS2 and a g++ compiler with C++17 support are installed.

Run the `run.bat` file. It compiles all the project files and starts the game automatically.

If MSYS2 and g++ are not installed, add all `.cpp` and `.h` files from the `QuizArena` folder to a Visual Studio C++ project, compile the project, and run it from there.

The game should be run from the `QuizArena` folder because it contains the question files used by the program.

## How to Play

1. Choose **New Game** from the main menu.
2. Enter a unique player name.
3. Choose Easy, Normal, or Hard difficulty.
4. Enter the number of the answer you want to select.
5. Enter `0` during a question to save the current progress and return to the main menu.

## Important Notes

The **Saved Games** menu allows the player to continue saved progress, start a new run for a selected difficulty, or delete a saved game.
The **Leaderboard** displays each player's best score for Easy, Normal, and Hard, together with the total score.

Do not manually edit `saves.txt` or `leaderboard.txt` after they are created. These files are created and updated automatically by the game.
Each of the 20 questions in `questions_easy.txt`, `questions_normal.txt`, and `questions_hard.txt` can be edited.
All questions and answers must be written in English and must follow the existing format.
A multiple choice question starts with `MC`, followed by the question, four answer options, and the correct answer number from `1` to `4`.
  Example:
      MC
      What is 8 + 6?
      14
      13
      15
      16
      1

A true or false question starts with `TF`, followed by the statement and the correct answer number. Use `1` for True and `2` for False.
  Example:
      TF
      6 + 7 equals 14.
      2
#include "QuizGame.h"
#include "MultipleChoiceQuestion.h"
#include "TrueFalseQuestion.h"
#include "SaveManager.h"
#include <fstream>

static const int MC_POINTS = 5;
static const int TF_POINTS = 5;

// Lives for Easy, Normal, Hard
const int QuizGame::LIVES[3] = {5, 4, 3};

// Target score to win for Easy, Normal, Hard
const int QuizGame::TARGETS[3] = {85, 90, 95};

static const char* SAVES_PATH = "saves.txt";
static const char* LEADERBOARD_PATH = "leaderboard.txt";

// Create a game with the normal difficulty as the default.
QuizGame::QuizGame() {
    m_questionIndex = 0;
    applyDifficulty(DIFFICULTY_NORMAL);
}

// Copy another game, including a deep copy of every question.
QuizGame::QuizGame(const QuizGame& other) {
    copyFrom(other);
}

// Replace this game with a copy of another game.
QuizGame& QuizGame::operator=(const QuizGame& other) {
    if (this != &other) {
        clearQuestions();
        copyFrom(other);
    }
    return *this;
}

// Free every owned Question before the game object is destroyed.
QuizGame::~QuizGame() {
    clearQuestions();
}

// Copy every field from another game. Questions are cloned so the two
// games never share the same Question objects.
void QuizGame::copyFrom(const QuizGame& other) {
    for (int i = 0; i < (int)other.m_questions.size(); i++) {
        if (other.m_questions[i] != nullptr) {
            Question* copy = other.m_questions[i]->clone();
            if (copy != nullptr) {
                m_questions.push_back(copy);
            }
        }
    }

    m_player = other.m_player;
    m_leaderboard = other.m_leaderboard;
    m_difficulty = other.m_difficulty;
    m_targetScore = other.m_targetScore;
    m_startLives = other.m_startLives;
    m_questionIndex = other.m_questionIndex;
}

// Take ownership of one question pointer.
void QuizGame::addQuestion(Question* question) {
    if (question != nullptr) {
        m_questions.push_back(question);
    }
}

// Delete every owned question and empty the container.
void QuizGame::clearQuestions() {
    for (int i = 0; i < (int)m_questions.size(); i++) {
        if (m_questions[i] != nullptr) {
            delete m_questions[i];
            m_questions[i] = nullptr;
        }
    }
    m_questions.clear();
}

// Set lives and target score for the chosen difficulty.
void QuizGame::applyDifficulty(int difficulty) {

    if (difficulty < 0 || difficulty > 2) {
        difficulty = DIFFICULTY_NORMAL;
    }

    m_difficulty = difficulty;
    m_startLives = LIVES[difficulty];
    m_targetScore = TARGETS[difficulty];
}

// Return the question file that belongs to a difficulty.
string QuizGame::questionFileForDifficulty(int difficulty) const {
    if (difficulty == DIFFICULTY_EASY) {
        return "questions_easy.txt";
    }
    if (difficulty == DIFFICULTY_HARD) {
        return "questions_hard.txt";
    }
    return "questions_normal.txt";
}

// Count MC and TF markers in a question file without fully parsing it.
int QuizGame::countQuestionsInFile(const string& path) const {
    ifstream in(path.c_str());
    if (!in.is_open()) {
        return 0;
    }

    int count = 0;
    string line;
    while (getline(in, line)) {
        if (line == "MC" || line == "TF") {
            count++;
        }
    }

    in.close();
    return count;
}

// Load the question file that matches the chosen difficulty.
bool QuizGame::loadQuestionsForDifficulty(int difficulty) {
    return loadQuestions(questionFileForDifficulty(difficulty));
}

// Return how many questions a difficulty file contains.
int QuizGame::getQuestionCountForDifficulty(int difficulty) const {
    return countQuestionsInFile(questionFileForDifficulty(difficulty));
}

// Read questions from a text file into the polymorphic question list.
bool QuizGame::loadQuestions(const string& path) {
    ifstream in(path.c_str());
    if (!in.is_open()) {
        return false;
    }

    clearQuestions();

    string line;
    while (getline(in, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }

        if (line == "MC") {
            string text;
            string options[MC_OPTION_COUNT];
            int correct1Based = 0;

            if (!getline(in, text)) {
                break;
            }
            for (int i = 0; i < MC_OPTION_COUNT; i++) {
                if (!getline(in, options[i])) {
                    break;
                }
            }
            if (!(in >> correct1Based)) {
                break;
            }

            // Files store a 1 based answer. The class stores a 0 based index.
            int correctIndex = correct1Based - 1;
            if (correctIndex >= 0 && correctIndex < MC_OPTION_COUNT) {
                addQuestion(new MultipleChoiceQuestion(text, MC_POINTS, options, correctIndex));
            }
        } else if (line == "TF") {
            string text;
            int correct1Based = 0;

            if (!getline(in, text)) {
                break;
            }
            if (!(in >> correct1Based)) {
                break;
            }

            int correctIndex = correct1Based - 1;
            if (correctIndex == 0 || correctIndex == 1) {
                bool correctIsTrue = (correctIndex == 0);
                addQuestion(new TrueFalseQuestion(text, TF_POINTS, correctIsTrue));
            }
        }
    }
    in.close();

    return !m_questions.empty();
}

// Return true when the current score reached the target for this difficulty.
bool QuizGame::isWin() const {
    return m_player.getScore() >= m_targetScore;
}

// Run the main menu until the player quits.
void QuizGame::run(ConsoleUI& ui) {
    m_leaderboard.loadFromFile(LEADERBOARD_PATH);

    bool running = true;
    while (running) {
        int choice = ui.showMainMenu();
        if (choice == 1) {
            startNewGame(ui);
        } else if (choice == 2) {
            openSavesMenu(ui);
        } else if (choice == 3) {
            ui.showLeaderboard(m_leaderboard);
        } else {
            running = false;
        }
    }

    ui.showMessage("Thanks for playing Quiz Arena!");
}

// Start a new named game at the chosen difficulty.
void QuizGame::startNewGame(ConsoleUI& ui) {
    SaveManager saves(SAVES_PATH);
    saves.load();

    string name = ui.askSaveNameNotTaken(saves);
    int difficulty = ui.askDifficulty(LIVES, TARGETS);

    applyDifficulty(difficulty);
    if (!loadQuestionsForDifficulty(difficulty)) {
        ui.showError("Could not load questions for this difficulty.");
        return;
    }

    m_player = Player(name, m_startLives);
    m_questionIndex = 0;

    playSession(ui);
}

// Load, continue, restart, or delete a saved game.
void QuizGame::openSavesMenu(ConsoleUI& ui) {
    SaveManager saves(SAVES_PATH);
    if (!saves.load()) {
        ui.showError("Save data is corrupted.");
        return;
    }

    if (saves.getCount() == 0) {
        ui.showMessage("No saved games yet.");
        return;
    }

    bool done = false;
    while (!done) {
        int numQperDifficulty[numOfDifficulties];
        for (int difficulty = 0; difficulty < numOfDifficulties; difficulty++) {
            numQperDifficulty[difficulty] = getQuestionCountForDifficulty(difficulty);
        }
        ui.showSaveSlots(saves, numQperDifficulty);

        int menuChoice = ui.askSavesAction();
        if (menuChoice == 3) {
            done = true;
            continue;
        }

        int number = ui.askSaveNumber(saves.getCount());
        if (number == 0) {
            continue;
        }
        int index = number - 1;

        if (menuChoice == 1) {
            const SaveSlot& slot = saves.getSlot(index);
            int difficulty = ui.askDifficulty(LIVES, TARGETS);

            applyDifficulty(difficulty);
            if (!loadQuestionsForDifficulty(difficulty)) {
                ui.showError("Could not load questions for this difficulty.");
                return;
            }

            m_player.setName(slot.getName());

            if (!slot.doneDifficulty(difficulty)) {
                ui.showMessage("No saved progress for this difficulty yet. Starting a new run.");
                m_player = Player(slot.getName(), m_startLives);
                m_questionIndex = 0;
            } else {
                int loadAction = ui.askContinueOrNew();
                if (loadAction == 1) {
                    int savedQuestionIndex = slot.getCurrentIndex(difficulty);
                    if (savedQuestionIndex < 0) {
                        ui.showError("Save data is corrupted.");
                        continue;
                    }
                    m_player.setScore(slot.getScore(difficulty));
                    m_player.setLives(slot.getLives(difficulty));
                    m_questionIndex = savedQuestionIndex;
                } else {
                    m_player = Player(slot.getName(), m_startLives);
                    m_questionIndex = 0;
                }
            }

            playSession(ui);
            return;
        }

        if (menuChoice == 2) {
            if (saves.removeSlot(index)) {
                saves.store();
                ui.showMessage("Save deleted.");
            }
            if (saves.getCount() == 0) {
                ui.showMessage("No saved games left.");
                done = true;
            }
        }
    }
}

// Play from the current question until the run ends or the player saves.
void QuizGame::playSession(ConsoleUI& ui) {
    if (m_questions.empty()) {
        ui.showError("No questions are loaded.");
        return;
    }

    if (m_questionIndex < 0) {
        ui.showError("Save data is corrupted.");
        return;
    }

    bool quitToMenu = false;
    while (m_questionIndex < (int)m_questions.size()
           && m_player.isAlive() && !quitToMenu) {
        Question* currentQuestion = m_questions[m_questionIndex];
        if (currentQuestion == nullptr) {
            m_questionIndex++;
            continue;
        }

        ui.showStatus(m_player);
        ui.showQuestion(*currentQuestion, m_questionIndex + 1, (int)m_questions.size());

        int answerChoice = ui.askAnswerOrCommand(currentQuestion->getOptionCount());
        if (answerChoice == -1) {
            saveCurrentGame(ui);
            quitToMenu = true;
            continue;
        }

        int answerIndex = answerChoice - 1;
        if (currentQuestion->checkAnswer(answerIndex)) {
            m_player.addScore(currentQuestion->getPoints());
            ui.showAnswerResult(true, "", currentQuestion->getPoints(), m_player.getLives());
        } else {
            m_player.loseLife();
            string correctText = currentQuestion->getOption(0);
            for (int i = 0; i < currentQuestion->getOptionCount(); i++) {
                if (currentQuestion->checkAnswer(i)) {
                    correctText = currentQuestion->getOption(i);
                    break;
                }
            }
            ui.showAnswerResult(false, correctText, 0, m_player.getLives());
        }

        m_questionIndex++;
    }

    if (quitToMenu) {
        return;
    }

    bool won = m_player.isAlive() && isWin();
    ui.showGameOver(won, m_player, m_targetScore);

    // Only scores that reached the target are stored on the leaderboard.
    if (isWin()) {
        m_leaderboard.submitResult(m_player.getName(), m_player.getScore(), m_difficulty);
        if (!m_leaderboard.saveToFile(LEADERBOARD_PATH)) {
            ui.showError("Could not update the leaderboard file.");
        }
    }

    updateSaveSlot();
}

// Write the current player progress into saves.txt.
bool QuizGame::updateSaveSlot() {
    SaveManager saves(SAVES_PATH);
    saves.load();

    SaveSlot slot;
    int index = saves.findIndexByName(m_player.getName());
    if (index >= 0) {
        slot = saves.getSlot(index);
    } else {
        slot.setName(m_player.getName());
    }

    slot.setDoneDifficulty(m_difficulty, true);
    slot.setScore(m_difficulty, m_player.getScore());
    slot.setLives(m_difficulty, m_player.getLives());
    slot.setCurrentIndex(m_difficulty, m_questionIndex);

    saves.update_insertSlot(slot);
    return saves.store();
}

// Save progress and tell the player whether it succeeded.
void QuizGame::saveCurrentGame(ConsoleUI& ui) {
    if (updateSaveSlot()) {
        ui.showMessage("Game saved. Returning to menu.");
    } else {
        ui.showError("Could not save the game.");
    }
}

// Return how many questions are currently loaded.
int QuizGame::getQuestionCount() const {
    return (int)m_questions.size();
}

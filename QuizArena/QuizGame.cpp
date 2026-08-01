#include "QuizGame.h"
#include "MultipleChoiceQuestion.h"
#include "TrueFalseQuestion.h"
#include "SaveManager.h"
#include <fstream>

static const int MC_POINTS = 10;
static const int TF_POINTS = 10;

static const char* SAVES_PATH = "saves.txt";
static const char* LEADERBOARD_PATH = "leaderboard.txt";

QuizGame::QuizGame() {
    m_currentIndex = 0;
    applyDifficulty(DIFFICULTY_NORMAL);
}

QuizGame::QuizGame(const QuizGame& other) {
    copyFrom(other);
}

QuizGame& QuizGame::operator=(const QuizGame& other) {
    if (this != &other) {
        clearQuestions();
        copyFrom(other);
    }
    return *this;
}

QuizGame::~QuizGame() {
    clearQuestions();
}

// Deep-copies every field from another game, cloning the questions so the two
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
    m_currentIndex = other.m_currentIndex;
}

void QuizGame::addQuestion(Question* question) {
    if (question != nullptr) {
        m_questions.push_back(question);
    }
}

void QuizGame::clearQuestions() {
    for (int i = 0; i < (int)m_questions.size(); i++) {
        if (m_questions[i] != nullptr) {
            delete m_questions[i];
            m_questions[i] = nullptr;
        }
    }
    m_questions.clear();
}

void QuizGame::applyDifficulty(int difficulty) {
    static const int LIVES[3] = {5, 3, 2};
    static const int TARGETS[3] = {80, 100, 120};

    if (difficulty < 0 || difficulty > 2) {
        difficulty = DIFFICULTY_NORMAL;
    }

    m_difficulty = difficulty;
    m_startLives = LIVES[difficulty];
    m_targetScore = TARGETS[difficulty];
}

string QuizGame::questionFileForDifficulty(int difficulty) const {
    if (difficulty == DIFFICULTY_EASY) {
        return "questions_easy.txt";
    }
    if (difficulty == DIFFICULTY_HARD) {
        return "questions_hard.txt";
    }
    return "questions_normal.txt";
}

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

bool QuizGame::loadQuestionsForDifficulty(int difficulty) {
    return loadQuestions(questionFileForDifficulty(difficulty));
}

int QuizGame::getQuestionCountForDifficulty(int difficulty) const {
    return countQuestionsInFile(questionFileForDifficulty(difficulty));
}

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

bool QuizGame::isWin() const {
    return m_player.getScore() >= m_targetScore;
}

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

void QuizGame::startNewGame(ConsoleUI& ui) {
    SaveManager saves(SAVES_PATH);
    saves.load();

    string name = ui.askSaveNameNotTaken(saves);
    int difficulty = ui.askDifficulty();

    applyDifficulty(difficulty);
    if (!loadQuestionsForDifficulty(difficulty)) {
        ui.showError("Could not load questions for this difficulty.");
        return;
    }

    m_player = Player(name, m_startLives);
    m_currentIndex = 0;

    playSession(ui);
}

void QuizGame::openSavesMenu(ConsoleUI& ui) {
    SaveManager saves(SAVES_PATH);
    saves.load();

    if (saves.getCount() == 0) {
        ui.showMessage("No saved games yet.");
        return;
    }

    bool done = false;
    while (!done) {
        int totals[SAVE_MODE_COUNT];
        for (int mode = 0; mode < SAVE_MODE_COUNT; mode++) {
            totals[mode] = getQuestionCountForDifficulty(mode);
        }
        ui.showSaveSlots(saves, totals);

        int action = ui.askSavesAction();
        if (action == 3) {
            done = true;
            continue;
        }

        int number = ui.askSaveNumber(saves.getCount());
        if (number == 0) {
            continue;
        }
        int index = number - 1;

        if (action == 1) {
            const SaveSlot& slot = saves.getSlot(index);
            int mode = ui.askDifficulty();

            applyDifficulty(mode);
            if (!loadQuestionsForDifficulty(mode)) {
                ui.showError("Could not load questions for this difficulty.");
                return;
            }

            m_player.setName(slot.getName());

            if (!slot.hasMode(mode)) {
                ui.showMessage("No saved progress for this mode yet. Starting a new run.");
                m_player = Player(slot.getName(), m_startLives);
                m_currentIndex = 0;
            } else {
                int loadAction = ui.askContinueOrNew();
                if (loadAction == 1) {
                    m_player.setScore(slot.getScore(mode));
                    m_player.setLives(slot.getLives(mode));
                    m_currentIndex = slot.getCurrentIndex(mode);
                } else {
                    m_player = Player(slot.getName(), m_startLives);
                    m_currentIndex = 0;
                }
            }

            playSession(ui);
            return;
        }

        if (action == 2) {
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

// Plays through the questions from the current index. A correct answer adds
// points; a wrong answer costs a life. The session ends when the questions run
// out, the player loses all lives, or the player saves and quits.
void QuizGame::playSession(ConsoleUI& ui) {
    if (m_questions.empty()) {
        ui.showError("No questions are loaded.");
        return;
    }

    bool quitToMenu = false;
    while (m_currentIndex < (int)m_questions.size()
           && m_player.isAlive() && !quitToMenu) {
        Question* current = m_questions[m_currentIndex];
        if (current == nullptr) {
            m_currentIndex++;
            continue;
        }

        ui.showStatus(m_player);
        ui.showQuestion(*current, m_currentIndex + 1, (int)m_questions.size());

        int action = ui.askAnswerOrCommand(current->getOptionCount());
        if (action == -1) {
            saveCurrentGame(ui);
            quitToMenu = true;
            continue;
        }

        int answerIndex = action - 1;
        if (current->checkAnswer(answerIndex)) {
            m_player.addScore(current->getPoints());
            ui.showAnswerResult(true, "", current->getPoints(), m_player.getLives());
        } else {
            m_player.loseLife();
            string correctText = current->getOption(0);
            for (int i = 0; i < current->getOptionCount(); i++) {
                if (current->checkAnswer(i)) {
                    correctText = current->getOption(i);
                    break;
                }
            }
            ui.showAnswerResult(false, correctText, 0, m_player.getLives());
        }

        m_currentIndex++;
    }

    if (quitToMenu) {
        return;
    }

    bool won = m_player.isAlive() && isWin();
    ui.showGameOver(won, m_player, m_targetScore);

    m_leaderboard.submitResult(m_player.getName(), m_player.getScore(), m_difficulty);
    if (!m_leaderboard.saveToFile(LEADERBOARD_PATH)) {
        ui.showError("Could not update the leaderboard file.");
    }

    updateSaveSlot();
}

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

    slot.setHasMode(m_difficulty, true);
    slot.setScore(m_difficulty, m_player.getScore());
    slot.setLives(m_difficulty, m_player.getLives());
    slot.setCurrentIndex(m_difficulty, m_currentIndex);

    saves.upsertSlot(slot);
    return saves.store();
}

void QuizGame::saveCurrentGame(ConsoleUI& ui) {
    if (updateSaveSlot()) {
        ui.showMessage("Game saved. Returning to menu.");
    } else {
        ui.showError("Could not save the game.");
    }
}

int QuizGame::getQuestionCount() const {
    return (int)m_questions.size();
}

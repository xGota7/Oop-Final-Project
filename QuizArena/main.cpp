#include "QuizGame.h"
#include "ConsoleUI.h"

// Entry point. Create the UI and the game, then start the main menu.
int main() {
    ConsoleUI ui;
    QuizGame game;

    game.run(ui);
    return 0;
}

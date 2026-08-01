#include "QuizGame.h"
#include "ConsoleUI.h"

// Entry point. Creates the user interface and the game, then hands control
// to the main menu. Questions are loaded when the player picks a difficulty.
int main() {
    ConsoleUI ui;
    QuizGame game;

    game.run(ui);
    return 0;
}

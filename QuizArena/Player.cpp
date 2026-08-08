#include "Player.h"

// Create a player with default name and empty session values.
Player::Player()
    : m_name("Player"), m_score(0), m_lives(0) {
}

// Create a player with a name and a starting life count.
Player::Player(const string& name, int lives)
    : m_name(name), m_score(0), m_lives(lives) {
}

// Set the player name.
void Player::setName(const string& name) {
    m_name = name;
}

// Set the score. Negative values are stored as 0.
void Player::setScore(int score) {
    // Keep the stored score non negative.
    if (score < 0) {
        score = 0;
    }
    m_score = score;
}

// Set the life count. Negative values are stored as 0.
void Player::setLives(int lives) {
    // Keep the stored life count non negative.
    if (lives < 0) {
        lives = 0;
    }
    m_lives = lives;
}

// Add points to the score. Only positive values are applied.
void Player::addScore(int points) {
    // Only positive gains are applied. Scoring rules live in QuizGame.
    if (points > 0) {
        m_score += points;
    }
}

// Remove one life if the player still has lives left.
void Player::loseLife() {
    if (m_lives > 0) {
        m_lives--;
    }
}

// Return the player name.
const string& Player::getName() const {
    return m_name;
}

// Return the current score.
int Player::getScore() const {
    return m_score;
}

// Return the remaining lives.
int Player::getLives() const {
    return m_lives;
}

// Return true while the player still has at least one life.
bool Player::isAlive() const {
    return m_lives > 0;
}

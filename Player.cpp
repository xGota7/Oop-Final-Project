#include "Player.h"

Player::Player() {
    m_name = "Player";
    m_score = 0;
    m_lives = 0;
}

Player::Player(const string& name, int lives) {
    m_name = name;
    m_score = 0;
    m_lives = lives;
}

void Player::setName(const string& name) {
    m_name = name;
}

void Player::setScore(int score) {
    if (score < 0) {
        score = 0;
    }
    m_score = score;
}

void Player::setLives(int lives) {
    if (lives < 0) {
        lives = 0;
    }
    m_lives = lives;
}

void Player::addScore(int points) {
    if (points > 0) {
        m_score += points;
    }
}

void Player::loseLife() {
    if (m_lives > 0) {
        m_lives--;
    }
}

const string& Player::getName() const {
    return m_name;
}

int Player::getScore() const {
    return m_score;
}

int Player::getLives() const {
    return m_lives;
}

bool Player::isAlive() const {
    return m_lives > 0;
}

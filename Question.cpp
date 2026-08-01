#include "Question.h"

Question::Question(const string& text, int points) {
    m_text = text;
    m_points = points;
}

Question::~Question() {
}

const string& Question::getText() const {
    return m_text;
}

int Question::getPoints() const {
    return m_points;
}

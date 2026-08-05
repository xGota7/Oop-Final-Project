#include "Question.h"

// Store the shared text and point value for every question type.
Question::Question(const string& text, int points) {
    m_text = text;
    m_points = points;
}

// Empty on purpose. Question itself owns no dynamic memory.
// The destructor is virtual so derived cleanup still runs when the
// object is deleted through a Question*.
Question::~Question() {
}

// Return the question text.
const string& Question::getText() const {
    return m_text;
}

// Return the points awarded for a correct answer.
int Question::getPoints() const {
    return m_points;
}

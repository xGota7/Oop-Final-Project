#include "TrueFalseQuestion.h"

const int TF_OPTION_COUNT = 2;
const int TF_TRUE_INDEX = 0;
const int TF_FALSE_INDEX = 1;

// Create a true or false question and store which side is correct.
TrueFalseQuestion::TrueFalseQuestion(const string& text, int points, bool correctIsTrue)
    : Question(text, points) {
    m_correctIsTrue = correctIsTrue;
}

// No extra cleanup. This class owns no dynamic memory.
TrueFalseQuestion::~TrueFalseQuestion() {
}

// Return how many options this question type has.
int TrueFalseQuestion::getOptionCount() const {
    return TF_OPTION_COUNT;
}

// Return "True", "False", or an empty string if the index is invalid.
string TrueFalseQuestion::getOption(int index) const {
    if (index == TF_TRUE_INDEX) {
        return "True";
    }
    if (index == TF_FALSE_INDEX) {
        return "False";
    }
    return "";
}

// Return true when the chosen index matches the correct side.
bool TrueFalseQuestion::checkAnswer(int index) const {
    if (m_correctIsTrue) {
        return index == TF_TRUE_INDEX;
    }
    return index == TF_FALSE_INDEX;
}

// Return a new copy of this question as a Question*.
Question* TrueFalseQuestion::clone() const {
    return new TrueFalseQuestion(*this);
}

#include "TrueFalseQuestion.h"

const int TF_OPTION_COUNT = 2;
const int TF_TRUE_INDEX = 0;
const int TF_FALSE_INDEX = 1;

TrueFalseQuestion::TrueFalseQuestion(const string& text, int points, bool correctIsTrue)
    : Question(text, points) {
    m_correctIsTrue = correctIsTrue;
}

TrueFalseQuestion::~TrueFalseQuestion() {
}

int TrueFalseQuestion::getOptionCount() const {
    return TF_OPTION_COUNT;
}

string TrueFalseQuestion::getOption(int index) const {
    if (index == TF_TRUE_INDEX) {
        return "True";
    }
    if (index == TF_FALSE_INDEX) {
        return "False";
    }
    return "";
}

bool TrueFalseQuestion::checkAnswer(int index) const {
    if (m_correctIsTrue) {
        return index == TF_TRUE_INDEX;
    }
    return index == TF_FALSE_INDEX;
}

Question* TrueFalseQuestion::clone() const {
    return new TrueFalseQuestion(*this);
}

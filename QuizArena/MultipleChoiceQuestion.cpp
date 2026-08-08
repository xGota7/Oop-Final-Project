#include "MultipleChoiceQuestion.h"

// Create a multiple choice question and copy its four options.
MultipleChoiceQuestion::MultipleChoiceQuestion(const string& text, int points,
                                               const string options[], int correctIndex)
    : Question(text, points), m_correctIndex(correctIndex) {
    // Missing options array: leave option texts empty.
    if (options == nullptr) {
        return;
    }

    // Copy the options so this object does not depend on the caller's array.
    for (int i = 0; i < MC_OPTION_COUNT; i++) {
        m_options[i] = options[i];
    }
}

// No extra cleanup. string members free their own memory.
MultipleChoiceQuestion::~MultipleChoiceQuestion() {
}

// Return how many options this question type has.
int MultipleChoiceQuestion::getOptionCount() const {
    return MC_OPTION_COUNT;
}

// Return one option text, or an empty string if the index is invalid.
string MultipleChoiceQuestion::getOption(int index) const {
    if (index < 0 || index >= MC_OPTION_COUNT) {
        return "";
    }
    return m_options[index];
}

// Return true when the chosen index is the correct option.
bool MultipleChoiceQuestion::checkAnswer(int index) const {
    return index == m_correctIndex;
}

// Members are value types, so the implicit copy constructor already copies
// the object correctly. clone() returns that copy as a Question*.
Question* MultipleChoiceQuestion::clone() const {
    return new MultipleChoiceQuestion(*this);
}

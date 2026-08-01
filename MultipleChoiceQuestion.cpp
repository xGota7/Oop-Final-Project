#include "MultipleChoiceQuestion.h"

MultipleChoiceQuestion::MultipleChoiceQuestion(const string& text, int points,
                                               const string options[], int correctIndex)
    : Question(text, points) {
    for (int i = 0; i < MC_OPTION_COUNT; i++) {
        m_options[i] = options[i];
    }
    m_correctIndex = correctIndex;
}

MultipleChoiceQuestion::~MultipleChoiceQuestion() {
}

int MultipleChoiceQuestion::getOptionCount() const {
    return MC_OPTION_COUNT;
}

string MultipleChoiceQuestion::getOption(int index) const {
    if (index < 0 || index >= MC_OPTION_COUNT) {
        return "";
    }
    return m_options[index];
}

bool MultipleChoiceQuestion::checkAnswer(int index) const {
    return index == m_correctIndex;
}

// Deep copy: every member is a value type (string handles its own memory),
// so the implicit copy constructor already performs a correct deep copy.
Question* MultipleChoiceQuestion::clone() const {
    return new MultipleChoiceQuestion(*this);
}

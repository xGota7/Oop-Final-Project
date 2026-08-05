#ifndef MULTIPLE_CHOICE_QUESTION_H
#define MULTIPLE_CHOICE_QUESTION_H

#include "Question.h"

// Every multiple choice question has exactly four answers.
const int MC_OPTION_COUNT = 4;

// Concrete Question with four options and one correct answer.
// A fixed array is used because the number of options never changes.
class MultipleChoiceQuestion : public Question {
private:
    string m_options[MC_OPTION_COUNT];
    int m_correctIndex; // index into m_options, starting from 0

public:
    MultipleChoiceQuestion(const string& text, int points,
                           const string options[], int correctIndex);
    ~MultipleChoiceQuestion();

    int getOptionCount() const;
    string getOption(int index) const;
    bool checkAnswer(int index) const;
    Question* clone() const;
};

#endif

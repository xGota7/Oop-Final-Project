#ifndef MULTIPLE_CHOICE_QUESTION_H
#define MULTIPLE_CHOICE_QUESTION_H

#include "Question.h"

const int MC_OPTION_COUNT = 4;

// A question with four answer options and exactly one correct option.
class MultipleChoiceQuestion : public Question {
private:
    string m_options[MC_OPTION_COUNT];
    int m_correctIndex; // 0-based index into m_options

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

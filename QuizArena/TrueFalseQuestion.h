#ifndef TRUE_FALSE_QUESTION_H
#define TRUE_FALSE_QUESTION_H

#include "Question.h"

// Concrete Question answered with True or False.
// Option text is built when asked, so the class stores only the correct side.
class TrueFalseQuestion : public Question {
private:
    bool m_correctIsTrue; // true means the correct option is True

public:
    TrueFalseQuestion(const string& text, int points, bool correctIsTrue);
    ~TrueFalseQuestion();

    int getOptionCount() const;
    string getOption(int index) const;
    bool checkAnswer(int index) const;
    Question* clone() const;
};

#endif

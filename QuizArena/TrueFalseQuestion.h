#ifndef TRUE_FALSE_QUESTION_H
#define TRUE_FALSE_QUESTION_H

#include "Question.h"

// A question answered with True or False. The two option strings are generated
// on demand rather than stored.
class TrueFalseQuestion : public Question {
private:
    bool m_correctIsTrue;

public:
    TrueFalseQuestion(const string& text, int points, bool correctIsTrue);
    ~TrueFalseQuestion();

    int getOptionCount() const;
    string getOption(int index) const;
    bool checkAnswer(int index) const;
    Question* clone() const;
};

#endif

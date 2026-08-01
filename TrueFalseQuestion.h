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
    virtual ~TrueFalseQuestion();

    virtual int getOptionCount() const;
    virtual string getOption(int index) const;
    virtual bool checkAnswer(int index) const;
    virtual Question* clone() const;
};

#endif

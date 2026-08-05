#ifndef QUESTION_H
#define QUESTION_H

#include <string>
using namespace std;

// Abstract base for every question type.
// Stores the shared text and point value, and defines the polymorphic
// interface used by QuizGame and ConsoleUI. Derived classes implement
// the behaviour that depends on the question type. This class does no
// input or output, and is never instantiated directly.
class Question {
protected:
    string m_text;
    int m_points;

public:
    Question(const string& text, int points);

    // Virtual so a derived object can be deleted safely through Question*.
    virtual ~Question();

    virtual int getOptionCount() const = 0;
    virtual string getOption(int index) const = 0;
    virtual bool checkAnswer(int index) const = 0;

    // Allocates a copy of the actual derived type. Needed so QuizGame can
    // deep copy vector<Question*>. The caller owns the returned pointer.
    virtual Question* clone() const = 0;

    const string& getText() const;
    int getPoints() const;
};

#endif

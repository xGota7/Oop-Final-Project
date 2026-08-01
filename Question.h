#ifndef QUESTION_H
#define QUESTION_H

#include <string>
using namespace std;

// Abstract base class for every quiz question.
// It stores the data shared by all question types (text and points) and
// declares the polymorphic interface that each concrete type must implement.
// It is never instantiated directly because of the pure virtual methods.
class Question {
protected:
    string m_text;
    int m_points;

public:
    Question(const string& text, int points);
    virtual ~Question();

    // Polymorphic interface: each concrete question type behaves differently.
    virtual int getOptionCount() const = 0;
    virtual string getOption(int index) const = 0;
    virtual bool checkAnswer(int index) const = 0;
    virtual Question* clone() const = 0;

    // Behaviour shared by all question types.
    const string& getText() const;
    int getPoints() const;
};

#endif

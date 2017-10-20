#ifndef INSTRUCTION_STATE_H
#define INSTRUCTION_STATE_H

#include "state.h"

class InstructionState : public State {
  public:
    InstructionState(Engine* engine, ErrorHandler* errorHandler);

    virtual ~InstructionState();
    virtual void load();

    double timer = 1;
  private:
    virtual void setup();
    virtual void update(double seconds);

    std::tuple<int, int> getCenterForImage(Image*);

    int a0 = 0;
    int a1 = 0;
    int a2 = 0;
    int a3 = 0;
    int a4 = 0;
    int a5 = 0;
    int a6 = 0;
    int a7 = 0;
    int a8 = 0;
    int bar_w = 0;
    double newA = 0;
    double speed = 50;

    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool run = false;
    bool attack = false;

    std::string skip = "Press [1] to SKIP";
    std::string i1 = "MOVE AROUND by pressing w,a,s,d or the arrow keys";
    std::string w = "[w] or [up]";
    std::string a = "[a] or [left]";
    std::string s = "[s] or [down]";
    std::string d = "[d] or [right]";
    std::string i2 = "Hold [SHIFT] to run and Press [SPACE] to attack";
    std::string sh = "[SHIFT]";
    std::string sp = "[SPACE]";
    std::string end = "Now Go Out There and Defend You Castle! (next [n])";
};

#endif

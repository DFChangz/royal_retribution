#ifndef HIGHSCORE_STATE_H
#define HIGHSCORE_STATE_H

#include "state.h"
#include <algorithm>
#include <functional>

class HighscoreState : public State {
  public:
    HighscoreState(Engine* engine, ErrorHandler* errorHandler);

    virtual ~HighscoreState();
    void update(double seconds);
    void load();
    void setup();

  private:
    int TOTAL = 10;
    std::string line;
    std::ifstream file;
    std::vector<int> scores;

    int a0 = 0;
    int a1 = 0;
    int a2 = 0;
    int a3 = 0;
    int a4 = 0;
    int a5 = 0;
    int a6 = 0;
    int a7 = 0;
    int a8 = 0;
    int a9 = 0;
    int a10 = 0;

    double speed = 50;
};

#endif

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
};

#endif

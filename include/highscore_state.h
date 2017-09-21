#ifndef HIGHSCORE_STATE_H
#define HIGHSCORE_STATE_H

#include "state.h"

class HighscoreState : public State {
    public:
      HighscoreState(Engine* engine, ErrorHandler* errorHandler);

      virtual ~HighscoreState();

      void update(double seconds);

      void load();
      
      void setup();

    private:
      int scores = 0;
};

#endif

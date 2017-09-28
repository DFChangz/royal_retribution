#ifndef PLAYING_STATE_H
#define PLAYING_STATE_H

#include "state.h"

class PlayingState : public State {
  public:
    PlayingState(Engine* engine, ErrorHandler* errorHandler);

    virtual ~PlayingState();

    double timer = 1;
  private:
    int currentScore;
    virtual void setup();
    void update(double);
};

#endif

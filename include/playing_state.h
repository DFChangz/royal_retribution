#ifndef PLAYING_STATE_H
#define PLAYING_STATE_H

#include "state.h"

#define NUM_ENEMIES     10
#define ENEMY_FILENAME  "level_1_enemies.txt"

class PlayingState : public State {
  public:
    PlayingState(Engine* engine, ErrorHandler* errorHandler);

    virtual ~PlayingState();
    virtual void load();

    double timer = 1;
  private:
    int currentScore;
    void update(double);
    virtual void setup();

    void checkFollow();
    void enemyFollow();

    int num_enemies = 0;
};

#endif

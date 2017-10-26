#ifndef LEVEL_3_STATE_H
#define LEVEL_3_STATE_H

#include "state.h"

#define NUM_ENEMIES     10
#define ENEMY_FILENAME  "level_1_enemies.txt"

class Level_3_State : public State {
  public:
    Level_3_State(Engine* engine, ErrorHandler* errorHandler);

    virtual ~Level_3_State();
    virtual void load();
    void updateHearts();
    void updateHeartsPlus();

    double timer = 1;
  private:
    int currentLevel;
    int currentScore;
    void update(double);
    virtual void setup();

    void updateSta();
    void updateExp();
    void checkFollow();
    void enemyFollow();

    int num_lights = 0;
    int num_enemies = 0;
};

#endif

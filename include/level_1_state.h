#ifndef LEVEL_1_STATE_H
#define LEVEL_1_STATE_H

#include "playing_state.h"

class Level_1_State : public PlayingState {
  public:
    Level_1_State(Engine* engine, ErrorHandler* errorHandler);

    virtual ~Level_1_State();
    virtual void load();
//    void updateHearts();
//    void updateHeartsPlus();

//    double timer = 1;
  private:
//    int currentLevel;
//    int currentScore;
    void update(double);
    virtual void setup();

/*    void updateSta();
    void updateExp();
    void checkFollow();
    void enemyFollow();

    bool skipPan = false;

    int num_lights = 0;
    int num_enemies = 0;*/
};

#endif

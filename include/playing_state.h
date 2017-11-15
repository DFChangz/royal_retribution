#ifndef PLAYING_STATE_H
#define PLAYING_STATE_H

#include "state.h"

//#define NUM_ENEMIES     10
//#define ENEMY_FILENAME  "level_1_enemies.txt"

class PlayingState : public State {
  public:
    PlayingState(Engine* engine, ErrorHandler* errorHandler);

    virtual ~PlayingState();
    virtual void load();
    virtual void activateInstructionText(int);
    void deactivateInstructionText();
    void updateHearts();
    void updateHeartsPlus();

    static int instrGiven;
    static int fallen;
    double timer = 1;

    virtual void enter();
  protected:
    int currentLevel;
    int currentScore;

    void setupFood();
    void update(double);
    virtual void setup();

    int num_food = 0;
    int num_lights = 0;
    int num_enemies = 0;

    bool eat0 = false;
    bool eat1 = false;
    bool eat2 = false;

    bool skipPan = false;

    int a0 = 0;
    int a1 = 255;

    virtual void cleanup();
};

#endif

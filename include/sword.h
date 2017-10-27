#ifndef SWORD_H
#define SWORD_H

#define CHARACTER_FPS       5
#define ATTACK_FRAMES       3

#define D_SWORD_POS         0
#define U_SWORD_POS         3
#define L_SWORD_POS         6
#define R_SWORD_POS         9

#include "audio.h"
#include "sprite.h"
#include "constants.h"
#include "event_handler.h"

class State;
class Sword : public Sprite {
  public:
    Sword(SDL_Renderer *renderer, std::string filename,
      ErrorHandler *errorHandler, int, int, int, int, Sprite* king_p,
      EventHandler *eventHandler, Audio *audioHandler, State *state_p);

    bool isSword() { return true; }

    virtual void update(double seconds);
    virtual void render(Camera*, double interpol_alpha = 1);
    virtual void notifyCollision(Image*, SDL_Rect* intersection);

    virtual void cleanup();

    ~Sword();

  private:
    void createListeners(EventHandler*);

    Sprite* king = nullptr;
    State* state = nullptr;
    Audio *audioHandler = nullptr;

    bool hit = false;
    bool attacking = false;
    bool lastAttack = false;
    bool interacting = false;

    double attackingTimer = 0;

    std::string dir = "";
    std::string collisionDir = "";
};

#endif

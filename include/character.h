#ifndef CHARACTER_H
#define CHARACTER_H

#define CHARACTER_FPS   5
#define RUNNING_FRAMES    4
#define ATTACK_FRAMES     3
#define IDLE_FRAMES       1
#define R_RUNNING_POS     12
#define L_RUNNING_POS     8
#define D_RUNNING_POS     0
#define U_RUNNING_POS     4
#define D_ATTACK_POS      32
#define U_ATTACK_POS      35
#define L_ATTACK_POS      38
#define R_ATTACK_POS      41
#define INVINCIBLE_TIME   3.0
#define STA_WAITING_TIME  3.0

#include "audio.h"
#include "sprite.h"
#include "constants.h"
#include "event_handler.h"
#include "camera.h"

/*class for the player character that inherits from sprite will be 
responsible for player movement events and holds certain sfx along with
its position and rect*/
class State;
class Character : public Sprite {
  public:
    Character(SDL_Renderer *renderer, std::string, ErrorHandler *error_handler,
      int width, int height, int pos_x, int pos_y, EventHandler*, Audio*,
      State*);

    Character(SDL_Renderer *renderer, std::string, ErrorHandler *error_handler,
     int pos_x, int pos_y, EventHandler*, Audio*, State*);

    virtual void notifyCollision(Image*, SDL_Rect* intersection);
    virtual void update(double seconds);
    virtual void render(Camera*);
    virtual void cleanup();

    int maxH = 6;
    int hearts = 6;

    double sta = 1;
    double exp = 1;

    bool frozen = false;
    bool running = false;
    bool leveledUp = false;

    std::string dir = "down";
    std::vector<Sprite*> inventory;

    ~Character();

  private:
    void updateSta();
    void updateExp();
    void idleAnimation(double seconds);
    void createListeners(EventHandler*);
    Audio *audioHandler = nullptr;

    State* state = nullptr;

    bool attacking = false;
    bool lastAttack = false;
    bool invincible = false;
    bool interacting = false;

    double expInc = 0.2;
    double attackingTimer = 0;
    double staSec = STA_WAITING_TIME;
    double invincibilitySeconds = INVINCIBLE_TIME;
};

#endif

#include "main_boss.h"

MainBoss::MainBoss(SDL_Renderer *renderer, std::string filename, ErrorHandler *error_handler,
    int width, int height, int pos_x, int pos_y, double velocity_x,
    double velocity_y, int health)
    : Boss_Enemy(renderer, filename, error_handler, width, height, pos_x,
    pos_y, velocity_x, velocity_y, health) {
}

void MainBoss::update(double seconds){}
void MainBoss::notifyCollision(Image*, doubleRect*, bool resolved){}

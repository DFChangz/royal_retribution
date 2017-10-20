/*
 * state.cpp
 */

#include "state.h"

State::State(Engine* engine_ref, ErrorHandler* errorHandler) :
  audioHandler(errorHandler), errorHandler(errorHandler), camera(errorHandler) {

  engine = engine_ref;

  errorHandler = &engine->error_handler;
}

/*State::State(Engine* engine_ref,ErrorHandler* errorHandler,Image* sentKing) :
  audioHandler(errorHandler), errorHandler(errorHandler), camera(errorHandler) {

  engine = engine_ref;

  errorHandler = &engine->error_handler;

  king = sentKing;
}*/

void State::run(double* seconds) {
  if (!paused) {
    eventHandler.check();
    update(*seconds);
    collisionDetector.check(&camera, map);
  } else {
    pauseUpdate(*seconds);
  }

  render();

  *seconds = 0;
}

void State::load() {
  audioHandler.load();

  for (it = images.begin(); it != images.end(); it++) {
    it->second->load();
    collisionDetector.updateBuckets(it->second, map);
  }
}

void State::update(double seconds) {
  int counter = 0;

  for (it = images.begin(); it != images.end(); it++) {
    SDL_Rect cameraRect = camera.getRect();

    if (it->second->getDestRect()) {
      SDL_Rect imageRect = {(int) it->second->pos_x, (int) it->second->pos_y,
      it->second->getDestRect()->w, it->second->getDestRect()->h};

      if (!SDL_HasIntersection(&imageRect, &cameraRect))
        continue;
    }

    it->second->update(seconds);

    collisionDetector.updateBuckets(it->second, map);

    counter++;
  }
  if (map != nullptr) {
    map->update(seconds);
  }
  camera.updatePosition();
  audioHandler.setVolume(engine->volume);
}

void State::pauseUpdate(double) {}

void State::render() {
  if (SDL_RenderClear(engine->renderer) < 0) {
    errorHandler->quit(__func__, SDL_GetError());
  }
  if (map != nullptr) {
    map->render(&camera);
  }

  for (it = images.begin(); it != images.end(); it++) {
    if (!it->second->isEnemy() || !static_cast<Enemy*>(it->second)->isDead())
      it->second->render(&camera);
  }

  SDL_RenderPresent(engine->renderer);
}

/* fades in a texture */
int State::fadeIn(std::string s, int a, double seconds, double mult) {
  newA = (double)a + speed * seconds * mult;
  if (newA < 255) {
    a = (int)newA;
    SDL_SetTextureAlphaMod(images[s]->getTexture(), a);
  } else {
    SDL_SetTextureAlphaMod(images[s]->getTexture(), 255);
  }
  return a;
}

void State::cleanup() {
  audioHandler.cleanup();

  for (it = images.begin(); it != images.end(); it++) {
    if (it->second != nullptr) {
      delete it->second;
      it->second = nullptr;
    }
	}

  if (map != nullptr) {
    delete map;
    map = nullptr;
  }
}

State::~State() {
  cleanup();
}

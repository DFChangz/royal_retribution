/*
 * state.cpp
 */

#include "state.h"

State::State(Engine* engine_ref, ErrorHandler* errorHandler) : audioHandler(errorHandler), errorHandler(errorHandler), camera(errorHandler) {
  engine = engine_ref;

  errorHandler = &engine->error_handler;
}

void State::run(double seconds) {
  std::cout << "start run\n";
  if (!paused) {
    eventHandler.check();
    std::cout << "does state check\n";

    update(seconds);
    std::cout << "does state update\n";

    collisionDetector->check(&images, map);
    std::cout << "does collisionDetector\n";
  } else {
    pauseUpdate(seconds);
  }
  std::cout << "b4 render\n";
  render();
  std::cout << "after render\n";
}

void State::load() {
  audioHandler.load();

  for (it = images.begin(); it != images.end(); it++) {
    it->second->load();
  }
}

void State::update(double seconds) {
  int counter = 0;
  for (it = images.begin(); it != images.end(); it++) {
    it->second->update(seconds);
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

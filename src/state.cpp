/*
 * state.cpp
 */

#include "state.h"

State::State(Engine* engine_ref, ErrorHandler* errorHandler) : audioHandler(errorHandler), errorHandler(errorHandler), camera(errorHandler) {
  engine = engine_ref;

  errorHandler = &engine->error_handler;
}

void State::run(double seconds) {
  if (!paused) {
    eventHandler.check();

    update(seconds);

    collisionDetector->check(&images, map);
  } else {
    pauseUpdate(seconds);
  }

  render();
}

void State::load() {
  audioHandler.load();

  for (Image* image : images) {
    image->load();
  }
}

void State::update(double seconds) {
  int counter = 0;
  for (Image* image : images) {
    image->update(seconds);
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

  for (Image* image : images) {
    if (!image->isEnemy() || !static_cast<Enemy*>(image)->isDead())
      image->render(&camera);
  }

  SDL_RenderPresent(engine->renderer);
}

void State::cleanup() {
  audioHandler.cleanup();

  for (Image* image : images) {
    if (image != nullptr) {
      delete image;
      image = nullptr;
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

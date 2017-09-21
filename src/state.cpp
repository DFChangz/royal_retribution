/*
 * state.cpp
 */

#include "state.h"

State::State(Engine* engine_ref, ErrorHandler* errorHandler) : audioHandler(errorHandler), errorHandler(errorHandler) {
  engine = engine_ref;

  errorHandler = &engine->error_handler;
}

void State::run(double seconds) {
  eventHandler.check();

  update(seconds);

  collisionDetector->check(&images);

  render();
}

void State::load() {
  audioHandler.load();

  for (Image* image : images) {
    image->load();
  }
}

void State::update(double seconds) {
  int counter =0 ;
  for (Image* image : images) {
    image->update(seconds);
    counter++;
  }
}

void State::render() {
  if (SDL_RenderClear(engine->renderer) < 0) {
    errorHandler->quit(__func__, SDL_GetError());
  }

  for (Image* image : images) {
    image->render();
  }

  SDL_RenderPresent(engine->renderer);
}

void State::cleanup() {
  audioHandler.cleanup();

  for (Image* image : images) {
    if (image != nullptr) delete image;
	}
}

State::~State() {
  cleanup();
}

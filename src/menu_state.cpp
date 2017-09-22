#include "menu_state.h"

MenuState::MenuState(Engine* engine, ErrorHandler* errorHandler)
  : State(engine, errorHandler) {

	setup();
  load();
}

void MenuState::setup() {
  images.push_back(new Image(engine->renderer, CASTLE_FILENAME, errorHandler));
  images.push_back(new Sprite(engine->renderer, SHIP_FILENAME, errorHandler, 0,
    0, false));
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler, 50,
    50, 70, WINDOW_TITLE, ROYAL_GOLD));
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler, 50,
    50, 40, "New Game"));
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler, 50,
    50, 40, "High Scores"));
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler, 50,
    50, 40, "Quit"));
}

void MenuState::load() {
  State::load();

  auto center = getCenterForImage(images[2]);
  images[2]->setPosition(std::get<0>(center), std::get<1>(center) - 200);

  center = getCenterForImage(images[3]);
  images[3]->setPosition(std::get<0>(center), std::get<1>(center) - 40);

  center = getCenterForImage(images[4]);
  images[4]->setPosition(std::get<0>(center), std::get<1>(center) + 10);

  center = getCenterForImage(images[5]);
  images[5]->setPosition(std::get<0>(center), std::get<1>(center) + 60);

  eventHandler.addListener(SDL_MOUSEMOTION, [&] (SDL_Event* e) {
    SDL_SetTextureColorMod(images[3]->getTexture(), 255, 255, 255);
    SDL_SetTextureColorMod(images[4]->getTexture(), 255, 255, 255);
    SDL_SetTextureColorMod(images[5]->getTexture(), 255, 255, 255);
    
    images[1]->setPosition(e->motion.x - 100, e->motion.y - 75);
  });

  images[3]->onHover(&eventHandler, [&] () {
    SDL_SetTextureColorMod(images[3]->getTexture(), 255, 69, 0);
  });
  images[3]->onClick(&eventHandler, [&] () {
    engine->setState("playing");
  });

  images[4]->onHover(&eventHandler, [&] () {
    SDL_SetTextureColorMod(images[4]->getTexture(), 255, 69, 0);
  });
  images[4]->onClick(&eventHandler, [&] () {
    engine->setState("Highscore");
  });

  images[5]->onHover(&eventHandler, [&] () {
    SDL_SetTextureColorMod(images[5]->getTexture(), 255, 69, 0);
  });
  images[5]->onClick(&eventHandler, [&] () {
    engine->quit();
  });
}

MenuState::~MenuState() {}

std::tuple<int, int> MenuState::getCenterForImage(Image* image) {
  int x = WIDTH / 2 - image->getDestRect()->w / 2;
  int y = HEIGHT / 2 - image->getDestRect()->h / 2;
  return std::tuple<int, int>(x, y);
}

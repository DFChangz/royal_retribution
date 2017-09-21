#include "menu_state.h"

MenuState::MenuState(Engine* engine, ErrorHandler* errorHandler)
  : State(engine, errorHandler) {

	setup();
}

void MenuState::setup() {
  images.push_back(new Image(engine->renderer, BG_FILENAME, errorHandler));
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler, 50, 50, 32, "Testing"));
}

void MenuState::update(double seconds) {
  State::update(seconds);

  timer += seconds;

  images[1]->getDestRect()->x = WIDTH / 2 - images[1]->getDestRect()->w / 2;
  images[1]->getDestRect()->y = HEIGHT / 2 - images[1]->getDestRect()->h / 2;

  if (timer > 1) {
    engine->setState("playing");
    std::cout << "Switched States" << std::endl;;
  }
}

MenuState::~MenuState() {}

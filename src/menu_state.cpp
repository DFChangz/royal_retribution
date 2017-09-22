#include "menu_state.h"

MenuState::MenuState(Engine* engine, ErrorHandler* errorHandler)
  : State(engine, errorHandler) {

	setup();
  load();

  selectedIndex = 3;
}

void MenuState::setup() {
  // Create all the images needed for the text, spaceship pointer and background.
  images.push_back(new Image(engine->renderer, CASTLE_FILENAME, errorHandler));
  images.push_back(new Sprite(engine->renderer, SHIP_FILENAME, errorHandler,
    0, 0, false));
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

  // change ship size
  images[1]->getDestRect()->w = 50;
  images[1]->getDestRect()->h = 38;

  // Center all the text (lines 35-47)
  auto center = getCenterForImage(images[2]);
  images[2]->setPosition(std::get<0>(center), std::get<1>(center) - 200);

  center = getCenterForImage(images[3]);
  images[3]->setPosition(std::get<0>(center), std::get<1>(center) - 40);
  images[1]->setPosition(std::get<0>(center) - 55, std::get<1>(center) - 40);
  SDL_SetTextureColorMod(images[3]->getTexture(), 255, 69, 0);

  center = getCenterForImage(images[4]);
  images[4]->setPosition(std::get<0>(center), std::get<1>(center) + 10);

  center = getCenterForImage(images[5]);
  images[5]->setPosition(std::get<0>(center), std::get<1>(center) + 60);


  // Add mouse movement event listeners to reset link colors to white if they
  // are no longer selected.
  eventHandler.addListener(SDL_MOUSEMOTION, [&] (SDL_Event*) {
    if (selectedIndex != 3)
      SDL_SetTextureColorMod(images[3]->getTexture(), 255, 255, 255);
    if (selectedIndex != 4)
      SDL_SetTextureColorMod(images[4]->getTexture(), 255, 255, 255);
    if (selectedIndex != 5)
      SDL_SetTextureColorMod(images[5]->getTexture(), 255, 255, 255);
  });

  // If the down arrow key is pressed, this listener will set the link colors
  // correctly and then move the ship icon to the correct text entry
  eventHandler.addListener(SDL_KEYUP, [&] (SDL_Event*) {
    if (selectedIndex < 5)
      selectedIndex += 1;

    SDL_SetTextureColorMod(images[3]->getTexture(), 255, 255, 255);
    SDL_SetTextureColorMod(images[4]->getTexture(), 255, 255, 255);
    SDL_SetTextureColorMod(images[5]->getTexture(), 255, 255, 255);
    SDL_SetTextureColorMod(images[selectedIndex]->getTexture(), 255, 69, 0);

    int x = images[selectedIndex]->getDestRect()->x - 55;
    int y = images[selectedIndex]->getDestRect()->y;
    images[1]->setPosition(x, y);
  }, SDLK_DOWN);

  // Up key event listener (similar to last listener)
  eventHandler.addListener(SDL_KEYUP, [&] (SDL_Event*) {
    if (selectedIndex > 3)
      selectedIndex -= 1;

      SDL_SetTextureColorMod(images[3]->getTexture(), 255, 255, 255);
      SDL_SetTextureColorMod(images[4]->getTexture(), 255, 255, 255);
      SDL_SetTextureColorMod(images[5]->getTexture(), 255, 255, 255);
      SDL_SetTextureColorMod(images[selectedIndex]->getTexture(), 255, 69, 0);

      int x = images[selectedIndex]->getDestRect()->x - 55;
      int y = images[selectedIndex]->getDestRect()->y;
      images[1]->setPosition(x, y);
  }, SDLK_UP);

  // Enter key listeners for currently selected entry
  eventHandler.addListener(SDL_KEYUP, [&] (SDL_Event*) {
    if (selectedIndex == 3)
      engine->setState("playing");
    else if (selectedIndex == 4)
      engine->setState("Highscore");
    else if (selectedIndex == 5)
      engine->quit();
  }, SDLK_RETURN);

  // on hover and click events for the menu entries
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

// Gets the x and y positions for an image to center it in the window.
std::tuple<int, int> MenuState::getCenterForImage(Image* image) {
  int x = WIDTH / 2 - image->getDestRect()->w / 2;
  int y = HEIGHT / 2 - image->getDestRect()->h / 2;
  return std::tuple<int, int>(x, y);
}

#include "menu_state.h"
#include "audio.h"

MenuState::MenuState(Engine* engine, ErrorHandler* errorHandler)
  : State(engine, errorHandler) {

  setup();
  load();

  selectedIndex = 3;
}

void MenuState::setup() {
  // Create all the images needed for the text, spaceship pointer & background.
  images["home"] = new Image(engine->renderer, CASTLE_FILENAME, errorHandler);
  images["sh"] = new Sprite(engine->renderer, SHIP_FILENAME, errorHandler,
    0, 0, false);
  images["TL"] = new Text(engine->renderer, FONT_FILENAME, errorHandler, 50,
    50, 80, WINDOW_TITLE, ROYAL_GOLD);
  images["NG"] = new Text(engine->renderer, FONT_FILENAME, errorHandler, 50,
    50, 40, "New Game");
  images["HS"] = new Text(engine->renderer, FONT_FILENAME, errorHandler, 50,
    50, 40, "High Scores");
  images["CR"] = new Text(engine->renderer, FONT_FILENAME, errorHandler, 50,
    50, 40, "Credits");
  images["QT"] = new Text(engine->renderer, FONT_FILENAME, errorHandler, 50,
    50, 40, "Quit");
  images["BR"] = new Text(engine->renderer, FONT_FILENAME, errorHandler, 80,
    500, 40, "Brightness: ");
  images["12"] = new Text(engine->renderer, FONT_FILENAME, errorHandler, 80,
    540, 20, "(Use keys 1 and 2 to increase/decrease)");
  images["BRnum"] = new Text(engine->renderer, FONT_FILENAME, errorHandler, 350,
    500, 40, "100%");
  images["VM"] = new Text(engine->renderer, FONT_FILENAME, errorHandler, 800,
    500, 40, "Volume: ");
  images["90"] = new Text(engine->renderer, FONT_FILENAME, errorHandler, 800,
    540, 20, "(Use keys 9 and 0 to increase/decrease)");
  images["VMnm"] = new Text(engine->renderer, FONT_FILENAME, errorHandler, 1000,
    500, 40, "100%");
}

void MenuState::load() {
  State::load();

  // change ship size
  images["sh"]->getDestRect()->w = 50;
  images["sh"]->getDestRect()->h = 38;

  // Center all the text (lines 35-47)
  auto center = getCenterForImage(images["TL"]);
  images["TL"]->setPosition(std::get<0>(center), std::get<1>(center) - 220);

  center = getCenterForImage(images["NG"]);
  images["NG"]->setPosition(std::get<0>(center), std::get<1>(center) - 40);
  images["sh"]->setPosition(std::get<0>(center) - 55, std::get<1>(center) - 40);
  SDL_SetTextureColorMod(images["NG"]->getTexture(), 255, 69, 0);

  center = getCenterForImage(images["HS"]);
  images["HS"]->setPosition(std::get<0>(center), std::get<1>(center) + 10);

  center = getCenterForImage(images["CR"]);
  images["CR"]->setPosition(std::get<0>(center), std::get<1>(center) + 60);

  center = getCenterForImage(images["QT"]);
  images["QT"]->setPosition(std::get<0>(center), std::get<1>(center) + 110);

  // Add mouse movement event listeners to reset link colors to white if they
  // are no longer selected.
  eventHandler.addListener(SDL_MOUSEMOTION, [&] (SDL_Event*) {
    if (selectedIndex != 3)
      SDL_SetTextureColorMod(images["NG"]->getTexture(), 255, 255, 255);
    if (selectedIndex != 4)
      SDL_SetTextureColorMod(images["HS"]->getTexture(), 255, 255, 255);
    if (selectedIndex != 5)
      SDL_SetTextureColorMod(images["CR"]->getTexture(), 255, 255, 255);
    if (selectedIndex != 6)
      SDL_SetTextureColorMod(images["QT"]->getTexture(), 255, 255, 255);
  });

  // If the down arrow key is pressed, this listener will set the link colors
  // correctly and then move the ship icon to the correct text entry
  eventHandler.addListener(SDL_KEYUP, [&] (SDL_Event*) {
    if (selectedIndex < 6)
      selectedIndex += 1;

    std::string s = translate(selectedIndex);

    SDL_SetTextureColorMod(images["NG"]->getTexture(), 255, 255, 255);
    SDL_SetTextureColorMod(images["HS"]->getTexture(), 255, 255, 255);
    SDL_SetTextureColorMod(images["CR"]->getTexture(), 255, 255, 255);
    SDL_SetTextureColorMod(images["QT"]->getTexture(), 255, 255, 255);
    SDL_SetTextureColorMod(images[s]->getTexture(), 255, 69, 0);

    int x = images[s]->getDestRect()->x - 55;
    int y = images[s]->getDestRect()->y;
    images["sh"]->setPosition(x, y);
  }, SDLK_DOWN);

  // Up key event listener (similar to last listener)
  eventHandler.addListener(SDL_KEYUP, [&] (SDL_Event*) {
    if (selectedIndex > 3)
      selectedIndex -= 1;
      std::string s = translate(selectedIndex);

      SDL_SetTextureColorMod(images["NG"]->getTexture(), 255, 255, 255);
      SDL_SetTextureColorMod(images["HS"]->getTexture(), 255, 255, 255);
      SDL_SetTextureColorMod(images["CR"]->getTexture(), 255, 255, 255);
      SDL_SetTextureColorMod(images["QT"]->getTexture(), 255, 255, 255);
      SDL_SetTextureColorMod(images[s]->getTexture(), 255, 69, 0);

      int x = images[s]->getDestRect()->x - 55;
      int y = images[s]->getDestRect()->y;
      images["sh"]->setPosition(x, y);
  }, SDLK_UP);

  // Enter/Space key listeners for currently selected entry
  eventHandler.addListener(SDL_KEYUP, [&] (SDL_Event*) {
    transition();
  }, SDLK_RETURN);
  eventHandler.addListener(SDL_KEYUP, [&] (SDL_Event*) {
    transition();
  }, SDLK_SPACE);

  // Key listeners for brightness and volume
  eventHandler.addListener(SDL_KEYDOWN, [&] (SDL_Event*) {
    if (brightness > 0) {
      brightness--;
      updateBrightnessVolume();
    }
  }, SDLK_1);
  eventHandler.addListener(SDL_KEYDOWN, [&] (SDL_Event*) {
    if (brightness < 100) {
      brightness++;
      updateBrightnessVolume();
    }
  }, SDLK_2);
  eventHandler.addListener(SDL_KEYDOWN, [&] (SDL_Event*) {
    if (engine->volume > 0) {
      engine->volume--;
      updateBrightnessVolume();
      audioHandler.setVolume(engine->volume);
    }
  }, SDLK_9);
  eventHandler.addListener(SDL_KEYDOWN, [&] (SDL_Event*) {
    if (engine->volume < 100) {
      engine->volume++;
      updateBrightnessVolume();
      audioHandler.setVolume(engine->volume);
    }
  }, SDLK_0);

  // on hover and click events for the menu entries
  images["NG"]->onHover(&eventHandler, [&] () {
    SDL_SetTextureColorMod(images["NG"]->getTexture(), 255, 69, 0);
  });
  images["NG"]->onClick(&eventHandler, [&] () {
    Mix_PauseMusic(); 
    engine->newGame();
    engine->setState("intro");
  });

  images["HS"]->onHover(&eventHandler, [&] () {
    SDL_SetTextureColorMod(images["HS"]->getTexture(), 255, 69, 0);
  });
  images["HS"]->onClick(&eventHandler, [&] () {
    engine->newHighscore();
    engine->setState("Highscore");
  });

  images["CR"]->onHover(&eventHandler, [&] () {
    SDL_SetTextureColorMod(images["CR"]->getTexture(), 255, 69, 0);
  });
  images["CR"]->onClick(&eventHandler, [&] () {
    engine->newHighscore();
    engine->setState("credits");
  });

  images["QT"]->onHover(&eventHandler, [&] () {
    SDL_SetTextureColorMod(images["QT"]->getTexture(), 255, 69, 0);
  });
  images["QT"]->onClick(&eventHandler, [&] () {
    engine->quit();
  });
}

void MenuState::update(double seconds){
  State::update(seconds);  
  if(!audioHandler.isPlaying()){audioHandler.play("intro");}
}

MenuState::~MenuState() {}

void MenuState::transition() {
  if (selectedIndex == 3) {
    Mix_PauseMusic(); 
    engine->newGame();
    engine->setState("intro");
  }
  else if (selectedIndex == 4) {
    engine->newHighscore();
    engine->setState("Highscore");
  }
  else if (selectedIndex == 5) {
    engine->setState("credits");
  }
  else if (selectedIndex == 6) {
    engine->quit();
  }
}

// Gets the x and y positions for an image to center it in the window.
std::tuple<int, int> MenuState::getCenterForImage(Image* image) {
  int x = WIDTH / 2 - image->getDestRect()->w / 2;
  int y = HEIGHT / 2 - image->getDestRect()->h / 2;
  return std::tuple<int, int>(x, y);
}

void MenuState::updateBrightnessVolume() {
  delete images["BRnum"];
  delete images["VMnum"];
  
  images["BRnum"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    350, 500, 40, std::to_string(brightness) + "%");
  images["BRnum"]->load();

  images["VMnum"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    1000, 500, 40, std::to_string(engine->volume) + "%");
  images["VMnum"]->load();
}

// Translates selected index
std::string MenuState::translate(int i) {
  switch(i) {
    case 3:
      return "NG";
    case 4:
      return "HS";
    case 5:
      return "CR";
    case 6:
      return "QT";
  }
  return "THIS WIL WORK!";
}

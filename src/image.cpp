#include "image.h"
//Definitions for the the parent class that any image object inherit from

void Image::load() {
  //Makes a surface where the texture is created from
  surf = IMG_Load(image_file.c_str());
  if (surf == nullptr) {
    error_handler->quit(__func__, IMG_GetError());
  }

  texture = SDL_CreateTextureFromSurface(renderer, surf);
  if (texture == nullptr) {
    SDL_FreeSurface(surf);
    error_handler->quit(__func__, SDL_GetError());
  }

  SDL_FreeSurface(surf);
}

void Image::update(double) {
  
}

void Image::render() {
  if (SDL_RenderCopy(renderer, texture, NULL, NULL) < 0) {
    error_handler->quit(__func__, SDL_GetError());
  }
}

//Destroys the texture to free up memory
void Image::cleanup() {
  if (triggers != nullptr) {
    delete triggers;
    triggers = nullptr;
  }

  if (texture != nullptr) {
    SDL_DestroyTexture(texture);
  }
}

SDL_Rect* Image::getDestRect() {
  std::cerr << "Image does not have a rect. This only works on subclasses. Returning NULL." << std::endl;
  return NULL;
}

SDL_Texture* Image::getTexture() {
  return texture;
}

bool Image::isCollidable() {
  return false;
}


Image::Image(SDL_Renderer *renderer_p, std::string image_file,
  ErrorHandler *error_handler_p) :
    image_file(image_file) {
    
    renderer = renderer_p;
    error_handler = error_handler_p;

    triggers = new std::vector<std::function<void()>>;
}

Image::~Image() {
  cleanup();
}

void Image::setPosition(double, double) {
  error_handler->quit(__func__,
  "Image position is always (0, 0). Did you mean to call this on a subclass?");
}

void Image::onHover(EventHandler *eventHandler, std::function<void()> trigger) {
  triggers->push_back(trigger);

  eventHandler->addListener(SDL_MOUSEMOTION, [&] (SDL_Event* e) {
    int x = e->motion.x;
    int y = e->motion.y;
    SDL_Rect* rect = getDestRect();
    if (rect == NULL) *rect = {0, 0, WIDTH, HEIGHT};

    if (x > rect->x && x < rect->x + rect->w && y > rect->y
      && y < rect->y + rect->h) {
      
      triggers->back()();
    };
  });
}

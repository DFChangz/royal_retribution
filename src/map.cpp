#include "map.h"

Map::Map(SDL_Renderer* renderer_p, ErrorHandler* errorHandler_p,
        std::string map_filename, std::string textures_filename) {

  errorHandler = errorHandler_p;

  renderer = renderer_p;

  loadTextures(textures_filename);

  loadLayout(map_filename);
}

void Map::loadTextures(std::string filename) {
  std::ifstream file("maps/" + filename);
  
  if (!file.is_open()) {
    std::string error = filename + " could not be opened";
    errorHandler->quit(__func__, error.c_str());
  }

  std::string line;
  int id = 0;
  while (std::getline(file, line)) {
    std::istringstream iss(line);
    char sym = -1;
    std::string filename2;
    int options = 0;

    if (!(iss >> sym) || sym == -1 || !(iss >> filename2))
      errorHandler->quit(__func__, "Invalid file contents");

    iss >> options;

    textureIDs[sym] = id;

    createTexture(id, filename2, options);

    id++;
  }

  file.close();
}

void Map::loadLayout(std::string filename) {
  std::ifstream file("maps/" + filename);

  if (!file.is_open()) {
    std::string error = filename + " could not be opened";
    errorHandler->quit(__func__, error.c_str());
  }

  std::string line;
  int row = 0;
  while (std::getline(file, line)) {
    int col = 0;

    std::istringstream iss(line);
    char sym = -1;

    while(iss >> sym) {
      if (sym == -1)
        errorHandler->quit(__func__, "Invalid file contents");
      else if (textureIDs.find(sym) == textureIDs.end())
        errorHandler->quit(__func__, "Texture not found for symbol");

      struct texture* texture = &(textures[textureIDs[sym]]);

      bool collidable = ((texture->options & 1) == 1);

      tiles.push_back(new Sprite(renderer, "", errorHandler, col * TILE_DIM,
        row * TILE_DIM, collidable));

      tiles.back()->load(texture->texture);

      col++;
    }

    col = 0;
    row++;
  }

  file.close();
}

void Map::createTexture(int id, std::string filename, int options) {
  if ((int) textures.size() != id)
    errorHandler->quit(__func__, "Invalid Texture ID. Vector wrong size");

  SDL_Surface* surf = IMG_Load(("assets/" + filename).c_str());
  if (surf == nullptr) errorHandler->quit(__func__, IMG_GetError());

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surf);
  if (texture == nullptr) {
    SDL_FreeSurface(surf);
    errorHandler->quit(__func__, SDL_GetError());
  }
  SDL_FreeSurface(surf);

  struct texture t;
  t.texture = texture;
  t.options = options;

  textures.push_back(t);
}

void Map::update(double seconds) {
  for (auto tile : tiles) {
    tile->update(seconds);
  }
}

void Map::render(Camera* camera) {
  for (auto tile : tiles) {
    tile->render(camera);
  }
}

void Map::cleanup() {
  for (auto tile : tiles) {
    if (tile != nullptr) {
      delete tile;
    }
  }

  for (auto texture : textures) {
    if (texture.texture != nullptr) {
      SDL_DestroyTexture(texture.texture);
      texture.texture = nullptr;
    }
  }
}

Map::~Map() {
  cleanup();
}

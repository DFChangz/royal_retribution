#include "map.h"
#include "camera.h"

Map::Map(SDL_Renderer* renderer_p, Camera* camera_p, ErrorHandler* errorHandler_p,
        std::string map_filename, std::string textures_filename) {

  errorHandler = errorHandler_p;

  camera = camera_p;

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

    if (!(iss >> sym) || sym == -1 || !(iss >> filename2))
      errorHandler->quit(__func__, "Invalid file contents");

    textureIDs[sym] = id;

    createTexture(id, filename2);

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
      
      tile t;
      t.pos_x = col * TILE_DIM;
      t.pos_y = row * TILE_DIM;
      t.texture = textureIDs[sym];

      tiles.push_back(t);
      col++;
    }

    col = 0;
    row++;
  }

  file.close();
}

void Map::createTexture(int id, std::string filename) {
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

  textures.push_back(texture);
}

void Map::render() {
  for (auto tile : tiles) {
    SDL_Rect rect = {tile.pos_x, tile.pos_y, tile.width, tile.height};
    camera->render(renderer, textures[tile.texture], NULL, &rect);
  }
}

void Map::cleanup() {
  for (auto texture : textures) {
    if (texture != nullptr) {
      SDL_DestroyTexture(texture);
      texture = nullptr;
    }
  }
}

Map::~Map() {
  cleanup();
}

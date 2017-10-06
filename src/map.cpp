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
    int start_frame = -1;
    int frame_length = -1;
    int options = 0;

    if (!(iss >> sym) || sym == -1 || !(iss >> start_frame) || start_frame == -1
        || !(iss >> frame_length) || frame_length == -1)
      errorHandler->quit(__func__, "Invalid file contents");

    iss >> options;

    textureIDs[sym] = id;

    createTexture(id, TILE_FILENAME, start_frame, frame_length, options);

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

      tile t;
      t.start_frame = texture->start_frame;
      t.frame_length = texture->frame_length;
      t.image = new Sprite(renderer, "", errorHandler, TILE_DIM, TILE_DIM,
        col * TILE_DIM, row * TILE_DIM, collidable);

      if (collidable) {
        tiles.insert(tiles.begin(), t);
        tiles[0].image->load(texture->texture);
      } else {
        tiles.push_back(t);
        tiles.back().image->load(texture->texture);
      }

      col++;
    }

    int rowWidth = col * TILE_DIM;
    if (rowWidth > width) width = rowWidth;

    col = 0;
    row++;
  }
  height = row * TILE_DIM;

  file.close();
}

void Map::createTexture(int id, std::string filename, int start_frame,
    int frame_length, int options) {

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
  t.start_frame = start_frame;
  t.frame_length = frame_length;
  t.options = options;

  textures.push_back(t);
}

void Map::update(double seconds) {
  for (auto tile : tiles) {
    tile.image->update(seconds);
    tile.image->animate(seconds, tile.start_frame, tile.frame_length + tile.start_frame - 1);
  }
}

void Map::render(Camera* camera) {
  for (auto tile : tiles) {
    tile.image->render(camera);
  }
}

void Map::cleanup() {
  for (auto tile : tiles) {
    if (tile.image != nullptr) {
      delete tile.image;
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

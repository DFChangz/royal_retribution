#include "map.h"

Map::Map(SDL_Renderer* renderer_p, ErrorHandler* errorHandler_p,
        std::string map_filename, std::string textures_filename,
        CollisionDetector* collision_detector_p) {

  collisionDetector = collision_detector_p;

  errorHandler = errorHandler_p;

  renderer = renderer_p;

  loadTextures(textures_filename);

  loadLayout(map_filename);
}

void Map::loadTextures(std::string filename) {
  std::ifstream file(filename);
  
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

    createTexture(id, TILES_IMG, start_frame, frame_length, options, -1);

    id++;
  }

  file.close();
}

void Map::loadLayout(std::string filename) {
  std::ifstream file(filename);

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
        col * TILE_DIM, row * TILE_DIM, collidable, false);

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

  addCollidablesToBuckets(&tiles);
}

void Map::loadSecondLayout(std::string filename) {
  std::ifstream file(filename);

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
      else if (textureExtraIDs.find(sym) == textureExtraIDs.end())
        errorHandler->quit(__func__, "Texture not found for symbol");
      else if (sym == '-'){
        col++;
        continue;
      }

      struct texture* texture = &(textures[textureExtraIDs[sym]]);

      bool collidable = ((texture->options & 1) == 1);
      bool trap = ((texture->options & 2) == 2);
      bool door = ((texture->options & 4) == 4);
      bool torch = ((texture->options & 8) == 8);
      bool chest = ((texture->options & 16) == 16);
      bool hole = ((texture->options & 32) == 32);
      bool blade = ((texture->options & 64) == 64);
      
      if (torch) {
        lights.push_back(new Sprite(renderer, LIGHTS_FILENAME, errorHandler,
          140, 140, TILE_DIM * (col-1), TILE_DIM * (row-1), false, false));
      }

      tile t;
      t.start_frame = texture->start_frame;
      t.frame_length = texture->frame_length;
      t.image = new Sprite(renderer, "", errorHandler, TILE_DIM, TILE_DIM,
        col * TILE_DIM, row * TILE_DIM, collidable, false);
      t.image->pairing = texture->pairing;
      t.image->setTrap(trap);
      t.image->setDoor(door);
      t.image->setChest(chest);
      t.image->setBlade(blade);
      t.image->setHole(hole);
      for(unsigned i = 0; i < additions.size(); i++){
        if(t.image->pairing != -1 && additions[i].image->pairing == t.image->pairing){
          t.image->setPair(additions[i].image);
          additions[i].image->setPair(t.image);
        }
      }

      if (collidable) {
        additions.insert(additions.begin(), t);
        additions[0].image->load(texture->texture);
      } else {
        additions.push_back(t);
        additions.back().image->load(texture->texture);
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
  addCollidablesToBuckets(&additions);
}

void Map::loadSecondTextures(std::string filename) {
  std::ifstream file(filename);
  
  if (!file.is_open()) {
    std::string error = filename + " could not be opened";
    errorHandler->quit(__func__, error.c_str());
  }

  std::string line;
  int id = textures.size();
  while (std::getline(file, line)) {
    std::istringstream iss(line);
    char sym = -1;
    int start_frame = -1;
    int frame_length = -1;
    int options = 0;
    char pairing = -1;

    if (!(iss >> sym) || sym == -1 || !(iss >> start_frame) || start_frame == -1
        || !(iss >> frame_length) || frame_length == -1)
      errorHandler->quit(__func__, "Invalid file contents");

    iss >> options;
    if(!(iss >> pairing) || pairing == -1){
      pairing = -1;
    }

    textureExtraIDs[sym] = id;

    createTexture(id, TILES_IMG, start_frame, frame_length, options, pairing);

    id++;
  }
}
void Map::createTexture(int id, std::string filename, int start_frame,
    int frame_length, int options, char pairing) {

  if ((int) textures.size() != id)
    errorHandler->quit(__func__, "Invalid Texture ID. Vector wrong size");

  SDL_Surface* surf = IMG_Load((filename).c_str());
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
  t.pairing = pairing;

  textures.push_back(t);
}

void Map::update(double seconds) {
  for (auto light : lights) {
    static_cast<Sprite*>(light)->update(seconds);
    static_cast<Sprite*>(light)->animate(seconds, 0, 3);
  }
  for (auto tile : tiles) {
    tile.image->update(seconds);
    tile.image->animate(seconds, tile.start_frame,
      tile.frame_length + tile.start_frame - 1);
  }
  for (auto tile : additions) {
    tile.image->update(seconds);
    collisionDetector->updateBuckets(tile.image, this);
    if(tile.image->isChest()){
      if(tile.image->pair == nullptr){
        tile.image->animate(seconds, tile.start_frame, tile.start_frame);
        continue;
      }
      if(tile.image->pair == tile.image->pair){
        tile.image->animate(seconds, tile.frame_length + tile.start_frame - 1, 
          tile.frame_length + tile.start_frame - 1);
        continue;
      }
    }
    if(tile.image->isBlade()){
      tile.image->animate(seconds, tile.start_frame, tile.frame_length + tile.start_frame - 1, 9);
      continue;
    }
    tile.image->animate(seconds, tile.start_frame, tile.frame_length + tile.start_frame - 1);
  }
}

void Map::render(Camera* camera, double interpol_alpha) {
  for (auto tile : tiles) {
    tile.image->render(camera, interpol_alpha);
  }
  for (auto tile : additions) {
    tile.image->render(camera, interpol_alpha);
  }
}

int Map::pushLights(std::map<std::string, Image*>& images) {
  int counter = 0;
  for (auto light : lights) {
    std::string s = std::to_string(ADD)+"light_"+std::to_string(counter);
    images[s] = light;
    counter++;
  }
  lights.clear();
  return counter;
}

void Map::cleanup() {
  for (auto light : lights) {
    if (light != nullptr) {
      delete light;
      light = nullptr;
    }
  }

  for (auto tile : tiles) {
    if (tile.image != nullptr) {
      delete tile.image;
      tile.image = nullptr;
    }
  }
  for (auto tile : additions) {
    if (tile.image != nullptr) {
      delete tile.image;
      tile.image = nullptr;
    }
  }
  for (auto texture : textures) {
    if (texture.texture != nullptr) {
      SDL_DestroyTexture(texture.texture);
      texture.texture = nullptr;
    }
  }
}

void Map::addCollidablesToBuckets(std::vector<tile>* tiles) {
  for (auto tile = tiles->begin(); tile != tiles->end(); tile++) {
    if (!tile->image->isCollidable()) break;

    collisionDetector->updateBuckets(tile->image, this);
  }
}


Map::~Map() {
  cleanup();
}

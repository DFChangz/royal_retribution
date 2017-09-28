#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "image.h"
#include "error_handler.h"

#define TILE_DIM  15

struct tile {
  int pos_x;
  int pos_y;

  int texture = -1;

  int width = TILE_DIM;
  int height = TILE_DIM;
};

class Map {
  public:
    Map(SDL_Renderer* renderer, Camera *camera, ErrorHandler*, std::string, std::string);
    ~Map();

    std::vector<struct tile> tiles;

    std::map<char, int> textureIDs;

    std::vector<SDL_Texture*> textures;

    void loadTextures(std::string);
    void loadLayout(std::string);

    void cleanup();

    void render();
  private:
    void createTexture(int, std::string);

    ErrorHandler* errorHandler;

    SDL_Renderer* renderer = nullptr;

    Camera* camera;
};

#endif

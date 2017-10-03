#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "sprite.h"
#include "error_handler.h"

#define TILE_DIM  15

struct texture {
  SDL_Texture* texture;
  int options = 0;
};

class Map {
  public:
    Map(SDL_Renderer* renderer, ErrorHandler*, std::string, std::string);
    ~Map();

    std::vector<Image*> tiles;

    std::map<char, int> textureIDs;

    std::vector<texture> textures;

    void loadTextures(std::string);
    void loadLayout(std::string);

    void cleanup();

    void update(double seconds);
    void render(Camera* camera);

    int width = 0;
    int height = 0;
  private:
    void createTexture(int, std::string, int);

    ErrorHandler* errorHandler;

    SDL_Renderer* renderer = nullptr;
};

#endif

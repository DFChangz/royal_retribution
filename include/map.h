#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "sprite.h"
#include "error_handler.h"
#include "collision_detector.h"

#define TILE_DIM      32

struct texture {
  SDL_Texture* texture;
  int start_frame = 0;
  int frame_length = 0;
  int options = 0;
  int pairing = -1;
};

struct tile {
  int start_frame = 0;
  int frame_length = 0;

  Sprite* image = nullptr;
};

class CollisionDetector;
class Map {
  public:
    Map(SDL_Renderer* renderer, ErrorHandler*, std::string, std::string,
      CollisionDetector*);
    ~Map();

    std::vector<tile> tiles;
    std::vector<tile> additions;
    std::vector<tile> collidable_tiles;

    std::map<char, int> textureIDs;

    std::map<char, int> textureExtraIDs;

    std::vector<texture> textures;

    void loadTextures(std::string);
    void loadSecondTextures(std::string);
    void loadSecondLayout(std::string);
    void loadLayout(std::string);
    int pushLights(std::map<std::string,Image*>&);

    void cleanup();

    void update(double seconds);
    void render(Camera* camera);

    int width = 0;
    int height = 0;

    std::vector<Image*> lights;
    
  private:
    void createTexture(int, std::string, int, int, int, char);

    ErrorHandler* errorHandler;

    SDL_Renderer* renderer = nullptr;

    CollisionDetector* collisionDetector;

    void addCollidablesToBuckets(std::vector<tile>*);
};

#endif

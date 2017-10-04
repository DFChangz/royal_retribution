#ifndef STATE_H
#define STATE_H

#include <vector>
#include <fstream>
#include <iostream>

#include "map.h"
#include "text.h"
#include "audio.h"
#include "engine.h"
#include "camera.h"
#include "event_handler.h"
#include "collision_detector.h"

class State {
  public:
    State(Engine* engine, ErrorHandler* errorHandler);

    virtual void load();

    virtual void setup() = 0;

    virtual void run(double seconds);

    virtual ~State();

    Engine* engine;
  protected:
    virtual void cleanup();
    virtual void update(double seconds);
    std::vector<Image*> images;
    Audio audioHandler;
    EventHandler eventHandler;
    ErrorHandler* errorHandler;

    Camera camera;
    virtual void render();

    Map *map = nullptr;
  private:
    CollisionDetector* collisionDetector;
};

#endif

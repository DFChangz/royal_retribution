#ifndef STATE_H
#define STATE_H

#include <map>
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

    virtual void run(double* seconds);

    virtual ~State();

    Engine* engine;

    CollisionDetector collisionDetector;

    Map *map = nullptr;

  protected:
    virtual void render();
    virtual void cleanup();
    virtual void update(double seconds);

    double newA = 0;
    double speed = 50;
    int fadeIn(std::string s, int a, double seconds, double mult);

    std::map<std::string,Image*> images;
    std::map<std::string,Image*>::iterator it;

    Audio audioHandler;
    EventHandler eventHandler;
    ErrorHandler* errorHandler;

    Camera camera;


    void pauseUpdate(double);

    std::string ele = std::to_string(ELE);
    std::string ppl = std::to_string(PPL);
    std::string add = std::to_string(ADD);
    std::string top = std::to_string(TOP);

  private:
    bool paused = false;
};

#endif

#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <iostream>
#include <map>
#include <vector>
#include <array>
#include <functional>
#include <SDL.h>

typedef std::function<void()> lambda;
typedef std::vector<std::function<void()>> lambda_vector;
typedef std::map<std::array<int, 2>, lambda_vector*> event_bind;

class EventHandler {
  public:
    void check();
    void addListener(int event, lambda, int key_sym = 0);
    ~EventHandler();
  private:
    event_bind listeners;

    void runTriggers(lambda_vector* triggers);
};

#endif

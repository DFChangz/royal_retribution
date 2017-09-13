#include "event_handler.h"

void EventHandler::check() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    int sym = event.key.keysym.sym;

    for (auto map : listeners) {
      if ((int) event.type == map.first[0] && map.first[1] == sym) {
        runTriggers((map.second));
      }
    }
  }
}

// Add a listener for an event.
// `event` is the event to listen for
// `trigger` is what to run when event occurs
// `key_sym` is the key symbol to check for (defaults to 0 for none)
void EventHandler::addListener(int event, lambda trigger, int key_sym) {
  std::array<int, 2> actions = {{event, key_sym}};

  if (listeners.find(actions) == listeners.end()) {
    listeners[actions] = new lambda_vector;
  }
  listeners[actions]->push_back(trigger);
}

EventHandler::~EventHandler() {
  for (auto map : listeners) {
    delete map.second;
  }
}



void EventHandler::runTriggers(lambda_vector* triggers) {
  for (auto trigger : *triggers) {
    trigger();
  }
}
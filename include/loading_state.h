#ifndef LOAD_STATE_H
#define LOAD_STATE_H

#include "state.h"
#include "pallete.h"

class LoadingState : public State {
    public:
      LoadingState(Engine* engine, ErrorHandler* errorHandler);

      virtual ~LoadingState();
      virtual void load();
      void advance(); 

    private:
      virtual void setup();
      void update(double);

      std::string s;
      int countdown = 6;
};

#endif

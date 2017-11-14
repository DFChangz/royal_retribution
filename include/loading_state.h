#ifndef LOAD_STATE_H
#define LOAD_STATE_H

#include "state.h"
#include "pallete.h"

class LoadingState : public State {
    public:
      LoadingState(Engine* engine, ErrorHandler* errorHandler);

      virtual ~LoadingState();

      virtual void load();

      void loadingStart();
  
      void loadingEnd();

      void loadingMove(double x, double y);
 
      void changeColor(int r, int g, int b);
    private:
      virtual void setup();

      void update(double);
};

#endif

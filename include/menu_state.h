#ifndef MENU_STATE_H
#define MENU_STATE_H

#include "state.h"

class MenuState : public State {
    public:
      MenuState(Engine* engine, ErrorHandler* errorHandler);

      virtual ~MenuState();
    protected:
      void update(double seconds);
    private:
      double timer = 0;
      virtual void setup();
};

#endif

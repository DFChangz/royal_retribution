#ifndef MENU_STATE_H
#define MENU_STATE_H

#include "state.h"
#include "pallete.h"

class MenuState : public State {
    public:
      MenuState(Engine* engine, ErrorHandler* errorHandler);

      virtual ~MenuState();

      virtual void load();
    protected:
      void update(double seconds);
    private:
      virtual void setup();

      std::tuple<int, int> getCenterForImage(Image*);
};

#endif

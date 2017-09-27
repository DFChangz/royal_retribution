#ifndef WIN_STATE_H
#define WIN_STATE_H

#include "state.h"

class WinState : public State {
  public:
    WinState(Engine* engine, ErrorHandler* errorHandler);

    virtual ~WinState();

  private:
    virtual void setup();
    virtual void load();
    virtual void update(double seconds);

    std::tuple<int, int> getCenterForImage(Image*);
    int fadeIn(int i, int a, double seconds, double mult);

    int x, y;
    int a0 = 0;
    double newA = 0;
    double speed = 50;
    double totalTime = 0;
    std::string logo = "Y O U   W I N";
};

#endif

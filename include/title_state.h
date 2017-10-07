#ifndef TITLE_STATE_H
#define TITLE_STATE_H

#include "state.h"

class TitleState : public State {
  public:
    TitleState(Engine* engine, ErrorHandler* errorHandler);

    virtual ~TitleState();

  private:
    virtual void setup();
    virtual void load();
    virtual void update(double seconds);

    std::tuple<int, int> getCenterForImage(Image*);
    int fadeIn(int i, int a, double seconds, double mult);

    int x, y;
    int a0 = 0;
    int a1 = 0;
    int a2 = 0;
    int a3 = 0;
    double newA = 0;
    double speed = 50;
    double totalTime = 0;
    std::string logo = "Team  UBIHARD  presents";
    std::string skip = "press [q] to skip this intro";
};

#endif

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
    void fadeIn(int i, double seconds, double mult);

    int x, y;
    int alpha = 0;
    double newAlpha = 0;
    double totalTime = 0;
    double fade_speed = 50;
    double scroll_speed = -50;
    std::string logo = "Team  UBIHARD  presents";
};

#endif

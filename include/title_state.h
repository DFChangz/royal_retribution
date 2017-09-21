#ifndef TITLE_STATE_H
#define TITLE_STATE_H

#include "state.h"

class TitleState : public State {
  public:
    TitleState(Engine* engine, ErrorHandler* errorHandler);

    virtual ~TitleState();

  /* as reminder
  protected:
    virtual void cleanup();
    std::vector<Image*> images;
    Engine* engine;
    Audio audioHandler;
    EventHandler eventHandler;
    ErrorHandler* errorHandler;
  */

  private:
    virtual void setup();
    virtual void load();
    virtual void update(double seconds);

    void fadeIn(int i, double seconds);
    std::tuple<int, int> getCenterForImage(Image*);

    std::string logo = "Team UBIHARD presents ...";

    int alpha = 0;
    double newAlpha = 0;
    double totalTime = 0;
    double fade_speed = 20;
    double scroll_speed = 10;
};

#endif

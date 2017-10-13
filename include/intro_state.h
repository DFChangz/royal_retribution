#ifndef INTRO_STATE_H
#define INTRO_STATE_H

#include "state.h"

class IntroState : public State {
  public:
    IntroState(Engine* engine, ErrorHandler* errorHandler);

    virtual ~IntroState();

  private:
    virtual void setup();
    virtual void load();
    virtual void update(double seconds);

    int fadeIn(int i, int a, double seconds, double mult);

    int a1 = 0;
    int counter = 2;
    double newA = 0;
    double speed = 50;
    double totalTime = 0;
    double specificTime = 0;

    std::string s1 = "MY MAJESTY! I bear bad news! (next [SPACE])";
    std::string k1 = "Please tell, my trusted aide. (next [SPACE])";
    std::string s2 = "ALIENS! Our castle has fallen! (next [SPACE])";
    std::string k2 = "I see... How many of my men remain? (next [SPACE])";
    std::string s3 = "None, my Majesty... (T~T) (next [SPACE])";
    std::string k3 = "Worry Not! Stand Tall! (next [SPACE])";
    std::string k4 = "Royal retribution shall be dealt! (next [SPACE])";
    std::string s4 = "Our lives are in your hands, my king... (next [SPACE])";
};

#endif

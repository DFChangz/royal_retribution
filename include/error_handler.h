#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

class Engine;
class ErrorHandler {
  public:
    void quit(const char *, const char*);
    ErrorHandler(Engine *engine_ref);
  private:
    Engine *engine;
};
#endif

#include "engine.h"
#include "error_handler.h"

void ErrorHandler::quit(const char *function_name, const char *error_message) {
  printf("%s: %s", function_name, error_message);
  engine->cleanup();
  exit(0);
}

ErrorHandler::ErrorHandler(Engine *engine_ref) {
  engine = engine_ref;
}

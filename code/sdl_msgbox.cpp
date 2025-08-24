#include <SDL.h>

int main(int argc, char *argv[]) { 
  SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
  SDL_Log("Starting application with verbose logging...");
  int Integer;

  Integer = 5;
  Integer = 5 + 2;
  Integer = Integer + 7;
  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Title", "Dismiss me.", 0);
  return 0;
}

#include <SDL.h>
// #include <SDL_init.h>
// #include <SDL_messagebox.h>

int main(int argc, char *argv[])
{
  bool sdlInitialised = SDL_Init(SDL_INIT_VIDEO);
  if (!sdlInitialised) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Handmade Hero", "This is Handmade Hero", 0);
  }
  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Handmade Hero", "This is Handmade Hero", 0);
  
  SDL_Quit();
	return(0);
}

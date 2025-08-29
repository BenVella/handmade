#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
//#include <SDL3/SDL.h>


int main()
{
  SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Rect positionwindow = {50, 50, 400, 400};
	SDL_Rect viewport = {100, 100, 400, 400};
	
	SDL_Rect square = {-100, -100, 400, 400};
	//SDL_FRect square = {-100, -100, 400, 400};
	
	window = SDL_CreateWindow("le bug", positionwindow.x, positionwindow.y, positionwindow.w, positionwindow.h, 0);
	//window = SDL_CreateWindow("le bug", positionwindow.w, positionwindow.h, 0);
	
	renderer = SDL_CreateRenderer(window, -1, 0);
	//renderer = SDL_CreateRenderer(window, "opengl");
	
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 400, 400);
	if(texture == NULL) printf("%s\n", SDL_GetError());
	
	SDL_SetRenderTarget(renderer, texture);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderTarget(renderer, NULL);
	
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	
	SDL_RenderSetViewport(renderer, NULL);
	//SDL_SetRenderViewport(renderer, &viewport);
	
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_RenderClear(renderer);
	
	SDL_RenderCopy(renderer, texture, NULL, &square);
	//SDL_RenderTexture(renderer, texture, NULL, &square);
	
	SDL_RenderSetViewport(renderer, NULL);
	//SDL_SetRenderViewport(renderer, NULL);
	
	SDL_RenderPresent(renderer);
	SDL_Delay(3000);
	
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	return 0;
}


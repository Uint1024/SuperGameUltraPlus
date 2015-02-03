#include <iostream>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "engine.h"
#include "globals.h"

//Does nothing, don't forget to initialize !
Engine::Engine()
{
  
}

void
Engine::initialize(const char* window_name, const Veci window_size) {
  std::cout << "Starting Engine" << std::endl;
  
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cout << SDL_GetError() << std::endl;
  }

  if(IMG_Init(IMG_INIT_PNG) < 0) {
    std::cout << SDL_GetError() << std::endl;
  }
  
  if(TTF_Init() < 0) {
    std::cout << SDL_GetError() << std::endl;
  }
  
	window = SDL_CreateWindow(window_name,
											SDL_WINDOWPOS_UNDEFINED,
											SDL_WINDOWPOS_UNDEFINED,
											window_size.x,
											window_size.y,
											SDL_WINDOW_SHOWN);
  
  if(window == NULL) {
    std::cout << SDL_GetError() << std::endl;
  }        

  pixel_format = SDL_GetWindowPixelFormat(window);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if(renderer == NULL) {
      std::cout << SDL_GetError() << std::endl;
  }  
  
  character_sprite_sheet = IMG_LoadTexture(renderer, "sprites/characters.png");
  if(!character_sprite_sheet){
    std::cout << SDL_GetError() << std::endl;
  }
  
  std::vector<SDL_Rect> src_rect_player = { {0,0,128,128},
                                            {0,128,128,128},
                                            {0,256,128,128} };
  sprites_data[kTexture_Player] = 
          SpriteData(character_sprite_sheet, 3, src_rect_player);
          
    
}

void 
Engine::render() {
  SDL_SetRenderDrawColor(renderer, 210, 210, 210, 255);
  SDL_RenderClear(renderer);
  
  
  g_game_data.player.render();
  SDL_RenderPresent(renderer);
}

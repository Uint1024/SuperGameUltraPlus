#include <iostream>
#include <string>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "engine.h"
#include "gamedata.h"

//Does nothing, don't forget to initialize !
Engine::Engine() :
camera{0.0f,0.0f}
{
  
}

void
Engine::Initialize(std::string window_name, const Veci window_size) {
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
  
	window = SDL_CreateWindow(window_name.c_str(),
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
  
  logic_sprite_sheet = IMG_LoadTexture(renderer, "sprites/logic.png");
  if(!logic_sprite_sheet){
    std::cout << SDL_GetError() << std::endl;
  }
  
  std::vector<SDL_Rect> src_rect = {{0,0,64,64},
                                    {0,64,64,64},
                                    {0,128,64,64}};
  sprites_data[kTexture_Player] = 
          SpriteData(character_sprite_sheet, 3, src_rect, 100);
          
  src_rect = {{48,0,16,16}};
  sprites_data[kTexture_Wire_Empty] = 
          SpriteData(logic_sprite_sheet, 0, src_rect, 0);  
  
  src_rect = {{48,16,16,16}};
  sprites_data[kTexture_Wire_0] = 
          SpriteData(logic_sprite_sheet, 0, src_rect, 0); 
  
  src_rect = {{48,32,16,16}};
  sprites_data[kTexture_Wire_1] = 
          SpriteData(logic_sprite_sheet, 0, src_rect, 0); 
  
  src_rect = {{0,0,48,48}};
  sprites_data[kTexture_And] = 
          SpriteData(logic_sprite_sheet, 0, src_rect, 0);  
  
  src_rect = {{0,48,48,48}};
  sprites_data[kTexture_Not] = 
          SpriteData(logic_sprite_sheet, 0, src_rect, 0);  

  
  src_rect = {{64,0,48,48}};
  sprites_data[kTexture_Constant_1] = 
          SpriteData(logic_sprite_sheet, 0, src_rect, 0);
}

void 
Engine::Render(GameData& game_data) {
  SDL_SetRenderDrawColor(renderer, 210, 210, 210, 255);
  SDL_RenderClear(renderer);
  
  
  game_data.Render(*this);
  SDL_RenderPresent(renderer);
}

void Engine::MoveCamera(const Vecf& movement) {
  AddVecf(movement, &camera);
}

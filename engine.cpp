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
  
  font = TTF_OpenFont("LiberationSans-Regular.ttf", 22);
  if (font == NULL)
  {
    std::cout << TTF_GetError() << std::endl;
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
          
  src_rect = {{48,0,22,22}};
  sprites_data[kTexture_Wire_Empty] = 
          SpriteData(logic_sprite_sheet, 0, src_rect, 0);  
  
  src_rect = {{48,22,22,22}};
  sprites_data[kTexture_Wire_1] = 
          SpriteData(logic_sprite_sheet, 0, src_rect, 0); 
  
  src_rect = {{48,44,22,22}};
  sprites_data[kTexture_Wire_0] = 
          SpriteData(logic_sprite_sheet, 0, src_rect, 0); 
  
  src_rect = {{48,66,22,22}};
  sprites_data[kTexture_Wire_Error] = 
          SpriteData(logic_sprite_sheet, 0, src_rect, 0); 
  
   src_rect = {{160,32,32,32}};
  sprites_data[kTexture_ThreeState_Empty] = 
          SpriteData(logic_sprite_sheet, 0, src_rect, 0); 
  
   src_rect = {{160,64,32,32}};
  sprites_data[kTexture_ThreeState_0] = 
          SpriteData(logic_sprite_sheet, 0, src_rect, 0); 
  
   src_rect = {{160,96,32,32}};
  sprites_data[kTexture_ThreeState_1] = 
          SpriteData(logic_sprite_sheet, 0, src_rect, 0); 
  
  
  src_rect = {{128,0,32,32}};
  sprites_data[kTexture_And] = 
          SpriteData(logic_sprite_sheet, 0, src_rect, 0);  
  
  src_rect = {{128,32,32,32}};
  sprites_data[kTexture_Not] = 
          SpriteData(logic_sprite_sheet, 0, src_rect, 0); 
  
  src_rect = {{128,64,32,32}};
  sprites_data[kTexture_Separator] = 
          SpriteData(logic_sprite_sheet, 0, src_rect, 0); 
    
  src_rect = {{128,96,32,32}};
  sprites_data[kTexture_And_1] = 
          SpriteData(logic_sprite_sheet, 0, src_rect, 0);  
  
  src_rect = {{128,128,32,32}};
  sprites_data[kTexture_Not_1] = 
          SpriteData(logic_sprite_sheet, 0, src_rect, 0); 
  
  src_rect = {{128,160,32,32}};
  sprites_data[kTexture_Separator_1] = 
          SpriteData(logic_sprite_sheet, 0, src_rect, 0); 
    
  src_rect = {{128,192,32,32}};
  sprites_data[kTexture_And_0] = 
          SpriteData(logic_sprite_sheet, 0, src_rect, 0);  
  
  src_rect = {{128,224,32,32}};
  sprites_data[kTexture_Not_0] = 
          SpriteData(logic_sprite_sheet, 0, src_rect, 0); 
  
  src_rect = {{128,256,32,32}};
  sprites_data[kTexture_Separator_0] = 
          SpriteData(logic_sprite_sheet, 0, src_rect, 0); 
  
  src_rect = {{96,64,32,32}};
  sprites_data[kTexture_Constant_1] = 
          SpriteData(logic_sprite_sheet, 0, src_rect, 0);
  
  src_rect = {{96,96,32,32}};
  sprites_data[kTexture_Constant_0] = 
          SpriteData(logic_sprite_sheet, 0, src_rect, 0);
  
  src_rect = {{48,88,22,22}};
    sprites_data[kTexture_Wire_Underground] = 
            SpriteData(logic_sprite_sheet, 0, src_rect, 0);
    
  src_rect = {{4,88,22,22}};
    sprites_data[kTexture_Wire_Underground_0] = 
            SpriteData(logic_sprite_sheet, 0, src_rect, 0);
    
  src_rect = {{26,88,22,22}};
  sprites_data[kTexture_Wire_Underground_1] = 
          SpriteData(logic_sprite_sheet, 0, src_rect, 0);

  src_rect = {{48,100,22,22}};
  sprites_data[kTexture_Wire_Underground_Exit] = 
          SpriteData(logic_sprite_sheet, 0, src_rect, 0);

  src_rect = {{48,132,22,22}};
  sprites_data[kTexture_Wire_Underground_SemiInvisible] = 
          SpriteData(logic_sprite_sheet, 0, src_rect, 0);
  
   src_rect = {{4,0,22,22}};
    sprites_data[kTexture_Clock] = 
            SpriteData(logic_sprite_sheet, 0, src_rect, 0);
    
  src_rect = {{4,22,22,22}};
    sprites_data[kTexture_Clock_0] = 
            SpriteData(logic_sprite_sheet, 0, src_rect, 0);
    
  src_rect = {{4,44,22,22}};
  sprites_data[kTexture_Clock_1] = 
          SpriteData(logic_sprite_sheet, 0, src_rect, 0);
  
}

void 
Engine::Render(GameData& game_data) {
  SDL_SetRenderDrawColor(renderer, 225, 225, 225, 255);
  SDL_RenderClear(renderer);
  
  
  game_data.Render(*this);
  SDL_RenderPresent(renderer);
}

void Engine::MoveCamera(const Vecf& movement) {
  AddVecf(movement, &camera);
}

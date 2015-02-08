/* 
 * File:   engine.h
 * Author: John
 *
 * Created on 2 février 2015, 19:25
 */

#ifndef ENGINE_H
#define	ENGINE_H
#include <array>
#include <string>
#include <vector>
#include <tuple>
#include "SDL_ttf.h"
#include "SDL.h"
#include "utils.h"
#include "sprite_data.h"

class GameData;
struct Engine {
  Engine();
  void Initialize(std::string window_name, const Veci window_size);
  void Render(GameData& game_data);
  
  void MoveCamera(const Vecf& movement);
  
  SDL_Window* window;
  SDL_Renderer* renderer;
  Uint32 pixel_format; //pixel format of the window, used with SDL_RenderCopy
  SDL_Texture* character_sprite_sheet;
  SDL_Texture* logic_sprite_sheet;
  
  //access using the eTexture enum
  std::array<SpriteData, kTexture_Count> sprites_data;
   Vecf camera;
   TTF_Font* font;
   
};

#endif	/* ENGINE_H */


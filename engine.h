/* 
 * File:   engine.h
 * Author: John
 *
 * Created on 2 février 2015, 19:25
 */

#ifndef ENGINE_H
#define	ENGINE_H
#include <array>
#include <vector>
#include <tuple>
#include "SDL.h"
#include "utils.h"
#include "sprite_data.h"

class GameData;
struct Engine {
  Engine();
  void Initialize(const char* window_name, const Veci window_size);
  void Render(GameData& game_data);
  SDL_Window* window;
  SDL_Renderer* renderer;
  Uint32 pixel_format; //pixel format of the window, used with SDL_RenderCopy
  SDL_Texture* character_sprite_sheet;
  
  //access using the eTexture enum
  std::array<SpriteData, kTexture_Count> sprites_data;
};

#endif	/* ENGINE_H */


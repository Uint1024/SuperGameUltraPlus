#include <iostream>
#include "SDL.h"
#include "SDL_ttf.h"
#include "save_gate_window.h"
#include "solid_body.h"
#include "engine.h"
#include "static_body.h"

SaveGateWindow::SaveGateWindow(const Vecf& position) :
body(new StaticBody(position, Veci{300, 100}, (eTexture)0, (eDirection)0)),
gate_name(){
  font = TTF_OpenFont("LiberationSans-Regular.ttf", 22);
  if (font == NULL)
  {
    std::cout << TTF_GetError() << std::endl;
  }
}

void
SaveGateWindow::Render(Engine& engine) {
  
  SDL_SetRenderDrawColor(engine.renderer, 200, 200, 1, 255);
  SDL_Rect dest_rect = {30, 30, 400, 200};
  SDL_RenderFillRect(engine.renderer, &dest_rect);
  
  SDL_Color black = {0, 0, 0};
  SDL_Surface* surface = TTF_RenderText_Solid(font, gate_name, black);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(engine.renderer, surface);
  int w, h;
  TTF_SizeText(font, gate_name, &h, &w);
  SDL_Rect rect = {30, 50, h, w};
  
  SDL_RenderCopy(engine.renderer, texture, NULL, &rect);
  
}

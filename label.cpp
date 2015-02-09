#include <iostream>
#include "label.h"

Label::Label() :
bbox(Vecf{0,0}, Veci{0,0}), text(""), texture(nullptr){
  
}
Label::Label(const Vecf& position, 
        const std::string text) :
bbox(position, Veci{0,0}), text(text), texture(nullptr){

}

Label::~Label(){
  if(texture)
    SDL_DestroyTexture(texture);
}

void Label::Render(Engine& engine) {

    SDL_Color black = {0, 0, 0};
    SDL_Surface* surface = TTF_RenderText_Solid(engine.font, text.c_str(), black);

    texture = SDL_CreateTextureFromSurface(engine.renderer, surface);
    int w, h;
    TTF_SizeText(engine.font, text.c_str(), &h, &w);

    bbox.Resize(Veci{h, w});

    SDL_Rect rect = {static_cast<int>(bbox.left - engine.camera.x), 

    static_cast<int>(bbox.top - engine.camera.y), 
    bbox.size.x, bbox.size.y};

    SDL_RenderCopy(engine.renderer, texture, NULL, &rect);
    
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
  
}


/* 
 * File:   label.h
 * Author: John
 *
 * Created on 8 février 2015, 03:12
 */

#ifndef LABEL_H
#define	LABEL_H
#include "bbox.h"
#include "engine.h"
#include "SDL_ttf.h"
#include "SDL.h"
#include <string>
struct Label {
  Label(const Vecf& position, const std::string text);
  ~Label();
  Bbox bbox;
  std::string text;
  void Render(Engine &engine);
  SDL_Texture* texture;
};

#endif	/* LABEL_H */


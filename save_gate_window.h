/* 
 * File:   save_gate_window.h
 * Author: John
 *
 * Created on 5 février 2015, 23:05
 */

#ifndef SAVE_GATE_WINDOW_H
#define	SAVE_GATE_WINDOW_H
#include "SDL.h"
#include "SDL_ttf.h"
#include <string>
#include "utils.h"

class Engine;
class SolidBody;
struct SaveGateWindow {
  SaveGateWindow(const Vecf& position);
  void Render(Engine& engine);
  SolidBody* body;
  char gate_name[100];
  TTF_Font* font;
};

#endif	/* SAVE_GATE_WINDOW_H */


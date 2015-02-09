/* 
 * File:   main.cpp
 * Author: John
 *
 * Created on 2 février 2015, 03:59
 * 
 * //todo
 * AND
 * NOT
 * Wire
 * Rotate
 * Select
 * Copy
 * Paste
 * Toolbox menu when you can save and load more complex structures
 * Toolbar to quickly access structures from the toolbox
 *
 */

#include <string>
#include <iostream>
#include <SDL.h>
#include "utils.h"
#include "gamedata.h"
#include "engine.h"
#include "input.h"

using namespace std;

float g_delta_t = 0;

high_resolution_clock_time_point (*TimeNow)() = 
        &std::chrono::high_resolution_clock::now;

int main(int argc, char** argv) {
  std::string window_name = "Electron Simulator";
  Veci window_size{1366, 768};
  Engine engine;
  engine.Initialize(window_name, window_size);
  GameData game_data(window_size);
  
  Input input;

  /*Used to calculate delta_t*/
  /*auto current =  TimeNow();
  auto last =  TimeNow();
  auto diff = current - last;
  */
  
  Uint32 last = SDL_GetTicks();
  
  float fps = 0;
  
  bool running = true;
  
  Uint32 start_time = SDL_GetTicks();
  Uint32 ms_since_last_frame;
  
  //limit to 120 fps (= 1 frame every 15 ms)
  while (running) {

    Uint32 diff = SDL_GetTicks() - last;
    ms_since_last_frame += diff;
    if (ms_since_last_frame >= 5) { 
      g_delta_t = ms_since_last_frame / 1000.0f;
      running = input.PollEvents(game_data, engine);
      game_data.Update();
      engine.Render(game_data);
      fps = 1000 / g_delta_t;
      ms_since_last_frame = 0;
     
    } 
    
    last = SDL_GetTicks();
    
  }
  return 0;
}


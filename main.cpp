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
 * Use only structs, no inheritance, no member functions, no private variables
 * no overloading, no exceptions
 * no smart pointers
 *
 */

#include <SDL.h>
#include <iostream>
#include "utils.h"
#include "gamedata.h"
#include "engine.h"
#include "input.h"

using namespace std;

int g_delta_t = 0;

high_resolution_clock_time_point (*TimeNow)() = 
        &std::chrono::high_resolution_clock::now;

int main(int argc, char** argv) {
  GameData game_data;
  Engine engine;
  Input input;

  /*Used to calculate delta_t*/
  auto current =  TimeNow();
  auto last =  TimeNow();
  auto diff = current - last;
  
  
  int fps = 0;
  
  bool running = true;
  
  engine.Initialize("Electron Simulator", Veci{1280, 720});
  
  //limit to 120 fps (= 1 frame every 15 ms)
  while (running) {
    current = TimeNow();
    diff = current - last;
    g_delta_t += std::chrono::duration_cast<milliseconds>(diff).count();
    
    if (g_delta_t >= 8) {  
      running = input.PollEvents(game_data.player);
      engine.Render(game_data);
      
      g_delta_t = 0;
    } 
    
    last = current;
    
  }
  return 0;
}


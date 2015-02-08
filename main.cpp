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

int g_delta_t = 0;

high_resolution_clock_time_point (*TimeNow)() = 
        &std::chrono::high_resolution_clock::now;

int main(int argc, char** argv) {
  std::string window_name = "Electron Simulator";
  Veci window_size{1280, 720};
  Engine engine;
  engine.Initialize(window_name, window_size);
  GameData game_data(window_size);
  
  Input input;

  /*Used to calculate delta_t*/
  auto current =  TimeNow();
  auto last =  TimeNow();
  auto diff = current - last;
  
  
  int fps = 0;
  
  bool running = true;
  
  
  
  //limit to 120 fps (= 1 frame every 15 ms)
  while (running) {
    current = TimeNow();
    diff = current - last;
    g_delta_t += std::chrono::duration_cast<milliseconds>(diff).count();
    
    if (g_delta_t >= 2) {  
      running = input.PollEvents(game_data, engine);
      game_data.Update();
      engine.Render(game_data);
      g_delta_t = 0;
    } 
    
    last = current;
    
  }
  return 0;
}


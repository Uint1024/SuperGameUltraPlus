#include <iostream>
#include "gamedata.h"
#include "engine.h"
#include "wire.h"

GameData::GameData(const Veci& window_size) :
player(Vecf{window_size.x/2.0f, window_size.y/2.0f}),
map_size{100,100}{
  wire_map.reserve(map_size.x * map_size.y);
  for(int i = 0 ; i < map_size.x * map_size.y ; ++i){
    wire_map.push_back(nullptr);
  }
  wire_map[10] = new Wire(Vecf{100,100});
  
}

void
GameData::InitializeWorld(const Veci& window_size) {
  //player = Player(Vecf{window_size.x/2.0f, window_size.y/2.0f});
}

void 
GameData::Render(Engine& engine) {
  for(auto wire : wire_map){
    
    if(wire){
      wire->body.Render(engine);
    }
  }
  player.body.Render(engine);
}

Vecf 
GameData::ReceiveInput( const std::array<bool, kKey_Count>& keys_down, 
                        const std::array<bool, 255 > & mouse_buttons_down, 
                        const Veci& mouse_position_in_window,
                        const Vecf& mouse_position_in_world) {

  //position of the mouse on the map grid, so {5, 10} is the 5th column, 10th row
  Veci mouse_grid_position = {static_cast<int>(mouse_position_in_world.x)/16,
                              static_cast<int>(mouse_position_in_world.y)/16};
  
  Vecf grid_position_position = { static_cast<float>(mouse_grid_position.x) * 16,  
                                  static_cast<float>(mouse_grid_position.y) * 16 };
  
  std::cout << mouse_position_in_world.x << std::endl;
  if(mouse_buttons_down[SDL_BUTTON_LEFT]){
    int position_in_vector = 
                  mouse_grid_position.x + mouse_grid_position.y * map_size.x;
    
    wire_map[position_in_vector] = new Wire(grid_position_position);
  }
  
  Vecf player_movement = player.ReceiveInput(keys_down, mouse_buttons_down);
  
  return player_movement;
}

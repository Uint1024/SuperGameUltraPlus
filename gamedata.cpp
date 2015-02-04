#include <iostream>
#include "gamedata.h"
#include "engine.h"
#include "wire.h"


GameData::GameData(const Veci& window_size) :
player(Vecf{window_size.x/2.0f, window_size.y/2.0f}),
map_size{100,100},
currently_selected_object(kEditorObject_None),
temporary_gate(nullptr),
temporary_wire(nullptr),
temporary_rotation(kDirection_Down){
  wire_map.reserve(map_size.x * map_size.y);
  for(int i = 0 ; i < map_size.x * map_size.y ; ++i){
    wire_map.push_back(nullptr);
  }
 
  logic_gate_map.push_back(nullptr);
}

void
GameData::InitializeWorld(const Veci& window_size) {
  //player = Player(Vecf{window_size.x/2.0f, window_size.y/2.0f});
}

void 
GameData::Render(Engine& engine) {
  for(auto &wire : wire_map){
    
    if(wire){
      wire->body->Render(engine);
    }
  }
  
  for(auto &logic_gate : logic_gate_map){
    if(logic_gate){
      logic_gate->body->Render(engine);
    }
  }
  
  for(auto &logic_gate : battery_map){
    if(logic_gate){
      logic_gate->body->Render(engine);
    }
  }
  
  player.body->Render(engine);
  
  if(temporary_gate){
    temporary_gate->body->Render(engine);
    
  }
  if(temporary_wire){
    temporary_wire->body->Render(engine);
  }
}

Vecf 
GameData::ReceiveInput( const std::array<bool, kKey_Count>& keys_down, 
                        const std::array<bool, 255 > & mouse_buttons_down, 
                        const Veci& mouse_position_in_window,
                        const Vecf& mouse_position_in_world) {

  //position of the mouse on the map grid, so {5, 10} is the 5th column, 10th row
  mouse_grid_position = {static_cast<int>(mouse_position_in_world.x)/16,
                         static_cast<int>(mouse_position_in_world.y)/16};
  
  grid_position_position = { static_cast<float>(mouse_grid_position.x) * 16,  
                             static_cast<float>(mouse_grid_position.y) * 16 };
  
  int position_in_vector = 
                  mouse_grid_position.x + mouse_grid_position.y * map_size.x;

  
  
  bool pressed_rotation_key = false;
  if(keys_down[kKey_Rotate_Left]){
    temporary_rotation = (eDirection)(temporary_rotation - 1);
    if(temporary_rotation < 0){
      temporary_rotation = (eDirection)(kDirection_Count - 1);
    }
    pressed_rotation_key = true;
    std::cout << temporary_rotation << std::endl;
  }
  if(keys_down[kKey_Rotate_Right]){
    temporary_rotation = (eDirection)(temporary_rotation + 1);
    if(temporary_rotation >= kDirection_Count){
      temporary_rotation = (eDirection)0;
    }
  }
  
  if(keys_down[kKey_Constant_1]){
    currently_selected_object = kEditorObject_Constant_1;
    Clean();
    temporary_gate = new LogicGate(grid_position_position, 
            kEditorObject_Constant_1,
            kDirection_Down);
    pressed_rotation_key = true;
  }
  
  if(pressed_rotation_key){
    if(temporary_wire){
      Clean();
      temporary_wire = new Wire(grid_position_position, 0, temporary_rotation);
    }
    
  }
  
  if(keys_down[kKey_And]){
    Clean();
    currently_selected_object = kEditorObject_And;
  }
  if(keys_down[kKey_Or]){
    Clean();
    currently_selected_object = kEditorObject_Or;
  }
   if(keys_down[kKey_Wire]){
    currently_selected_object = kEditorObject_Wire;
    Clean();
    temporary_wire = new Wire(grid_position_position, 0, temporary_rotation);
  }
  
  if(temporary_wire){
    temporary_wire->body->bbox.MoveTo(grid_position_position);
  }
  if(temporary_gate){
    temporary_gate->body->bbox.MoveTo(grid_position_position);
  }
  
  if(currently_selected_object == kEditorObject_Constant_1 || 
                  currently_selected_object == kEditorObject_And ||
                  currently_selected_object == kEditorObject_Or){
    if(mouse_buttons_down[SDL_BUTTON_LEFT]){
        battery_map.push_back(new LogicGate(grid_position_position,
                  currently_selected_object, kDirection_Down));
    }
  } else if(currently_selected_object == kEditorObject_Wire) {
    if(mouse_buttons_down[SDL_BUTTON_LEFT]){
      delete wire_map[position_in_vector];
      wire_map[position_in_vector] = nullptr;
      wire_map[position_in_vector] = new Wire(grid_position_position,
                position_in_vector, temporary_rotation);
      
    }
  }
  if(mouse_buttons_down[SDL_BUTTON_RIGHT]){ 
    for(int i = 0 ; i < wire_map.size() ; i++){
      if(wire_map[i]){
        
        if(wire_map[i]->body->bbox.CollisionWithPoint(mouse_position_in_world)){
          std::cout << "true" << i << std::endl;
          delete wire_map[i];
          wire_map[i] = nullptr;
        }
      }
    }
   
  }
  Vecf player_movement = player.ReceiveInput(keys_down, mouse_buttons_down);
  
  return player_movement;
}

void 
GameData::Update() {
  for(auto &logic_gate : battery_map){
    if(logic_gate){
      logic_gate->CheckOutputToWires(wire_map, map_size);
    }
  }
}

void 
GameData::ResetWiresVisitedState() {
  for(auto &wire : wire_map){
    if(wire){
      wire->ResetState();
    }
  }
}


void
GameData::Clean(){
    delete temporary_gate;
    temporary_gate = nullptr;
    delete temporary_wire;
    temporary_wire = nullptr;
}
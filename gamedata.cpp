#include <iostream>
#include "gamedata.h"
#include "engine.h"
#include "wire.h"
#include "and_gate.h"
#include "constant_1.h"
#include "not_gate.h"
#include "separator.h"
#include <memory>

GameData::GameData(const Veci& window_size) :
player(Vecf{window_size.x/2.0f, window_size.y/2.0f}),
map_size{100,100},
currently_selected_object(kEditorObject_None),
temporary_gate(nullptr),
temporary_wire(nullptr)
,selecting_area(false)
,temporary_rotation(kDirection_Down){
  wire_map.reserve(map_size.x * map_size.y);
  for(int i = 0 ; i < map_size.x * map_size.y ; ++i){
    wire_map.push_back(nullptr);
  }
  
  temporary_wire_map_blueprints.reserve(map_size.x * map_size.y);
  for(int i = 0 ; i < map_size.x * map_size.y ; ++i){
    temporary_wire_map_blueprints.push_back(nullptr);
  }
  
  /*energy_map.reserve(map_size.x * map_size.y);
  for(int i = 0 ; i < map_size.x * map_size.y ; ++i){
    //energy_map.push_back(nullptr);
    energy_map[i][0] = nullptr;
    energy_map[i][1] = nullptr;
    energy_map[i][2] = nullptr;
    energy_map[i][3] = nullptr;
  }*/
  for(int i = 0 ; i < map_size.x * map_size.y ; ++i){
    energy_map.push_back(std::array<Energy*, 4>{nullptr});
  }
  
  std::cout << energy_map[301][1] << std::endl;
 
}

void
GameData::InitializeWorld(const Veci& window_size) {
  //player = Player(Vecf{window_size.x/2.0f, window_size.y/2.0f});
}

void 
GameData::Render(Engine& engine) {
  for(auto &logic_gate : logic_gate_map){
    if(logic_gate){
      logic_gate->body->Render(engine);
    }
  }
  
  for(auto &wire : wire_map){  
    if(wire){
      wire->body->Render(engine);
    }
  }
  
  for(auto &wire : temporary_wire_map_blueprints){  
    if(wire){
      wire->body->Render(engine);
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

void GameData::CreateTemporaryObject(const eEditorObject object_type, const Vecf& position) {
  Clean();
  switch(object_type){
    case kEditorObject_And:
      temporary_gate = new AndGate(position, temporary_rotation, 0, map_size);
      break;
    case kEditorObject_Not:
      temporary_gate = new NotGate(position, temporary_rotation, 0, map_size);
      break;
    case kEditorObject_Constant_1:
      temporary_gate = new Constant_1(position, temporary_rotation, 0, map_size);
      break;
    case kEditorObject_Separator:
      temporary_gate = new Separator(position, temporary_rotation, 0, map_size);
      break;
    case kEditorObject_Wire:
      temporary_wire = new Wire(position, 0, temporary_rotation);
      break;
    default:
      std::cout << "Unknown object." << std::endl;
      break;
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
  
  if(!keeping_mouse_pressed &&
          mouse_buttons_down[SDL_BUTTON_LEFT] && 
          mouse_button_pressed_last_frame){
    mouse_selection_grid_position_begin = mouse_grid_position;
    keeping_mouse_pressed = true;
  }
  
  if(keeping_mouse_pressed && 
          (mouse_selection_grid_position_begin.x != mouse_grid_position.x ||
          mouse_selection_grid_position_begin.y != mouse_grid_position.y)) {
    selecting_area = true;
  }
  
  if(keeping_mouse_pressed && selecting_area &&
          currently_selected_object == kEditorObject_Wire){
    for(auto &temp_wire : temporary_wire_map_blueprints){
      delete temp_wire;
      temp_wire = nullptr;
    }
    
    delete temporary_wire;
    temporary_wire = nullptr;
    
    //calculating distance from origin of selection to know
    //direction of slide
    int diff_x = mouse_grid_position.x - mouse_selection_grid_position_begin.x;
    int diff_y = mouse_grid_position.y - mouse_selection_grid_position_begin.y;
    //std::cout << diff_x << " " << diff_y << std::endl;
    if(diff_x >= 0 && diff_y >= 0){
      if(diff_x > diff_y){
        temporary_rotation = kDirection_Right;
        for(int i = mouse_selection_grid_position_begin.x ;
                i <= mouse_grid_position.x ;
                ++i){
          int position_in_vector = 
                  i + mouse_selection_grid_position_begin.y * map_size.x;
          Vecf real_position = { static_cast<float>(i) * 16,  
                             static_cast<float>(mouse_selection_grid_position_begin.y) * 16 };
          temporary_wire_map_blueprints[position_in_vector] = new Wire(real_position,
                  position_in_vector, temporary_rotation);
        }
      }
      else{
        temporary_rotation = kDirection_Down;
        for(int i = mouse_selection_grid_position_begin.y ;
                i <= mouse_grid_position.y ;
                ++i){
          int position_in_vector = 
                  mouse_selection_grid_position_begin.x + i * map_size.x;
          Vecf real_position = { static_cast<float>(mouse_selection_grid_position_begin.x) * 16,  
                             static_cast<float>(i) * 16 };
          temporary_wire_map_blueprints[position_in_vector] = new Wire(real_position,
                  position_in_vector, temporary_rotation);
        }
      }
    }
    if(diff_x <= 0 && diff_y >= 0){
      if(abs(diff_x) > diff_y){
        temporary_rotation = kDirection_Left;
        for(int i = mouse_selection_grid_position_begin.x ;
                i >= mouse_grid_position.x ;
                --i){
          int position_in_vector = 
                  i + mouse_selection_grid_position_begin.y * map_size.x;
          Vecf real_position = { static_cast<float>(i) * 16,  
                             static_cast<float>(mouse_selection_grid_position_begin.y) * 16 };
          temporary_wire_map_blueprints[position_in_vector] = new Wire(real_position,
                  position_in_vector, temporary_rotation);
        }
      }
      else{
        temporary_rotation = kDirection_Down;
        for(int i = mouse_selection_grid_position_begin.y ;
                i <= mouse_grid_position.y ;
                ++i){
          int position_in_vector = 
                  mouse_selection_grid_position_begin.x + i * map_size.x;
          Vecf real_position = { static_cast<float>(mouse_selection_grid_position_begin.x) * 16,  
                             static_cast<float>(i) * 16 };
          temporary_wire_map_blueprints[position_in_vector] = new Wire(real_position,
                  position_in_vector, temporary_rotation);
        }
      }
    }
    if(diff_x <= 0 && diff_y <= 0){
      if(abs(diff_x) > abs(diff_y)){
        temporary_rotation = kDirection_Left;
        for(int i = mouse_selection_grid_position_begin.x ;
                i >= mouse_grid_position.x ;
                --i){
          int position_in_vector = 
                  i + mouse_selection_grid_position_begin.y * map_size.x;
          Vecf real_position = { static_cast<float>(i) * 16,  
                             static_cast<float>(mouse_selection_grid_position_begin.y) * 16 };
          temporary_wire_map_blueprints[position_in_vector] = new Wire(real_position,
                  position_in_vector, temporary_rotation);
        }
      }
      else{
        temporary_rotation = kDirection_Up;
        for(int i = mouse_selection_grid_position_begin.y ;
                i >= mouse_grid_position.y ;
                --i){
          int position_in_vector = 
                  mouse_selection_grid_position_begin.x + i * map_size.x;
          Vecf real_position = { static_cast<float>(mouse_selection_grid_position_begin.x) * 16,  
                             static_cast<float>(i) * 16 };
          temporary_wire_map_blueprints[position_in_vector] = new Wire(real_position,
                  position_in_vector, temporary_rotation);
        }
      }
    }
    if(diff_x >= 0 && diff_y <= 0){
      if(diff_x > abs(diff_y)){
        temporary_rotation = kDirection_Right;
        for(int i = mouse_selection_grid_position_begin.x ;
                i <= mouse_grid_position.x ;
                ++i){
          int position_in_vector = 
                  i + mouse_selection_grid_position_begin.y * map_size.x;
          Vecf real_position = { static_cast<float>(i) * 16,  
                             static_cast<float>(mouse_selection_grid_position_begin.y) * 16 };
          temporary_wire_map_blueprints[position_in_vector] = new Wire(real_position,
                  position_in_vector, temporary_rotation);
        }
      }
      else{
        temporary_rotation = kDirection_Up;
        for(int i = mouse_selection_grid_position_begin.y ;
                i >= mouse_grid_position.y ;
                --i){
          int position_in_vector = 
                  mouse_selection_grid_position_begin.x + i * map_size.x;
          Vecf real_position = { static_cast<float>(mouse_selection_grid_position_begin.x) * 16,  
                             static_cast<float>(i) * 16 };
          temporary_wire_map_blueprints[position_in_vector] = new Wire(real_position,
                  position_in_vector, temporary_rotation);
        }
      }
    }
    

    
    if(temporary_wire){
      Clean();
      temporary_wire = new Wire(grid_position_position, 0, temporary_rotation);
    }
    
    
  }
  
  if(!selecting_area && currently_selected_object == kEditorObject_Wire 
          && !temporary_wire){
     temporary_wire = new Wire(grid_position_position, 0, temporary_rotation);
  }
  
  bool pressed_rotation_key = false;
  if(keys_down[kKey_Rotate_Left]){
    temporary_rotation = (eDirection)(temporary_rotation - 1);
    if(temporary_rotation < 0){
      temporary_rotation = (eDirection)(kDirection_Count - 1);
    }
    pressed_rotation_key = true;
  }
  if(keys_down[kKey_Rotate_Right]){
    temporary_rotation = (eDirection)(temporary_rotation + 1);
    if(temporary_rotation >= (int)kDirection_Count){
      temporary_rotation = (eDirection)0;
    }
    pressed_rotation_key = true;
  }
  
  if(pressed_rotation_key){
    if(temporary_wire){
      Clean();
      temporary_wire = new Wire(grid_position_position, 0, temporary_rotation);
    }
    if(temporary_gate){
      Clean();
      if(currently_selected_object == kEditorObject_Separator){
        temporary_gate = new Separator(grid_position_position, temporary_rotation,
            0, map_size);
      }
      if(currently_selected_object == kEditorObject_And){
        temporary_gate = new AndGate(grid_position_position, temporary_rotation,
            0, map_size);
      }
      if(currently_selected_object == kEditorObject_Not){
        temporary_gate = new NotGate(grid_position_position, temporary_rotation,
            0, map_size);
      }
      
    }
  }
  
  if(keys_down[kKey_Constant_1]){
    currently_selected_object = kEditorObject_Constant_1;
  }
  
  if(keys_down[kKey_And]){
    currently_selected_object = kEditorObject_And;
  }
  
  if(keys_down[kKey_Not]){
    currently_selected_object = kEditorObject_Not;
  }
   if(keys_down[kKey_Wire]){
    currently_selected_object = kEditorObject_Wire;
  }
  
  if(keys_down[kKey_Separator]){
    currently_selected_object = kEditorObject_Separator;
  }
  
  CreateTemporaryObject(currently_selected_object, grid_position_position);
  /*
  if(keys_down[kKey_Constant_1]){
    currently_selected_object = kEditorObject_Constant_1;
    Clean();
    temporary_gate = new Constant_1(grid_position_position,
            kDirection_Down, 0, map_size);
  }
  
  if(keys_down[kKey_And]){
    Clean();
    currently_selected_object = kEditorObject_And;
    temporary_gate = new AndGate(grid_position_position, temporary_rotation,
            0, map_size);
  }
  
  if(keys_down[kKey_Not]){
    Clean();
    currently_selected_object = kEditorObject_Not;
    temporary_gate = new NotGate(grid_position_position, kDirection_Down,
            0, map_size);
  }
   if(keys_down[kKey_Wire]){
    currently_selected_object = kEditorObject_Wire;
    Clean();
    temporary_wire = new Wire(grid_position_position, 0, temporary_rotation);
  }
  
  if(keys_down[kKey_Separator]){
    Clean();
    currently_selected_object = kEditorObject_Separator;
    temporary_gate = new Separator(grid_position_position, temporary_rotation,
            0, map_size);
  }
  
  if(temporary_wire){
    temporary_wire->body->bbox.MoveTo(grid_position_position);
  }
  if(temporary_gate){
    temporary_gate->body->bbox.MoveTo(grid_position_position);
  }*/
  
  bool temp_collision = false;
  
  //if(!selecting_area && mouse_buttons_down[SDL_BUTTON_LEFT]){
    
    
  for(auto &gate : logic_gate_map){
    if(gate){
      if(temporary_gate){
        temp_collision = gate->body->bbox.CollisionWithBbox(temporary_gate->body->bbox);
        if(temp_collision){
          break;
        }
      }
      else if(temporary_wire){
        temp_collision = gate->body->bbox.CollisionWithBbox(temporary_wire->body->bbox);
        if(temp_collision){
          break;
        }
      }
    }
  }
  for(auto &wire : wire_map){
    if(wire){
      if(temporary_gate){
        temp_collision = wire->body->bbox.CollisionWithBbox(temporary_gate->body->bbox);
        if(temp_collision){
          break;
        }
      }
    }
  }
  
  if(temp_collision){
    Clean();
  }
  
  if(!temp_collision && !selecting_area && mouse_buttons_down[SDL_BUTTON_LEFT]){
    if(currently_selected_object == kEditorObject_And) {      
      logic_gate_map.push_back(new AndGate(grid_position_position, temporary_rotation,
             position_in_vector, map_size));
    } else if(currently_selected_object == kEditorObject_Constant_1){
      logic_gate_map.push_back(new Constant_1(grid_position_position, kDirection_Down,
             position_in_vector, map_size));
    } else if(currently_selected_object == kEditorObject_Not){
      logic_gate_map.push_back(new NotGate(grid_position_position, temporary_rotation,
             position_in_vector, map_size));
    } else if(currently_selected_object == kEditorObject_Separator){
      logic_gate_map.push_back(new Separator(grid_position_position, temporary_rotation,
             position_in_vector, map_size));
    }else if(currently_selected_object == kEditorObject_Wire) {
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
          delete wire_map[i];
          wire_map[i] = nullptr;
        }
      }
    }
    for(auto &logic_gate : logic_gate_map){
      if(logic_gate){
        if(logic_gate->body->bbox.CollisionWithPoint(mouse_position_in_world)){
          delete logic_gate;
          logic_gate = nullptr;
        }
      }
    }
   
  }
  Vecf player_movement = player.ReceiveInput(keys_down, mouse_buttons_down);
  
  
  if(mouse_buttons_down[SDL_BUTTON_LEFT]){
    mouse_button_pressed_last_frame = true;
  }
  else{
    //we let go of the button, we can create the temporary objects
    mouse_button_pressed_last_frame = false;
    keeping_mouse_pressed = false;
    selecting_area = false;
    
    for(int i = 0 ; i < temporary_wire_map_blueprints.size() ; i++){
      if(temporary_wire_map_blueprints[i]){
        wire_map[i] = temporary_wire_map_blueprints[i];
        temporary_wire_map_blueprints[i] = nullptr;
      }
    }
  }
  return player_movement;
}

void 
GameData::Update() {  
  for(auto &wire : wire_map){
    if(wire){
      wire->CheckIfHasEnergy(energy_map);
    }
  }
  
  std::vector<std::array<Energy*, 4>> temporary_energy_map;
  for(int i = 0 ; i < map_size.x * map_size.y ; ++i){
    temporary_energy_map.push_back(std::array<Energy*, 4>{nullptr});
  }
  for(auto &logic_gate : logic_gate_map){
    if(logic_gate){
  logic_gate->CheckOutputToWires(temporary_energy_map, map_size);
    }
  }
  //todo: fix memory leak!
  for(int i = 0 ; i < energy_map.size() ; i++){
      if(wire_map[i]){
        Wire* wire = wire_map[i];
        
        switch(wire->body->direction){
          case kDirection_Down:
            temporary_energy_map[i + map_size.x][kDirection_Down] = new Energy(wire->logical_state);
            std::cout << i + map_size.x << std::endl;
            break;
          case kDirection_Right:
            temporary_energy_map[i+1][kDirection_Right] = new Energy(wire->logical_state);
            break;
          case kDirection_Left:
            
            temporary_energy_map[i-1][kDirection_Left] = new Energy(wire->logical_state);
            break;
          case kDirection_Up:
            temporary_energy_map[i - map_size.y][kDirection_Up] = new Energy(wire->logical_state);
            break;
        }
        delete energy_map[i][0];
        delete energy_map[i][1];
        delete energy_map[i][2];
        delete energy_map[i][3];
        energy_map[i][0] = nullptr;
        energy_map[i][1] = nullptr;
        energy_map[i][2] = nullptr;
        energy_map[i][3] = nullptr;
      }
      else {
        //if(!logic_gate_map[i]){
          //temporary_energy_map[i] = energy_map[i];
        //}
      }
  }
  
  energy_map = temporary_energy_map;
  //std::cout << energy_map[301][1] << wire_map[301] << std::endl;
  
  for(auto &logic_gate : logic_gate_map){
    if(logic_gate){
      
      logic_gate->RunLogic(energy_map);
      
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
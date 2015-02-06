#include <iostream>
#include "gamedata.h"
#include "engine.h"
#include "wire.h"
#include "and_gate.h"
#include "constant_1.h"
#include "not_gate.h"
#include "separator.h"
#include "save_gate_window.h"
#include <memory>

const int CELLS_SIZE = 22;

GameData::GameData(const Veci& window_size) :
player(Vecf{window_size.x/2.0f, window_size.y/2.0f}),
map_size{100,100},
currently_selected_object(kEditorObject_None),
temporary_gate(nullptr),
temporary_wire(nullptr)
//,selecting_area(false)
,temporary_rotation(kDirection_Down)
,update_delay(100)
,update_timer(0)
,save_gate_window(nullptr)
,selecting_area(false){
  wire_map.reserve(map_size.x * map_size.y);
  for(int i = 0 ; i < map_size.x * map_size.y ; ++i){
    wire_map.push_back(nullptr);
  }
  
  for(int i = 0 ; i < map_size.x * map_size.y ; ++i){
    logic_gate_map.push_back(nullptr);
  }
  
  temporary_wire_map_blueprints.reserve(map_size.x * map_size.y);
  for(int i = 0 ; i < map_size.x * map_size.y ; ++i){
    temporary_wire_map_blueprints.push_back(nullptr);
  }
 
  for(int i = 0 ; i < map_size.x * map_size.y ; ++i){
    energy_map.push_back(std::array<Energy*, 4>{nullptr});
  }
 
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
  
  if(save_gate_window){
    save_gate_window->Render(engine);
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
      //do nothing
      break;
  }
}


Vecf 
GameData::ReceiveInput( const std::array<bool, kKey_Count>& keys_down, 
                        const std::array<bool, 255 > & mouse_buttons_down, 
                        const Veci& mouse_position_in_window,
                        const Vecf& mouse_position_in_world,
                        const std::array<bool, kKey_Count>& last_keys_down) {

  if(keys_down[kKey_Speed_Down]){
    if(update_delay < 1000){
      update_delay += 100;
    }
  }
  if(keys_down[kKey_Speed_Up]){
    if(update_delay >= 100){
      update_delay -= 100;
    }
  }
  
  //position of the mouse on the map grid, so {5, 10} is the 5th column, 10th row
  mouse_grid_position = {static_cast<int>(mouse_position_in_world.x)/CELLS_SIZE,
                         static_cast<int>(mouse_position_in_world.y)/CELLS_SIZE};
  
  grid_position_position = { static_cast<float>(mouse_grid_position.x) * CELLS_SIZE,  
                             static_cast<float>(mouse_grid_position.y) * CELLS_SIZE };
  
  int position_in_vector = 
                  mouse_grid_position.x + mouse_grid_position.y * map_size.x;
  
  if(keys_down[kKey_Select_To_Save] && mouse_buttons_down[SDL_BUTTON_LEFT]){
    if(!selecting_area){
      selecting_area = true;
      selection_grid_position_begin = mouse_grid_position;
    }
    else{
      selection_grid_position_end = mouse_grid_position;
    }
  }
  if(selecting_area && !keys_down[kKey_Select_To_Save]){
    std::cout << selection_grid_position_begin.x << " " << selection_grid_position_end.x << std::endl;
    selecting_area = false;
    
    save_gate_window = new SaveGateWindow(Vecf{100, 100});
  }
  
  if(save_gate_window){
    
    if(keys_down[kKey_Enter]){
      //save
      temporary_pasted_gates.clear();
      temporary_pasted_wires.clear();
      Veci size_selection = {selection_grid_position_end.x - selection_grid_position_begin.x,
                            selection_grid_position_end.x - selection_grid_position_begin.x};
      int vector_size = size_selection.x * size_selection.y;
      for(int i = 0 ; i < vector_size ; ++i){
        temporary_pasted_gates.push_back(nullptr);
        temporary_pasted_wires.push_back(nullptr);
      }
      int position_in_vector_of_real_map = 0;
      int position_in_vector_of_temp_map = 0;
      int x_temp = 0;
      int y_temp = 0;
      for(int x = selection_grid_position_begin.x ; 
              x < selection_grid_position_end.x ; ++x){
        for(int y = selection_grid_position_begin.y ; 
                y < selection_grid_position_end.y; ++y){
          
          position_in_vector_of_real_map = x + map_size.y * y;
          position_in_vector_of_temp_map = x_temp * y_temp;
          //(temporary_pasted_gates[position_in_vector_of_temp_map]) = 
                  
                  //*(logic_gate_map[position_in_vector_of_real_map]);
          /**temporary_pasted_wires[position_in_vector_of_temp_map] = 
                  *wire_map[position_in_vector_of_real_map];*/
          ++y_temp;
        }
        ++x_temp;
      }
      delete save_gate_window;
      save_gate_window = nullptr;
    }
  }
  
  if(!keeping_mouse_pressed &&
          mouse_buttons_down[SDL_BUTTON_LEFT] && 
          mouse_button_pressed_last_frame){
    making_line_of_wires_begin = mouse_grid_position;
    keeping_mouse_pressed = true;
  }
  
  
  if(!selecting_area){
    if(keeping_mouse_pressed && 
            (making_line_of_wires_begin.x != mouse_grid_position.x ||
            making_line_of_wires_begin.y != mouse_grid_position.y)) {
      making_line_of_wires = true;
    }

    if(keeping_mouse_pressed && making_line_of_wires &&
            currently_selected_object == kEditorObject_Wire){
      for(auto &temp_wire : temporary_wire_map_blueprints){
        delete temp_wire;
        temp_wire = nullptr;
      }



      //calculating distance from origin of selection to know
      //direction of slide
      int diff_x = mouse_grid_position.x - making_line_of_wires_begin.x;
      int diff_y = mouse_grid_position.y - making_line_of_wires_begin.y;
      if(diff_x >= 0 && diff_y >= 0){
        if(diff_x > diff_y){
          temporary_rotation = kDirection_Right;
          for(int i = making_line_of_wires_begin.x ;
                  i <= mouse_grid_position.x ;
                  ++i){
            int position_in_vector = 
                    i + making_line_of_wires_begin.y * map_size.x;
            Vecf real_position = { static_cast<float>(i) * CELLS_SIZE,  
                               static_cast<float>(making_line_of_wires_begin.y) * CELLS_SIZE };
            temporary_wire_map_blueprints[position_in_vector] = new Wire(real_position,
                    position_in_vector, temporary_rotation);
          }
        }
        else{
          temporary_rotation = kDirection_Down;
          for(int i = making_line_of_wires_begin.y ;
                  i <= mouse_grid_position.y ;
                  ++i){
            int position_in_vector = 
                    making_line_of_wires_begin.x + i * map_size.x;
            Vecf real_position = { static_cast<float>(making_line_of_wires_begin.x) * CELLS_SIZE,  
                               static_cast<float>(i) * CELLS_SIZE };
            temporary_wire_map_blueprints[position_in_vector] = new Wire(real_position,
                    position_in_vector, temporary_rotation);
          }
        }
      }
      if(diff_x <= 0 && diff_y >= 0){
        if(abs(diff_x) > diff_y){
          temporary_rotation = kDirection_Left;
          for(int i = making_line_of_wires_begin.x ;
                  i >= mouse_grid_position.x ;
                  --i){
            int position_in_vector = 
                    i + making_line_of_wires_begin.y * map_size.x;
            Vecf real_position = { static_cast<float>(i) * CELLS_SIZE,  
                               static_cast<float>(making_line_of_wires_begin.y) * CELLS_SIZE };
            temporary_wire_map_blueprints[position_in_vector] = new Wire(real_position,
                    position_in_vector, temporary_rotation);
          }
        }
        else{
          temporary_rotation = kDirection_Down;
          for(int i = making_line_of_wires_begin.y ;
                  i <= mouse_grid_position.y ;
                  ++i){
            int position_in_vector = 
                    making_line_of_wires_begin.x + i * map_size.x;
            Vecf real_position = { static_cast<float>(making_line_of_wires_begin.x) * CELLS_SIZE,  
                               static_cast<float>(i) * CELLS_SIZE };
            temporary_wire_map_blueprints[position_in_vector] = new Wire(real_position,
                    position_in_vector, temporary_rotation);
          }
        }
      }
      if(diff_x <= 0 && diff_y <= 0){
        if(abs(diff_x) > abs(diff_y)){
          temporary_rotation = kDirection_Left;
          for(int i = making_line_of_wires_begin.x ;
                  i >= mouse_grid_position.x ;
                  --i){
            int position_in_vector = 
                    i + making_line_of_wires_begin.y * map_size.x;
            Vecf real_position = { static_cast<float>(i) * CELLS_SIZE,  
                               static_cast<float>(making_line_of_wires_begin.y) * CELLS_SIZE };
            temporary_wire_map_blueprints[position_in_vector] = new Wire(real_position,
                    position_in_vector, temporary_rotation);
          }
        }
        else{
          temporary_rotation = kDirection_Up;
          for(int i = making_line_of_wires_begin.y ;
                  i >= mouse_grid_position.y ;
                  --i){
            int position_in_vector = 
                    making_line_of_wires_begin.x + i * map_size.x;
            Vecf real_position = { static_cast<float>(making_line_of_wires_begin.x) * CELLS_SIZE,  
                               static_cast<float>(i) * CELLS_SIZE };
            temporary_wire_map_blueprints[position_in_vector] = new Wire(real_position,
                    position_in_vector, temporary_rotation);
          }
        }
      }
      if(diff_x >= 0 && diff_y <= 0){
        if(diff_x > abs(diff_y)){
          temporary_rotation = kDirection_Right;
          for(int i = making_line_of_wires_begin.x ;
                  i <= mouse_grid_position.x ;
                  ++i){
            int position_in_vector = 
                    i + making_line_of_wires_begin.y * map_size.x;
            Vecf real_position = { static_cast<float>(i) * CELLS_SIZE,  
                               static_cast<float>(making_line_of_wires_begin.y) * CELLS_SIZE };
            temporary_wire_map_blueprints[position_in_vector] = new Wire(real_position,
                    position_in_vector, temporary_rotation);
          }
        }
        else{
          temporary_rotation = kDirection_Up;
          for(int i = making_line_of_wires_begin.y ;
                  i >= mouse_grid_position.y ;
                  --i){
            int position_in_vector = 
                    making_line_of_wires_begin.x + i * map_size.x;
            Vecf real_position = { static_cast<float>(making_line_of_wires_begin.x) * CELLS_SIZE,  
                               static_cast<float>(i) * CELLS_SIZE };
            temporary_wire_map_blueprints[position_in_vector] = new Wire(real_position,
                    position_in_vector, temporary_rotation);
          }
        }
      }

    }

    bool pressed_rotate = false;

    if(keys_down[kKey_Rotate_Left]){
      temporary_rotation = (eDirection)(temporary_rotation - 1);
      if(temporary_rotation < 0){
        temporary_rotation = (eDirection)(kDirection_Count - 1);
      }
      pressed_rotate = true;
    }
    if(keys_down[kKey_Rotate_Right]){
      temporary_rotation = (eDirection)(temporary_rotation + 1);
      if(temporary_rotation >= (int)kDirection_Count){
        temporary_rotation = (eDirection)0;
      }
      pressed_rotate = true;
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

      if(making_line_of_wires){
      Clean();
    }
    bool temp_collision = false;


    for(auto &gate : logic_gate_map) {
      if(gate) {
        if(temporary_gate) {
          temp_collision = gate->body->bbox.CollisionWithBbox(temporary_gate->body->bbox);
          if(temp_collision) {
            if(pressed_rotate) {
              gate->Rotate(temporary_rotation, map_size);
            }
            break;
          }
        }
        else if(temporary_wire) {
          temp_collision = gate->body->bbox.CollisionWithBbox(temporary_wire->body->bbox);
          if(temp_collision) {
            if(pressed_rotate) {
              gate->Rotate(temporary_rotation, map_size);
            }
            break;
          }
        }
      }
    }
    for(auto &wire : wire_map) {
      if(wire) {
        if(temporary_gate) {
          temp_collision = wire->body->bbox.CollisionWithBbox(temporary_gate->body->bbox);
          if(temp_collision) {
            if(pressed_rotate){
              wire->body->direction = temporary_rotation;
            }
            break;
          }
        }
        if(temporary_wire) {
          temp_collision = wire->body->bbox.CollisionWithBbox(temporary_wire->body->bbox);
          if(temp_collision) {
            if(pressed_rotate){
              wire->body->direction = temporary_rotation;
            }
            break;
          }
        }
      }
    }

    if(temp_collision) {
      Clean();
    }

    if(!temp_collision && !making_line_of_wires && mouse_buttons_down[SDL_BUTTON_LEFT]){
      if(currently_selected_object == kEditorObject_And) {      
        logic_gate_map[position_in_vector]= new AndGate(grid_position_position, temporary_rotation,
               position_in_vector, map_size);
      } else if(currently_selected_object == kEditorObject_Constant_1){
        logic_gate_map[position_in_vector]= new Constant_1(grid_position_position, kDirection_Down,
               position_in_vector, map_size);
      } else if(currently_selected_object == kEditorObject_Not){
        logic_gate_map[position_in_vector]= new NotGate(grid_position_position, temporary_rotation,
               position_in_vector, map_size);
      } else if(currently_selected_object == kEditorObject_Separator){
        logic_gate_map[position_in_vector]= new Separator(grid_position_position, temporary_rotation,
               position_in_vector, map_size);
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
  }//if !selecting_area
  
  Vecf player_movement = player.ReceiveInput(keys_down, mouse_buttons_down);
  
  
  if(mouse_buttons_down[SDL_BUTTON_LEFT]){
    mouse_button_pressed_last_frame = true;
  }
  else{
    //we let go of the button, we can create the temporary objects
    mouse_button_pressed_last_frame = false;
    keeping_mouse_pressed = false;
    making_line_of_wires = false;
    
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
  
  update_timer += g_delta_t;
  if(update_timer > update_delay){
    update_timer = 0;
    
    

    std::vector<std::array<Energy*, 4>> temporary_energy_map;
    for(int i = 0 ; i < map_size.x * map_size.y ; ++i){
      temporary_energy_map.push_back(std::array<Energy*, 4>{nullptr});
    }
    
    

    
    //todo: fix memory leak!
    for(int i = 0 ; i < energy_map.size() ; i++){
        if(wire_map[i]){
          Wire* wire = wire_map[i];
          bool moved_energy = false;
          
          switch(wire->body->direction){
            case kDirection_Down:
              if(wire_map[i + map_size.x] || logic_gate_map[i + map_size.x]){
                temporary_energy_map[i + map_size.x][kDirection_Down] = new Energy(wire->logical_state);
                //moved_energy = true;
              }
              break;
            case kDirection_Right:
              if(wire_map[i + 1] || logic_gate_map[i + 1]){
                temporary_energy_map[i+1][kDirection_Right] = new Energy(wire->logical_state);
                moved_energy = true;
              }
              break;
            case kDirection_Left:
              if(wire_map[i - 1] || logic_gate_map[i- 1]){
                temporary_energy_map[i-1][kDirection_Left] = new Energy(wire->logical_state);
                moved_energy = true;
              }
              break;
            case kDirection_Up:
              if(wire_map[i - map_size.x] || logic_gate_map[i - map_size.x]){
                temporary_energy_map[i - map_size.y][kDirection_Up] = new Energy(wire->logical_state);
                moved_energy = true;
              }
              break;
          }

        }
        else {
          if(logic_gate_map[i]){
            //temporary_energy_map[i] = energy_map[i];
          }
        }
    }
    
    for(auto &logic_gate : logic_gate_map){
      if(logic_gate){
        logic_gate->CheckOutputToWires(energy_map, temporary_energy_map, map_size);
      }
    }

    
    for(auto &energy : energy_map){
      for(int i = 0 ; i < 4 ; i++){
        delete energy[i];
        energy[i] = nullptr;
      }
    }
    energy_map = temporary_energy_map;

        
    for(auto &wire : wire_map){
      if(wire){
        wire->CheckIfHasEnergy(energy_map);
      }
    }
    
    for(auto &logic_gate : logic_gate_map){
      if(logic_gate){

        logic_gate->RunLogic(energy_map);

      }
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
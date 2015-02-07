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
,update_delay(0)
,update_timer(0)
,save_gate_window(nullptr)
,selecting_area(false)
,selected_area(false)
,wanting_to_paste(false){
  int map_size_total = map_size.x * map_size.y;
  //wire_map.reserve(map_size.x * map_size.y);
  for(int i = 0 ; i < map_size_total ; ++i){
    wire_map.push_back(nullptr);
  }
  
  for(int i = 0 ; i < map_size_total ; ++i){
    wire_map_underground.push_back(nullptr);
  }
  
  for(int i = 0 ; i < map_size_total ; ++i){
    logic_gate_map.push_back(nullptr);
  }
  
  //temporary_wire_map_blueprints.reserve(map_size.x * map_size.y);
  for(int i = 0 ; i < map_size_total ; ++i){
    temporary_wire_map_blueprints.push_back(nullptr);
  }
 
  for(int i = 0 ; i < map_size_total ; ++i){
    energy_map.push_back(std::array<Energy*, 4>{nullptr});
  }
  
  for(int i = 0 ; i < map_size_total ; ++i){
    energy_map_underground.push_back(std::array<Energy*, 4>{nullptr});
  }
 
}

void
GameData::InitializeWorld(const Veci& window_size) {
  //player = Player(Vecf{window_size.x/2.0f, window_size.y/2.0f});
}

void 
GameData::Render(Engine& engine) {

  
  for(auto &wire : wire_map_underground){  
    if(wire){
      wire->body->Render(engine);
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
  
  for(auto &logic_gate : logic_gate_map){
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
  
  if(wanting_to_paste){
    for(auto &gate : clipboard_gates){
      if(gate){
        gate->body->Render(engine);
      }
    }
    for(auto &wire : clipboard_wires){
      if(wire){
        wire->body->Render(engine);
      }
    }
    for(auto &wire : clipboard_wires_underground){
      if(wire){
        wire->body->Render(engine);
      }
    }
  }
  if(selecting_area || selected_area){
    
    Veci selection_size = {selection_grid_position_end.x  * CELLS_SIZE - 
    selection_grid_position_begin.x * CELLS_SIZE, 
    selection_grid_position_end.y  * CELLS_SIZE - selection_grid_position_begin.y  * CELLS_SIZE};
    SDL_Rect selection_rect{
      static_cast<int>(selection_grid_position_begin.x * CELLS_SIZE - engine.camera.x),
      static_cast<int>(selection_grid_position_begin.y * CELLS_SIZE - engine.camera.y),
            selection_size.x, selection_size.y};
            

    SDL_SetRenderDrawColor(engine.renderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(engine.renderer, &selection_rect);
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
      temporary_wire = new Wire(position, 0, temporary_rotation, kEditorObject_Wire);
      break;
    case kEditorObject_Wire_Underground_Exit:
      temporary_wire = new Wire(position, 0, temporary_rotation, kEditorObject_Wire_Underground_Exit);
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
                        const std::array<bool, kKey_Count>& last_keys_down,
        const std::array<bool, 255>& last_mouse_buttons_down) {

      CreateTemporaryObject(currently_selected_object, grid_position_position);

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
      
      bool pressed_object_selection_key = false;

  if(keys_down[kKey_Constant_1]){
    currently_selected_object = kEditorObject_Constant_1;
    pressed_object_selection_key = true;
  }

  if(keys_down[kKey_And]){
    currently_selected_object = kEditorObject_And;
    pressed_object_selection_key = true;
  }

  if(keys_down[kKey_Not]){
    currently_selected_object = kEditorObject_Not;
    pressed_object_selection_key = true;
  }
  
  if(keys_down[kKey_Wire]){
    currently_selected_object = kEditorObject_Wire;
    pressed_object_selection_key = true;
  }
      
  if(keys_down[kKey_Wire_Underground_Exit]){
    currently_selected_object = kEditorObject_Wire_Underground_Exit;
    pressed_object_selection_key = true;
  }

  if(keys_down[kKey_Separator]){
    currently_selected_object = kEditorObject_Separator;
    pressed_object_selection_key = true;
  }
      
  if(pressed_object_selection_key){
    wanting_to_paste = false;
  }

      
  //position of the mouse on the map grid, so {5, 10} is the 5th column, 10th row
  mouse_grid_position = {static_cast<int>(mouse_position_in_world.x)/CELLS_SIZE,
                         static_cast<int>(mouse_position_in_world.y)/CELLS_SIZE};
  
  grid_position_position = { static_cast<float>(mouse_grid_position.x) * CELLS_SIZE,  
                             static_cast<float>(mouse_grid_position.y) * CELLS_SIZE };
  
  int position_in_vector = 
                  mouse_grid_position.x + mouse_grid_position.y * map_size.x;
  
  if(keys_down[kKey_Select_To_Save]){
    if(!selecting_area){
      selecting_area = true;
      selection_grid_position_begin = mouse_grid_position;
    }
    else{
      selection_grid_position_end = mouse_grid_position;
    }
  }
  if(selecting_area && !keys_down[kKey_Select_To_Save]){
    selecting_area = false;
    selected_area = true;
    //save_gate_window = new SaveGateWindow(Vecf{100, 100});
  }
  
  if(selected_area && keys_down[kKey_Delete] && !last_keys_down[kKey_Copy]){
    selected_area = false;
    Veci selection_begin_copy = selection_grid_position_begin;
    Veci selection_end_copy = selection_grid_position_end;
    
    if(selection_grid_position_begin.x > selection_grid_position_end.x){
      selection_grid_position_begin.x = selection_grid_position_end.x;
      selection_grid_position_end.x = selection_begin_copy.x;
    }
    if(selection_grid_position_end.y < selection_grid_position_begin.y){
      selection_grid_position_begin.y = selection_grid_position_end.y;
      selection_grid_position_end.y = selection_begin_copy.y;
    }
    Veci size_selection = {selection_grid_position_end.x - selection_grid_position_begin.x,
                            selection_grid_position_end.y - selection_grid_position_begin.y};
    
    for(int y = selection_grid_position_begin.y ; 
                y < selection_grid_position_end.y; ++y){
      for(int x = selection_grid_position_begin.x ; 
                x < selection_grid_position_end.x ; ++x){
        
        int position_in_map = x + map_size.x * y;
        
        delete wire_map[position_in_map];
        wire_map[position_in_map] = nullptr;
        delete wire_map_underground[position_in_map];
        wire_map_underground[position_in_map] = nullptr;
        delete logic_gate_map[position_in_map];
        logic_gate_map[position_in_map] = nullptr;
      }
    }
  }
  
  if(selected_area && keys_down[kKey_Copy] && !last_keys_down[kKey_Copy]){
    selected_area = false;
    clipboard_gates.clear();
    clipboard_wires.clear();
    clipboard_wires_underground.clear();
    Veci selection_begin_copy = selection_grid_position_begin;
    Veci selection_end_copy = selection_grid_position_end;
    
    if(selection_grid_position_begin.x > selection_grid_position_end.x){
      selection_grid_position_begin.x = selection_grid_position_end.x;
      selection_grid_position_end.x = selection_begin_copy.x;
    }
    if(selection_grid_position_end.y < selection_grid_position_begin.y){
      selection_grid_position_begin.y = selection_grid_position_end.y;
      selection_grid_position_end.y = selection_begin_copy.y;
    }
    Veci size_selection = {selection_grid_position_end.x - selection_grid_position_begin.x,
                            selection_grid_position_end.y - selection_grid_position_begin.y};
    clipboard_size = size_selection;
    std::cout << clipboard_size.y << std::endl;
    int vector_size = size_selection.x * size_selection.y;
    for(int i = 0 ; i < vector_size ; ++i){
      clipboard_gates.push_back(nullptr);
      clipboard_wires.push_back(nullptr);
      clipboard_wires_underground.push_back(nullptr);
    }
    int position_in_vector_of_real_map = 0;
    int position_in_vector_of_clipboard_map = 0;
    int x_temp = 0;
    int y_temp = 0;
    for(int y = selection_grid_position_begin.y ; 
                y < selection_grid_position_end.y; ++y){
      for(int x = selection_grid_position_begin.x ; 
                x < selection_grid_position_end.x ; ++x){
        
          position_in_vector_of_real_map = x + map_size.x * y;
          position_in_vector_of_clipboard_map = x_temp + size_selection.x * y_temp;

            if(logic_gate_map[position_in_vector_of_real_map]){
              
              LogicGate* gate = logic_gate_map[position_in_vector_of_real_map];
              switch(gate->object_type){
                case kEditorObject_Not:
                  clipboard_gates[position_in_vector_of_clipboard_map] = 
                    new NotGate(Vecf{gate->body->bbox.left, gate->body->bbox.top},
                          gate->body->direction, position_in_vector_of_clipboard_map, 
                             map_size);
                  break;
                case kEditorObject_And:
                  clipboard_gates[position_in_vector_of_clipboard_map] = 
                    new AndGate(Vecf{gate->body->bbox.left, gate->body->bbox.top},
                          gate->body->direction, position_in_vector_of_clipboard_map, 
                             map_size);
                  break;
                case kEditorObject_Separator:
                  clipboard_gates[position_in_vector_of_clipboard_map] = 
                     new Separator(Vecf{gate->body->bbox.left, gate->body->bbox.top},
                          gate->body->direction, position_in_vector_of_clipboard_map, 
                             map_size);
                  break;
                default:
                  break;
              }
            }
            if(wire_map[position_in_vector_of_real_map]){
              
              Wire* wire = wire_map[position_in_vector_of_real_map];
              clipboard_wires[position_in_vector_of_clipboard_map] = 
                      new Wire(Vecf{wire->body->bbox.left, wire->body->bbox.top},
                       position_in_vector_of_clipboard_map, wire->body->direction,
                              kEditorObject_Wire);
            }
            if(wire_map_underground[position_in_vector_of_real_map]){
              Wire* wire = wire_map_underground[position_in_vector_of_real_map];
              clipboard_wires_underground[position_in_vector_of_clipboard_map] = 
                      new Wire(Vecf{wire->body->bbox.left, wire->body->bbox.top},
                        position_in_vector_of_clipboard_map,wire->body->direction,
                              kEditorObject_Wire_Underground);
            }
              
           
          ++x_temp;
          
          
        }
      ++y_temp;
      x_temp = 0;
    }
  }
  
  if(keys_down[kKey_Paste] && !last_keys_down[kKey_Paste]){
    wanting_to_paste = true;
    selected_area = false;
  }
  
  if(wanting_to_paste){
    Clean();
    if(wanting_to_paste){
      for(int y = 0 ; y < clipboard_size.y ; y++){
        for(int x = 0 ; x < clipboard_size.x ; x++){
          int vector_pos = x + y * clipboard_size.x;
          Vecf new_position = {
              static_cast<float>(mouse_grid_position.x * CELLS_SIZE + x * CELLS_SIZE),
              static_cast<float>(mouse_grid_position.y * CELLS_SIZE + y * CELLS_SIZE) };
          if(clipboard_gates[vector_pos]){
            clipboard_gates[vector_pos]->body->bbox.MoveTo(new_position);
          }
          if(clipboard_wires_underground[vector_pos]){
            clipboard_wires_underground[vector_pos]->body->bbox.MoveTo(new_position);
          }
          if(clipboard_wires[vector_pos]){
            clipboard_wires[vector_pos]->body->bbox.MoveTo(new_position);
          }
          
        }
      }
    }
    
    if(mouse_buttons_down[SDL_BUTTON_LEFT]){
      for(int y = 0 ; y < clipboard_size.y ; y++){
        for(int x = 0 ; x < clipboard_size.x ; x++){
          int vector_pos = x + y * clipboard_size.x;
          if(clipboard_gates[vector_pos]){
            LogicGate* gate = clipboard_gates[vector_pos];
          
            int vector_pos_in_real_map = 
            mouse_grid_position.x + mouse_grid_position.y * map_size.x +
            x + y * map_size.x;
            
            delete logic_gate_map[vector_pos_in_real_map];
            logic_gate_map[vector_pos_in_real_map] = nullptr;
            
            switch(gate->object_type){
              case kEditorObject_Not:
                logic_gate_map[vector_pos_in_real_map] = 
                  new NotGate(Vecf{gate->body->bbox.left, gate->body->bbox.top},
                       gate->body->direction, vector_pos_in_real_map, 
                          map_size);;
                break;
              case kEditorObject_And:
                logic_gate_map[vector_pos_in_real_map] = 
                  new AndGate(Vecf{gate->body->bbox.left, gate->body->bbox.top},
                        gate->body->direction, vector_pos_in_real_map, 
                           map_size);;
                break;
              case kEditorObject_Separator:
                logic_gate_map[vector_pos_in_real_map] = 
                   new Separator(Vecf{gate->body->bbox.left, gate->body->bbox.top},
                        gate->body->direction, vector_pos_in_real_map, 
                           map_size);;
                break;
             default:
               break;
            }
          }
          if(clipboard_wires[vector_pos]){
            Wire* wire = clipboard_wires[vector_pos];
            int vector_pos_in_real_map = 
            mouse_grid_position.x + mouse_grid_position.y * map_size.x +
            x + y * map_size.x;
            
            delete wire_map[vector_pos_in_real_map];
            wire_map[vector_pos_in_real_map] = nullptr;
            wire_map[vector_pos_in_real_map] = 
                    new Wire(Vecf{wire->body->bbox.left, wire->body->bbox.top},
                       vector_pos_in_real_map, wire->body->direction,
                              kEditorObject_Wire);
            
          }
          if(clipboard_wires_underground[vector_pos]){
            Wire* wire = clipboard_wires_underground[vector_pos];
            int vector_pos_in_real_map = 
            mouse_grid_position.x + mouse_grid_position.y * map_size.x +
            x + y * map_size.x;
            
            delete wire_map_underground[vector_pos_in_real_map];
            wire_map_underground[vector_pos_in_real_map] = nullptr;
            wire_map_underground[vector_pos_in_real_map] = 
                    new Wire(Vecf{wire->body->bbox.left, wire->body->bbox.top},
                        vector_pos_in_real_map,wire->body->direction,
                              kEditorObject_Wire_Underground);
                    std::cout << "yeay" << std::endl;
            
          }
        }
      }
    } 
  }
  
  /*if(save_gate_window){
    
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
          ++y_temp;
        }
        ++x_temp;
      }
      delete save_gate_window;
      save_gate_window = nullptr;
    }
  }
  */
  
  if(!wanting_to_paste){
    if(!keeping_mouse_pressed &&
            mouse_buttons_down[SDL_BUTTON_LEFT] && 
            mouse_button_pressed_last_frame){
      making_line_of_wires_begin = mouse_grid_position;
      making_line_of_wires_begin_backup = making_line_of_wires_begin;
      keeping_mouse_pressed = true;
    }


    if(!selecting_area){
      if(keeping_mouse_pressed && 
              (making_line_of_wires_begin.x != mouse_grid_position.x ||
              making_line_of_wires_begin.y != mouse_grid_position.y)) {
        making_line_of_wires = true;
      }

      if(keeping_mouse_pressed && making_line_of_wires &&
              (currently_selected_object == kEditorObject_Wire ||
              currently_selected_object == kEditorObject_Wire_Underground_Exit)){
        for(auto &temp_wire : temporary_wire_map_blueprints){
          delete temp_wire;
          temp_wire = nullptr;
        }



        //always begin creation top left corner
        //so we compare the begin and end and swap values if needed
        
        int diff_x = mouse_grid_position.x - making_line_of_wires_begin.x;
        int diff_y = mouse_grid_position.y - making_line_of_wires_begin.y;
        int spawn_begin = 0;
        int spawn_end = 0;
        if(abs(diff_x) > abs(diff_y)){
          if(diff_x >= 0 && diff_y >= 0){
            temporary_rotation = kDirection_Right;
            spawn_begin = making_line_of_wires_begin.x;
            spawn_end = mouse_grid_position.x;
          }
          
          else if(diff_x <= 0 && diff_y >= 0){
            temporary_rotation = kDirection_Left;
            spawn_begin = mouse_grid_position.x;
            spawn_end = making_line_of_wires_begin.x;
          }
          
          else if(diff_x <= 0 && diff_y <= 0){
            temporary_rotation = kDirection_Left;
            spawn_begin = mouse_grid_position.x;
            spawn_end = making_line_of_wires_begin.x;
          }
            
          else if(diff_x >= 0 && diff_y <= 0){
            temporary_rotation = kDirection_Right;
            spawn_begin = making_line_of_wires_begin.x;
            spawn_end = mouse_grid_position.x;
          }  
        } else {
          if(diff_x >= 0 && diff_y >= 0){
            temporary_rotation = kDirection_Down;
            spawn_begin = making_line_of_wires_begin.y;
            spawn_end = mouse_grid_position.y;
          }
          
          else if(diff_x <= 0 && diff_y >= 0){
            temporary_rotation = kDirection_Down;
            spawn_begin = making_line_of_wires_begin.y;
            spawn_end = mouse_grid_position.y;
          }
          
          else if(diff_x <= 0 && diff_y <= 0){
            temporary_rotation = kDirection_Up;
            spawn_begin = mouse_grid_position.y;
            spawn_end = making_line_of_wires_begin.y;
          }
          
          else if(diff_x >= 0 && diff_y <= 0){
            temporary_rotation = kDirection_Up;
            spawn_begin = mouse_grid_position.y;
            spawn_end = making_line_of_wires_begin.y;
          }
        }
        
        for(int i = spawn_begin ;
                    i <= spawn_end ;
                    ++i){ 
          int position_in_vector = 0;
          Vecf real_position = {0.0f, 0.0f};
          if(temporary_rotation == kDirection_Right || 
                  temporary_rotation == kDirection_Left){
            position_in_vector = i + making_line_of_wires_begin.y * map_size.x;
            real_position = { static_cast<float>(i) * CELLS_SIZE,  
                static_cast<float>(making_line_of_wires_begin.y) * CELLS_SIZE };
          }
          else {
            position_in_vector = making_line_of_wires_begin.x + i * map_size.x;
            real_position = { 
                static_cast<float>(making_line_of_wires_begin.x) * CELLS_SIZE,  
                static_cast<float>(i) * CELLS_SIZE };
          }
          if(!logic_gate_map[position_in_vector] || temporary_wire->type != kEditorObject_Wire){
            if(temporary_wire->type == kEditorObject_Wire){
              temporary_wire_map_blueprints[position_in_vector] = new Wire(real_position,
                    position_in_vector, temporary_rotation, temporary_wire->type);
            } else{
              temporary_wire_map_blueprints[position_in_vector] = new Wire(real_position,
                    position_in_vector, temporary_rotation, kEditorObject_Wire_Underground);
              /*if(i == making_line_of_wires_begin.x ||
                      i == mouse_grid_position.x){
                temporary_wire_map_blueprints[position_in_vector] = new Wire(real_position,
                    position_in_vector, temporary_rotation, temporary_wire->type);
              } else{
                temporary_wire_map_blueprints[position_in_vector] = new Wire(real_position,
                    position_in_vector, temporary_rotation, kEditorObject_Wire_Underground);
              }*/
            }
          }
        }
      }
             
      

      bool pressed_rotate = false;

      if((keys_down[kKey_Rotate_Left] && !last_keys_down[kKey_Rotate_Left]) || 
              (mouse_buttons_down[kKey_Mouse_Rotate_Left] && 
              !last_mouse_buttons_down[kKey_Mouse_Rotate_Left])){
        temporary_rotation = (eDirection)(temporary_rotation - 1);
        if(temporary_rotation < 0){
          temporary_rotation = (eDirection)(kDirection_Count - 1);
        }
        pressed_rotate = true;
      }
      if((keys_down[kKey_Rotate_Right] && keys_down[kKey_Rotate_Right]) || 
              (mouse_buttons_down[kKey_Mouse_Rotate_Right] && 
              !last_mouse_buttons_down[kKey_Mouse_Rotate_Right])){
        temporary_rotation = (eDirection)(temporary_rotation + 1);
        if(temporary_rotation >= (int)kDirection_Count){
          temporary_rotation = (eDirection)0;
        }
        pressed_rotate = true;
      }

      

      if(making_line_of_wires){
        Clean();
      }
      bool temp_collision = false;

      static int qsf = 0;
      
      for(auto &gate : logic_gate_map) {
        if(gate) {
          if(mouse_buttons_down[SDL_BUTTON_MIDDLE] &&
                    !last_mouse_buttons_down[SDL_BUTTON_MIDDLE] &&
                  gate->body->bbox.CollisionWithPoint(mouse_position_in_world)){
            if(gate->object_type == kEditorObject_Constant_1){
             if(gate->logical_state == kLogicalState_0){
               gate->logical_state = kLogicalState_1;
             } else if (gate->logical_state == kLogicalState_1){

               gate->logical_state= kLogicalState_0;
             }
           }
          }
           
          if(temporary_gate) {
            temp_collision = gate->body->bbox.CollisionWithPoint(mouse_position_in_world);
            if(temp_collision) {
              
              if(pressed_rotate) {
                gate->Rotate(temporary_rotation, map_size);
              }
             
              Clean();
              break;
            }
          }
          else if(temporary_wire) {
            temp_collision = gate->body->bbox.CollisionWithPoint(mouse_position_in_world);
            if(temp_collision) {
              if(pressed_rotate) {
                gate->Rotate(temporary_rotation, map_size);
              }
              Clean();
              break;
            }
          }
        }
      }
      for(auto &wire : wire_map) {
        if(wire) {
          if(temporary_gate) {
            temp_collision = wire->body->bbox.CollisionWithPoint(mouse_position_in_world);
            if(temp_collision) {
              if(pressed_rotate){
                wire->body->direction = temporary_rotation;
              }
              Clean();
              break;
            }
          }
          if(temporary_wire) {
            temp_collision = wire->body->bbox.CollisionWithPoint(mouse_position_in_world);
            if(temp_collision) {
              if(pressed_rotate){
                wire->body->direction = temporary_rotation;
              }
              Clean();
              break;
            }
          }
        }
      }

      

      if(!temp_collision && !making_line_of_wires && mouse_buttons_down[SDL_BUTTON_LEFT]){
        if(currently_selected_object == kEditorObject_And) {      
          logic_gate_map[position_in_vector]= new AndGate(grid_position_position, temporary_rotation,
                 position_in_vector, map_size);
        } else if(currently_selected_object == kEditorObject_Constant_1){
          logic_gate_map[position_in_vector]= new Constant_1(grid_position_position, temporary_rotation,
                 position_in_vector, map_size);
        } else if(currently_selected_object == kEditorObject_Not){
          logic_gate_map[position_in_vector]= new NotGate(grid_position_position, temporary_rotation,
                 position_in_vector, map_size);
        } else if(currently_selected_object == kEditorObject_Separator){
          logic_gate_map[position_in_vector]= new Separator(grid_position_position, temporary_rotation,
                 position_in_vector, map_size);
        }else if(currently_selected_object == kEditorObject_Wire) {
          if(temporary_wire->type == kEditorObject_Wire){
            delete wire_map[position_in_vector];
            wire_map[position_in_vector] = nullptr;
            wire_map[position_in_vector] = new Wire(grid_position_position,
                      position_in_vector, temporary_rotation, temporary_wire->type);
          }
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
        for(int i = 0 ; i < wire_map_underground.size() ; i++){
          if(wire_map_underground[i]){
            if(wire_map_underground[i]->body->bbox.CollisionWithPoint(mouse_position_in_world)){
              delete wire_map_underground[i];
              wire_map_underground[i] = nullptr;
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
  }//if wanting to paste
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
        if(temporary_wire_map_blueprints[i]->type == kEditorObject_Wire){
          wire_map[i] = temporary_wire_map_blueprints[i];
        } else {
          wire_map_underground[i] = temporary_wire_map_blueprints[i];
        }
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
    
    
    int map_size_total = map_size.x * map_size.y;
    std::vector<std::array<Energy*, 4>> temporary_energy_map;
    for(int i = 0 ; i < map_size_total ; ++i){
      temporary_energy_map.push_back(std::array<Energy*, 4>{nullptr});
    }
    
    std::vector<std::array<Energy*, 4>> temporary_energy_map_underground;
    for(int i = 0 ; i < map_size_total ; ++i){
      temporary_energy_map_underground.push_back(std::array<Energy*, 4>{nullptr});
    }
    
    
    for(int i = 0 ; i < energy_map.size() ; i++){
        if(wire_map[i]){
          Wire* wire = wire_map[i];
          bool moved_energy = false;
          
          if(wire->type == kEditorObject_Wire){
            int direction_grid_position[4] = {
              i + 1,          //right
              i + map_size.y, //down
              i - 1,          //left,
              i - map_size.y //up
            };
            eDirection direction = wire->body->direction;
            if(wire_map[direction_grid_position[direction]] || logic_gate_map[direction_grid_position[direction]]){
                temporary_energy_map[direction_grid_position[direction]][direction] = 
                        new Energy(wire->logical_state, wire->energy_value);
              }
            if(wire_map_underground[direction_grid_position[direction]] && 
                    wire_map_underground[direction_grid_position[direction]]->output_direction == direction){
                temporary_energy_map_underground[direction_grid_position[direction]][direction] = 
                new Energy(wire->logical_state, wire->energy_value);
            }
            
          }
        }
    }
    
    for(int i = 0 ; i < energy_map_underground.size() ; i++){
      if(wire_map_underground[i]){
        Wire* wire = wire_map_underground[i];
        int direction_grid_position[4] = {
            i + 1,          //right
            i + map_size.y, //down
            i - 1,          //left,
            i - map_size.y //up
          };
          eDirection direction = wire->body->direction;
            if(wire_map_underground[direction_grid_position[direction]]){
                temporary_energy_map_underground[direction_grid_position[direction]][direction] = 
                        new Energy(wire->logical_state, wire->energy_value);
            } else {
              if(wire_map[direction_grid_position[direction]]){
                temporary_energy_map[direction_grid_position[direction]][direction] =
                        new Energy(wire->logical_state, wire->energy_value);
              }
            }
      }
    }
          
    for(auto &logic_gate : logic_gate_map){
      if(logic_gate){
        logic_gate->CheckOutputToWires(energy_map, temporary_energy_map, map_size);
        /*logic_gate->CheckOutputToUndergroundWires(
        energy_map_underground, temporary_energy_map_underground, map_size,
                wire_map, wire_map_underground);*/
      }
    }

    
    for(auto &energy : energy_map){
      for(int i = 0 ; i < 4 ; i++){
        delete energy[i];
        energy[i] = nullptr;
      }
    }
    for(auto &energy : energy_map_underground){
      for(int i = 0 ; i < 4 ; i++){
        delete energy[i];
        energy[i] = nullptr;
      }
    }
    energy_map = temporary_energy_map;
    energy_map_underground = temporary_energy_map_underground;
        
    
    for(auto &wire : wire_map_underground){
      if(wire){
        wire->CheckIfHasEnergy(energy_map_underground);
        
      }
    }
    
    for(auto &logic_gate : logic_gate_map){
      if(logic_gate){
        logic_gate->RunLogic(energy_map);
      }
    }

    for(int i = 0 ; i < wire_map.size() ; i++){
      Wire* wire = wire_map[i];
      if(wire){
        wire->CheckIfHasEnergy(energy_map);
        if(wire->type == kEditorObject_Wire_Underground_Exit){
          wire->CheckIfHasEnergy(energy_map_underground);
        }

         
        int direction_grid_position[4] = {
          i + 1,          //right
          i + map_size.y, //down
          i - 1,          //left,
          i - map_size.y //up
        };
        eDirection direction = wire->body->direction;
        if(wire_map[direction_grid_position[direction]] || logic_gate_map[direction_grid_position[direction]]){
          if(energy_map[direction_grid_position[direction]][direction]){
            if(energy_map[direction_grid_position[direction]][direction]->value < wire_map[i]->energy_value){
              energy_map[direction_grid_position[direction]][direction]->value = wire_map[i]->energy_value;
              std::cout << wire_map[i]->energy_value << std::endl;
            }
          }
        }
        if(wire_map_underground[direction_grid_position[direction]] && 
                wire_map_underground[direction_grid_position[direction]]->output_direction == direction){
            if(energy_map_underground[direction_grid_position[direction]][direction]->value < wire_map[i]->energy_value){
              energy_map_underground[direction_grid_position[direction]][direction]->value = wire_map[i]->energy_value;
            }

        }
      
      }
    }
    
        
    //we need a way to remove unconnected energy
    /*for(int i = 0 ; i < energy_map.size() ; i++){
      for(int j = 0 ; j < 4 ; j++){
        if(energy_map[i][j]){
          if(energy_map[i][j]->value <= 0){
           
            delete energy_map[i][j];
            energy_map[i][j] = nullptr;
          }
          else{
            
            energy_map[i][j]->Update();
            
          }
        }
      }
    }*/
    
  }
}

void 
GameData::TransmitEnergyValue() {

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
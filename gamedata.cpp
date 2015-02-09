#include <iostream>
#include "label.h"
#include "gamedata.h"
#include "engine.h"
#include "wire.h"
#include "and_gate.h"
#include "constant_1.h"
#include "not_gate.h"
#include "separator.h"
#include "save_gate_window.h"
#include "three_state.h"
#include "clock.h"
#include <memory>
#include <fstream>

int CELLS_SIZE = 22;
const int PIXEL_SIZE = 2;

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
,paste_mode(false)
,mouse_collide_with_object(false)
,brush_color(kColor_Black),
brush_mode(false),
create_label_mode(false),
temporary_label(nullptr){
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
  
  for(int i = 0 ; i < map_size_total ; ++i){
    color_map.push_back(kColor_None);
  }
  
  last_selected_toolbar_item.emplace(kKey_ToolBar1, kEditorObject_Wire);
  last_selected_toolbar_item.emplace(kKey_ToolBar3, kEditorObject_And);
  
  map_size_pixels.x = map_size.x * CELLS_SIZE;
  map_size_pixels.y = map_size.y * CELLS_SIZE;
}

void
GameData::InitializeWorld(const Veci& window_size) {
  //player = Player(Vecf{window_size.x/2.0f, window_size.y/2.0f});
}

void GameData::RenderColorSquare(Engine& engine, const eColor ecolor, 
        const int x, const int y) {
    SDL_Color color = {255,255,255,255};
    switch(ecolor){
      case kColor_Black:
        color = {0,0,0,255};
        break;
      case kColor_White:
       color = {255,255,255,255};
       break;
      case kColor_Green:
        color = {62,152,87,255};
        break;
      case kColor_Orange:
        color = {231,118,56,255};
        break;
      case kColor_LightBlue:
        color = {56,134,231,255};
        break;
      case kColor_Grey:
        color = {126,126,126,255};
        break;
      case kColor_DarkBlue:
        color = {52,71,184,255};
        break;
      case kColor_Pink:
        color = {179,65,208,255};
        break;
      case kColor_Red:
        color = {171,60,60,255};
        break;
    }
    
    SDL_SetRenderDrawColor(engine.renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect = { static_cast<int>(x), 
                      static_cast<int>(y), 
                      CELLS_SIZE, CELLS_SIZE};
    SDL_RenderFillRect(engine.renderer, &rect);
}

void 
GameData::Render(Engine& engine) {
  
  if(paste_mode){
    int x = 0;
    int y = 0;
    for(int i = 0 ; i < clipboard_color_map.size() ; i++){
    
    if(clipboard_color_map[i] != kColor_None){
      RenderColorSquare(engine, clipboard_color_map[i], 
              x * CELLS_SIZE - engine.camera.x + mouse_grid_position.x * CELLS_SIZE, 
              y * CELLS_SIZE - engine.camera.y + mouse_grid_position.y * CELLS_SIZE);
    }
    ++x;
    if(x == clipboard_size.x){
      x = 0;
      ++y;
    }
    }
  }
  
  int x = 0;
  int y = 0;
  for(int i = 0 ; i < color_map.size() ; i++){
    
    if(color_map[i] != kColor_None){
      RenderColorSquare(engine, color_map[i], 
              x * CELLS_SIZE  - engine.camera.x, 
              y * CELLS_SIZE  - engine.camera.y);
    }
    ++x;
    if(x == map_size.x){
      x = 0;
      ++y;
    }
  }
  
  /*for(auto &wire : wire_map_underground){  
    if(wire){
      wire->body->Render(engine);
    }
  }*/
  
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
  
  for(int i = 0 ; i < wire_map_underground.size() ; i++){
    if(wire_map_underground[i]){
        wire_map_underground[i]->body->Render(engine);
    }
  }
  
  player.body->Render(engine);
  
  if(temporary_gate){
    temporary_gate->body->Render(engine);
    
  }
  if(temporary_wire){
    temporary_wire->body->Render(engine);
  }
  
  if(brush_mode){
    RenderColorSquare(engine, brush_color, 
            mouse_grid_position.x * CELLS_SIZE - engine.camera.x,
            mouse_grid_position.y * CELLS_SIZE - engine.camera.y);

  }
  
  if(save_gate_window){
    save_gate_window->Render(engine);
  }
  
  if(paste_mode){
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
  
  if(create_label_mode){
    if(temporary_label){
      temporary_label->Render(engine);
    }
  }
  
  for(auto &label : labels){
      label.Render(engine);
      
  }
  
  int map_width = map_size.x * CELLS_SIZE;
  int map_height = map_size.y * CELLS_SIZE;
  
  SDL_Rect map_rect = {static_cast<int>(0 - engine.camera.x) ,
  static_cast<int>(0 - engine.camera.y), 
  map_width, map_height};
  SDL_SetRenderDrawColor(engine.renderer, 255, 0, 0, 255);
  SDL_RenderDrawRect(engine.renderer, &map_rect);
}

void GameData::CreateTemporaryObject(const eEditorObject object_type, const Vecf& position) {
  Clean();
  switch(object_type){
    case kEditorObject_And:
      temporary_gate = new AndGate(position, temporary_rotation, 0, map_size);
      break;
    case kEditorObject_ThreeState:
      temporary_gate = new ThreeState(position, temporary_rotation, 0, map_size);
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
    case kEditorObject_Wire_Underground:
      temporary_wire = new Wire(position, 0, temporary_rotation, kEditorObject_Wire_Underground);
      break;
    case kEditorObject_Clock:
      temporary_gate = new Clock(position, temporary_rotation, 0, map_size);
      break;
    default:
      //do nothing
      break;
  }
  
  
  if(temporary_gate){
    if(temporary_gate->body->bbox.left < 0 || 
            temporary_gate->body->bbox.right > map_size_pixels.x ||
            temporary_gate->body->bbox.top< 0 || 
            temporary_gate->body->bbox.bottom > map_size_pixels.y){
      delete temporary_gate;
      temporary_gate = nullptr;
    }
  }
  if(temporary_wire){
    if(temporary_wire->body->bbox.left < 0 || 
            temporary_wire->body->bbox.right > map_size_pixels.x ||
            temporary_wire->body->bbox.top< 0 || 
            temporary_wire->body->bbox.bottom > map_size_pixels.y){
      delete temporary_wire;
      temporary_wire = nullptr;
    }
  }
}

void 
GameData::BrushMode( const std::array<bool, kKey_Count>& keys_down, 
        const std::array<bool, 255 > & mouse_buttons_down, 
                        const Veci& mouse_position_in_window,
                        const Vecf& mouse_position_in_world,
                        const std::array<bool, kKey_Count>& last_keys_down,
        const std::array<bool, 255>& last_mouse_buttons_down) {
  
  if(keys_down[kKey_ToolBar1]){
    brush_color = kColor_Black;
  }
  if(keys_down[kKey_ToolBar2]){
    brush_color = kColor_Grey;
  }
  if(keys_down[kKey_ToolBar3]){
    brush_color = kColor_White;
  }
  if(keys_down[kKey_ToolBar4]){
    brush_color = kColor_Green;
  }
  if(keys_down[kKey_ToolBar5]){
    brush_color = kColor_LightBlue;
  }
  if(keys_down[kKey_ToolBar6]){
    brush_color = kColor_DarkBlue;
  }
  if(keys_down[kKey_ToolBar7]){
    brush_color = kColor_Orange;
  }
  if(keys_down[kKey_ToolBar8]){
    brush_color = kColor_Pink;
  }
  if(keys_down[kKey_ToolBar9]){
    brush_color = kColor_Red;
  }
  if(mouse_buttons_down[SDL_BUTTON_LEFT]){
    color_map[mouse_vector_position] = brush_color;
  }
  if(mouse_buttons_down[SDL_BUTTON_RIGHT]){
    color_map[mouse_vector_position] = kColor_None;
  }
  
  if((keys_down[kKey_Fill] || keys_down[kKey_Delete] )&& selected_area){
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

    int position_in_vector= 0;
    for(int y = selection_grid_position_begin.y ; 
                y < selection_grid_position_end.y; ++y){
      for(int x = selection_grid_position_begin.x ; 
                x < selection_grid_position_end.x ; ++x){

          position_in_vector = x + map_size.x * y;
          
          color_map[position_in_vector] = 
                  keys_down[kKey_Fill]? brush_color : kColor_None;
        }
    }
    
    selected_area = false;
  }
}

void 
GameData::Save(std::string file_name) {
  std::cout <<"saving" << std::endl;
  std::ofstream out(file_name, std::ios::trunc | std::ios::binary | std::ios::out);

  if(out.is_open())
  {
    out.seekp(std::ios_base::beg);
    std::cout << wire_map.size() << std::endl;
    out.write(reinterpret_cast<char *>(&map_size.x), sizeof(map_size.x));
    out.write(reinterpret_cast<char *>(&map_size.y), sizeof(map_size.y));

    for(int i = 0 ; i < wire_map.size() ; i++){
      if(!wire_map[i]) {
        bool is_object = false;
        out.write(reinterpret_cast<char *>(&is_object), sizeof(is_object));
      }
      else
      {
        Wire* object = wire_map[i];
        bool is_object = true;
        Vecf position = object->body->bbox.GetCoordinates();
        eDirection direction = object->body->direction;

        out.write(reinterpret_cast<char *>(&is_object), sizeof(is_object));
        out.write(reinterpret_cast<char *>(&position.x), sizeof(position.x));
        out.write(reinterpret_cast<char *>(&position.y), sizeof(position.y));
        out.write(reinterpret_cast<char *>(&direction), sizeof(direction));
      }
    }
    
    for(int i = 0 ; i < wire_map_underground.size() ; i++){
      if(!wire_map_underground[i]) {
        bool is_object = false;
        out.write(reinterpret_cast<char *>(&is_object), sizeof(is_object));
      }
      else
      {
        Wire* object = wire_map_underground[i];
        bool is_object = true;
        Vecf position = object->body->bbox.GetCoordinates();
        eDirection direction = object->body->direction;

        out.write(reinterpret_cast<char *>(&is_object), sizeof(is_object));
        out.write(reinterpret_cast<char *>(&position.x), sizeof(position.x));
        out.write(reinterpret_cast<char *>(&position.y), sizeof(position.y));
        out.write(reinterpret_cast<char *>(&direction), sizeof(direction));
      }
    }

    
    for(int i = 0 ; i < logic_gate_map.size() ; i++){
      if(!logic_gate_map[i]) {
        bool is_object = false;
        out.write(reinterpret_cast<char *>(&is_object), sizeof(is_object));
      }
      else
      {
        std::cout << "heh" << std::endl;
        LogicGate* object = logic_gate_map[i];
        bool is_object = true;
        Vecf position = object->body->bbox.GetCoordinates();
        eDirection direction = object->body->direction;
        eEditorObject type = object->object_type;

        out.write(reinterpret_cast<char *>(&is_object), sizeof(is_object));
        out.write(reinterpret_cast<char *>(&position.x), sizeof(position.x));
        out.write(reinterpret_cast<char *>(&position.y), sizeof(position.y));
        out.write(reinterpret_cast<char *>(&direction), sizeof(direction));
        out.write(reinterpret_cast<char *>(&type), sizeof(type));
        out.write(reinterpret_cast<char *>(&object->logical_state), sizeof(object->logical_state));
      }
    }
    
   
    /*Writing colors*/
    for(int i = 0 ; i < color_map.size() ; i++){
      eColor color = color_map[i];
      out.write(reinterpret_cast<char *>(&color), sizeof(color));
       
    }
     
     
     std::cout << "begin writing color" << out.tellp() << std::endl;
    /*Writing labels*/
    int size = labels.size();
    out.write(reinterpret_cast<char *>(&size), sizeof(size));
    for(int i = 0 ; i < size ; i++){
      Vecf position = labels[i].bbox.GetCoordinates();
      std::string text = labels[i].text;
      int string_size = text.size();
      out.write(reinterpret_cast<char *>(&position.x), sizeof(position.x));
      out.write(reinterpret_cast<char *>(&position.y), sizeof(position.y));
      out.write(reinterpret_cast<char *>(&string_size), sizeof(string_size));
      out.write(text.c_str(), text.size());
      std::cout << text.c_str() << std::endl;
    }
    
    std::cout << "end writing color" << out.tellp() << std::endl;
  } 
}

void 
GameData::Load(std::string file_name) {
  std::cout <<  "loading" << std::endl;
  std::ifstream in("quick_save.sav", std::ios::binary | std::ios::in);

  if(in.is_open())
  {
    in.read(reinterpret_cast<char *>(&map_size.x), sizeof(map_size.x));
    in.read(reinterpret_cast<char *>(&map_size.y), sizeof(map_size.y));

    /*READING WIRE MAP*/
    for(auto &i : wire_map){
      delete i;
      i = nullptr;
    }
    int total_map_size = map_size.x * map_size.y;
    wire_map.resize(total_map_size);
    for(int i = 0 ; i < total_map_size ; i++)
    {
      wire_map[i] = nullptr;
    }

    for(int i = 0 ; i < total_map_size ; i++)
    {  
      bool is_object = false;
      in.read((char*)&is_object, sizeof(is_object));
      if(is_object)
      {
        Vecf position = {0.0f, 0.0f};
        eDirection direction = kDirection_Count;

        in.read(reinterpret_cast<char *>(&position.x), sizeof(position.x));
        in.read(reinterpret_cast<char *>(&position.y), sizeof(position.y));
        in.read(reinterpret_cast<char *>(&direction), sizeof(direction));
        wire_map[i] = new Wire(position, i, direction, kEditorObject_Wire);
      }
    }
    
    /*READING UNDERGROUND WIRE MAP*/
    for(auto &i : wire_map_underground){
      delete i;
      i = nullptr;
    }
    wire_map_underground.resize(total_map_size);
    for(int i = 0 ; i < total_map_size ; i++)
    {
      wire_map_underground[i] = nullptr;
    }

    for(int i = 0 ; i < total_map_size ; i++)
    {  
      bool is_object = false;
      in.read((char*)&is_object, sizeof(is_object));
      if(is_object)
      {
        Vecf position = {0.0f, 0.0f};
        eDirection direction = kDirection_Count;

        in.read(reinterpret_cast<char *>(&position.x), sizeof(position.x));
        in.read(reinterpret_cast<char *>(&position.y), sizeof(position.y));
        in.read(reinterpret_cast<char *>(&direction), sizeof(direction));
        wire_map_underground[i] = new Wire(position, i, direction, 
                kEditorObject_Wire_Underground);
      }
    }

    /*READING LOGIC GATE MAP*/
    for(auto &i : logic_gate_map){
      delete i;
      i = nullptr;
    }
    logic_gate_map.resize(total_map_size);
    for(int i = 0 ; i < total_map_size ; i++)
    {
      logic_gate_map[i] = nullptr;
    }
    for(int i = 0 ; i < total_map_size ; i++){
      bool is_object = false;
      in.read(reinterpret_cast<char *>(&is_object), sizeof(is_object));

      if(is_object)
      {
        
        Vecf position = {0.0f, 0.0f};
        eDirection direction = kDirection_Count;
        eEditorObject type = kEditorObject_None;
        eLogicalState logical_state = kLogicalState_Empty;

        in.read(reinterpret_cast<char *>(&position.x), sizeof(position.x));
        in.read(reinterpret_cast<char *>(&position.y), sizeof(position.y));
        in.read(reinterpret_cast<char *>(&direction), sizeof(direction));
        in.read(reinterpret_cast<char *>(&type), sizeof(type));
        in.read(reinterpret_cast<char *>(&logical_state), sizeof(logical_state));
        
        switch(type){
          case kEditorObject_And:
            logic_gate_map[i] = new AndGate(position, direction, i, map_size);
            break;
          case kEditorObject_Not:
            logic_gate_map[i] = new NotGate(position, direction, i, map_size);
            break;
          case kEditorObject_Constant_1:
            logic_gate_map[i] = new Constant_1(position, direction, i, map_size);
            break;
          case kEditorObject_Separator:
            logic_gate_map[i] = new Separator(position, direction, i, map_size);
            break;
          case kEditorObject_ThreeState:
            logic_gate_map[i] = new ThreeState(position, direction, i, map_size);
            break;
          case kEditorObject_Clock:
            logic_gate_map[i] = new Clock(position, direction, i, map_size);
            break;
        }
        
        logic_gate_map[i]->logical_state = logical_state;
      }
    }
    
    color_map.resize(total_map_size);

    /*Reading colors*/
    for(int i = 0 ; i < total_map_size ; i++){
      eColor color = kColor_None;
      
      in.read(reinterpret_cast<char *>(&color), sizeof(color));
      color_map[i] = color;
    }

    
    //Reading labels
    labels.clear();
    int size = 0;
    
    in.read(reinterpret_cast<char *>(&size), sizeof(size));
    labels.resize(size);
    for(int i = 0 ; i < size ; i++){
      Vecf position = Vecf{0,0};
      int string_size = 0;
      in.read(reinterpret_cast<char *>(&position.x), sizeof(position.x));
      in.read(reinterpret_cast<char *>(&position.y), sizeof(position.y));
      in.read(reinterpret_cast<char *>(&string_size), sizeof(string_size));
      
      char c_string[string_size];
      
      in.read(c_string, sizeof(c_string));
      std::string text(c_string);
      std::string real_string(text, 0, string_size);//hack
      labels[i] = Label(position, real_string);    
    }
  }
  
}


Vecf 
GameData::ReceiveInput( std::string text_input, 
                        const std::array<bool, kKey_Count>& keys_down, 
                        const std::array<bool, 255 > & mouse_buttons_down, 
                        const Veci& mouse_position_in_window,
                        const Vecf& mouse_position_in_world,
                        const std::array<bool, kKey_Count>& last_keys_down,
        const std::array<bool, 255>& last_mouse_buttons_down) {
  pressed_rotate = false;
 // std::cout << color_map.size() << " " << color_map.capacity() << std::endl;
  if(keys_down[kKey_QuickSave] && !last_keys_down[kKey_QuickSave]){
    Save("quick_save.sav");
  } 
  
  if(keys_down[kKey_QuickLoad] && !last_keys_down[kKey_QuickLoad]){
    Load("quick_save.sav");
  }
  
  if(create_label_mode){
    temporary_label_position = mouse_world_position;
    if(temporary_label){
      temporary_label->bbox.MoveTo(mouse_world_position);
    }
    Clean();
    if(text_input.length() > 0){
      temporary_label_string.append(text_input);
      delete temporary_label;
      temporary_label = nullptr;
      temporary_label = new Label(temporary_label_position, temporary_label_string);
    }
    
    if(keys_down[kKey_Backspace] && !last_keys_down[kKey_Backspace]){
      if (temporary_label_string.size() > 0){
        temporary_label_string.pop_back();
      }
      
      delete temporary_label;
      temporary_label = nullptr;
      temporary_label = new Label(temporary_label_position, temporary_label_string);
    }
   
    if(keys_down[kKey_Enter] || mouse_buttons_down[SDL_BUTTON_LEFT]){
      labels.push_back(Label(temporary_label_position, temporary_label_string));
      temporary_label_string.clear();
      create_label_mode = false;
    }

    if(keys_down[kKey_CreateLabel] && !last_keys_down[kKey_CreateLabel]){
      create_label_mode = false;
      temporary_label_string.clear();
    }
  } else {
    
    if(keys_down[kKey_CreateLabel] && !last_keys_down[kKey_CreateLabel]){
      temporary_label_position = mouse_world_position;
      create_label_mode = true;
      temporary_label_string.clear();
    }
  }
  if(brush_mode && !create_label_mode){
    Clean();
    BrushMode(keys_down, mouse_buttons_down, 
              mouse_position_in_window,
              mouse_position_in_world,
              last_keys_down,
              last_mouse_buttons_down);
    
    if(keys_down[kKey_Brush] && !last_keys_down[kKey_Brush]){
      brush_mode = false;
    }
  }
  else{
    if(keys_down[kKey_Brush] && !last_keys_down[kKey_Brush]){
      brush_mode = true;
    }
  }
  
  if(keys_down[kKey_Zoom_Out] &&
          !last_keys_down[kKey_Zoom_Out]){
  CELLS_SIZE -= 1;
  
    for(auto &i : wire_map){
      if(i){
        i->body->bbox.Resize(Veci{CELLS_SIZE, CELLS_SIZE});
      }
    }
  }

  if(keys_down[kKey_Speed_Down] &&
          !last_keys_down[kKey_Speed_Down]){
    if(update_delay < 1000){
      update_delay += 16;
      std::cout << "Update delay now " << update_delay << " ms." << std::endl;
    }
  }
  if(keys_down[kKey_Speed_Up] &&
          !last_keys_down[kKey_Speed_Up]){
    if(update_delay >= 16){
      update_delay -= 16;
      std::cout << "Update delay now " << update_delay << " ms." << std::endl;
    }
  }

  CheckItemSelectionKeys(keys_down, last_keys_down);
  UpdateMousePosition(mouse_position_in_world);
  mouse_collide_with_object = false;
 

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
    DeleteSelection();
  }
  
  if(selected_area && keys_down[kKey_Copy] && !last_keys_down[kKey_Copy]){
    CopySelectionToClipboard();
  }
  
  if(keys_down[kKey_Paste] && !last_keys_down[kKey_Paste]){
    paste_mode = true;
    selected_area = false;
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
  
  if(paste_mode  && !create_label_mode){
    
    Clean();
    MoveClipboardObjects();
    if(mouse_buttons_down[SDL_BUTTON_LEFT]){
      PasteClipboardObjects();
    } 
  } else if(!paste_mode && !brush_mode && !create_label_mode){
  CreateTemporaryObject(currently_selected_object, grid_position_position);
 
    
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
              currently_selected_object == kEditorObject_Wire_Underground)){
        
        CreateLineOfWires();
      }

      if((keys_down[kKey_Rotate_Left] && !last_keys_down[kKey_Rotate_Left]) || 
              (mouse_buttons_down[kKey_Mouse_Rotate_Left] && 
              !last_mouse_buttons_down[kKey_Mouse_Rotate_Left])){
       
        temporary_rotation = (eDirection)(temporary_rotation - 1);
        if(temporary_rotation < 0){
          temporary_rotation = (eDirection)(kDirection_Count - 1);
        }
        pressed_rotate = true;
        
      }
      if((keys_down[kKey_Rotate_Right] && !last_keys_down[kKey_Rotate_Right]) || 
              (mouse_buttons_down[kKey_Mouse_Rotate_Right] && 
              !last_mouse_buttons_down[kKey_Mouse_Rotate_Right])){
        temporary_rotation = (eDirection)(temporary_rotation + 1);
        if(temporary_rotation >= (int)kDirection_Count){
          temporary_rotation = (eDirection)0;
        }
        pressed_rotate = true;
      }      
        
      CheckMouseObjectsCollision(keys_down, mouse_buttons_down, 
                      mouse_position_in_window,
                      mouse_position_in_world,
                      last_keys_down,
                      last_mouse_buttons_down);
      

      if(!mouse_collide_with_object && 
              !making_line_of_wires && 
              (temporary_gate || temporary_wire) &&
              mouse_buttons_down[SDL_BUTTON_LEFT]){
        CreateSingleObject();
      }


      if(mouse_buttons_down[SDL_BUTTON_RIGHT]){ 
        DeleteSingleObject(mouse_position_in_world);
      }
      
      
    }//if !selecting_area
  }//if wanting to paste
  
  Vecf player_movement = {0,0};
  
  if(!create_label_mode){
    
    player_movement = player.ReceiveInput(keys_down, mouse_buttons_down);
    
  }
  
  
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
  
  update_timer += g_delta_t * 1000;
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
            if(wire_map_underground[direction_grid_position[direction]] &&
                    wire_map_underground[direction_grid_position[direction]]->output_direction == wire->output_direction){
                temporary_energy_map_underground[direction_grid_position[direction]][direction] = 
                        new Energy(wire->logical_state, wire->energy_value);
            } else {
              if(wire_map[direction_grid_position[direction]] &&
                      wire_map[direction_grid_position[direction]]->output_direction == wire->output_direction){
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
        /*if(wire->type == kEditorObject_Wire_Underground_Exit){
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
            }
          }
        }
        if(wire_map_underground[direction_grid_position[direction]] && 
                wire_map_underground[direction_grid_position[direction]]->output_direction == direction){
            if(energy_map_underground[direction_grid_position[direction]][direction]->value < wire_map[i]->energy_value){
              energy_map_underground[direction_grid_position[direction]][direction]->value = wire_map[i]->energy_value;
            }

        }*/
      
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

void 
GameData::DeleteSelection() {
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

void 
GameData::CopySelectionToClipboard() {
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
  int vector_size = size_selection.x * size_selection.y;
  for(int i = 0 ; i < vector_size ; ++i){
    clipboard_gates.push_back(nullptr);
    clipboard_wires.push_back(nullptr);
    clipboard_color_map.push_back(kColor_None);
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
          
        clipboard_color_map[position_in_vector_of_clipboard_map] = 
                color_map[position_in_vector_of_real_map];
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
              case kEditorObject_ThreeState:
                clipboard_gates[position_in_vector_of_clipboard_map] = 
                   new ThreeState(Vecf{gate->body->bbox.left, gate->body->bbox.top},
                        gate->body->direction, position_in_vector_of_clipboard_map, 
                           map_size);
              case kEditorObject_Clock:
                clipboard_gates[position_in_vector_of_clipboard_map] = 
                   new Clock(Vecf{gate->body->bbox.left, gate->body->bbox.top},
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

void GameData::PasteClipboardObjects() {
  for(int y = 0 ; y < clipboard_size.y ; y++){
    for(int x = 0 ; x < clipboard_size.x ; x++){
      int vector_pos = x + y * clipboard_size.x;
            int vector_pos_in_real_map = 
        mouse_grid_position.x + mouse_grid_position.y * map_size.x +
        x + y * map_size.x;
            
      if(mouse_grid_position.x + x < map_size.x &&
              mouse_grid_position.x + x > 0 &&
              mouse_grid_position.y + y < map_size.y &&
              mouse_grid_position.y + y > 0)
      {
        if(clipboard_color_map[vector_pos] != kColor_None){
          color_map[vector_pos_in_real_map] = clipboard_color_map[vector_pos];
        }
      
      
        if(!brush_mode){
          if(clipboard_gates[vector_pos]){
            LogicGate* gate = clipboard_gates[vector_pos];

            delete logic_gate_map[vector_pos_in_real_map];
            logic_gate_map[vector_pos_in_real_map] = nullptr;

            delete wire_map[vector_pos_in_real_map];
            wire_map[vector_pos_in_real_map] = nullptr;
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
                case kEditorObject_ThreeState:
                logic_gate_map[vector_pos_in_real_map] = 
                  new ThreeState(Vecf{gate->body->bbox.left, gate->body->bbox.top},
                        gate->body->direction, vector_pos_in_real_map, 
                           map_size);;
                break;
              case kEditorObject_Separator:
                logic_gate_map[vector_pos_in_real_map] = 
                   new Separator(Vecf{gate->body->bbox.left, gate->body->bbox.top},
                        gate->body->direction, vector_pos_in_real_map, 
                           map_size);
              case kEditorObject_Clock:
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
              delete wire_map_underground[vector_pos_in_real_map];
              wire_map_underground[vector_pos_in_real_map] = nullptr;
              delete logic_gate_map[vector_pos_in_real_map];
              logic_gate_map[vector_pos_in_real_map] = nullptr;
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

          }
        }
      }
    }
  }
}

void 
GameData::MoveClipboardObjects(){
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

void GameData::CreateLineOfWires() {

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
    
    if(real_position.x > 0 && 
            real_position.x < map_size_pixels.x &&
            real_position.y > 0 && 
            real_position.y < map_size_pixels.y && temporary_wire){
      if(!logic_gate_map[position_in_vector] || temporary_wire->type != kEditorObject_Wire){
        if(temporary_wire->type == kEditorObject_Wire){
          temporary_wire_map_blueprints[position_in_vector] = new Wire(real_position,
                position_in_vector, temporary_rotation, temporary_wire->type);
        } else{
          temporary_wire_map_blueprints[position_in_vector] = new Wire(real_position,
                position_in_vector, temporary_rotation, kEditorObject_Wire_Underground);
        }
      }
    }
  }
  Clean();
}

void GameData::CheckMouseObjectsCollision(const std::array<bool, kKey_Count>& keys_down,
        const std::array<bool, 255 > & mouse_buttons_down, 
                        const Veci& mouse_position_in_window,
                        const Vecf& mouse_position_in_world,
                        const std::array<bool, kKey_Count>& last_keys_down,
        const std::array<bool, 255>& last_mouse_buttons_down) {
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
      
      if(gate->body->bbox.CollisionWithPoint(mouse_position_in_world)) {
        mouse_collide_with_object = true;
        if(pressed_rotate) {
            gate->Rotate(temporary_rotation, map_size);
        }
        Clean();
      }
    }
  }
  for(auto &wire : wire_map) {
    if(wire) {
      if(wire->body->bbox.CollisionWithPoint(mouse_position_in_world)) {
        mouse_collide_with_object = true;
        Clean();
        if(pressed_rotate) {
          wire->Rotate(temporary_rotation);
        }
      }
    }
  }
  
  for(auto i = labels.begin() ; i < labels.end() ;){
    bool erase = false;
    if(i->bbox.CollisionWithPoint((mouse_position_in_world))){
      //mouse_collide_with_object = true;
      //Clean();
      
      if(keys_down[kKey_Delete]){
        labels.erase(i);
        erase = true;
      }
    }
    if(!erase){
      ++i;
    }
  }
}

void 
GameData::CreateSingleObject() {

  if(currently_selected_object == kEditorObject_And) {      
  logic_gate_map[mouse_vector_position]= new AndGate(grid_position_position, temporary_rotation,
           mouse_vector_position, map_size);
  } else if(currently_selected_object == kEditorObject_Constant_1){
    logic_gate_map[mouse_vector_position]= new Constant_1(grid_position_position, temporary_rotation,
           mouse_vector_position, map_size);
  } else if(currently_selected_object == kEditorObject_Not){
    logic_gate_map[mouse_vector_position]= new NotGate(grid_position_position, temporary_rotation,
           mouse_vector_position, map_size);
  } else if(currently_selected_object == kEditorObject_Separator){
    logic_gate_map[mouse_vector_position]= new Separator(grid_position_position, temporary_rotation,
           mouse_vector_position, map_size);
  } else if(currently_selected_object == kEditorObject_ThreeState){
    logic_gate_map[mouse_vector_position]= new ThreeState(grid_position_position, temporary_rotation,
           mouse_vector_position, map_size);
  } else if(currently_selected_object == kEditorObject_Clock){
    logic_gate_map[mouse_vector_position]= new Clock(grid_position_position, temporary_rotation,
           mouse_vector_position, map_size);
  }
  else if(currently_selected_object == kEditorObject_Wire) {
    if(temporary_wire && temporary_wire->type == kEditorObject_Wire){
      delete wire_map[mouse_vector_position];
      wire_map[mouse_vector_position] = nullptr;
      wire_map[mouse_vector_position] = new Wire(grid_position_position,
                mouse_vector_position, temporary_rotation, temporary_wire->type);
    }
  }
}

void 
GameData::DeleteSingleObject(const Vecf& mouse_position_in_world) {
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

void 
GameData::CheckItemSelectionKeys(const std::array<bool, kKey_Count>& keys_down,
        const std::array<bool, kKey_Count>& last_keys_down) {
        
  bool pressed_object_selection_key = false;

  if(keys_down[kKey_ToolBar4] && !last_keys_down[kKey_ToolBar4]){
    currently_selected_object = kEditorObject_ThreeState;
    pressed_object_selection_key = true;
  }

  if(keys_down[kKey_ToolBar5] && !last_keys_down[kKey_ToolBar5]){
    currently_selected_object = kEditorObject_Constant_1;
    pressed_object_selection_key = true;
  }
  
  if(keys_down[kKey_ToolBar6] && !last_keys_down[kKey_ToolBar6]){
    currently_selected_object = kEditorObject_Clock;
    pressed_object_selection_key = true;
  }
  
  if(keys_down[kKey_ToolBar1] && !last_keys_down[kKey_ToolBar1]){
    
    if(currently_selected_object != kEditorObject_Wire &&
            currently_selected_object != kEditorObject_Wire_Underground)
    {
      currently_selected_object = last_selected_toolbar_item.at(kKey_ToolBar1);
    }else{
      if(currently_selected_object == kEditorObject_Wire){
        currently_selected_object = kEditorObject_Wire_Underground;
      }else{
        currently_selected_object = kEditorObject_Wire;
      }
      last_selected_toolbar_item.at(kKey_ToolBar1) = currently_selected_object;
    }
    pressed_object_selection_key = true;
  }
      
  if(keys_down[kKey_ToolBar2] && !last_keys_down[kKey_ToolBar2]){
    currently_selected_object = kEditorObject_Separator;
    pressed_object_selection_key = true;
  }

  if(keys_down[kKey_ToolBar3] && !last_keys_down[kKey_ToolBar3]){
    if(currently_selected_object != kEditorObject_And &&
            currently_selected_object != kEditorObject_Not)
    {
      currently_selected_object = last_selected_toolbar_item.at(kKey_ToolBar3);
    }else{
      if(currently_selected_object == kEditorObject_And){
        currently_selected_object = kEditorObject_Not;
      }else{
        currently_selected_object = kEditorObject_And;
      }
      last_selected_toolbar_item.at(kKey_ToolBar3) = currently_selected_object;
    }
    pressed_object_selection_key = true;
  }
 
      
  if(pressed_object_selection_key){
    paste_mode = false;
  }
}

void 
GameData::UpdateMousePosition(const Vecf& mouse_position_in_world) {
  mouse_world_position = mouse_position_in_world;
  mouse_grid_position = {static_cast<int>(mouse_position_in_world.x)/CELLS_SIZE,
                         static_cast<int>(mouse_position_in_world.y)/CELLS_SIZE};
  
  grid_position_position = { static_cast<float>(mouse_grid_position.x) * CELLS_SIZE,  
                             static_cast<float>(mouse_grid_position.y) * CELLS_SIZE };
  
  mouse_vector_position = mouse_grid_position.x + mouse_grid_position.y * map_size.y;
}

#include <iostream>
#include "wire.h"
#include "solid_body.h"
#include "energy.h"

#include "static_body.h"
#include "logicgate.h"
Wire::Wire(const Vecf& position, const int position_in_array, 
        const eDirection direction, const eEditorObject type) :
body(new StaticBody(position, Veci{CELLS_SIZE,CELLS_SIZE}, kTexture_Wire_Empty, direction)),
parent(nullptr),
logical_state(kLogicalState_Empty),
position_in_array(position_in_array),
visited(false),
type(type),
output_direction(direction),
energy_value(100){
  switch(type){
    case kEditorObject_Wire:
      body->sprite.texture_id = kTexture_Wire_Empty;
      break;
    case kEditorObject_Wire_Underground:
      body->sprite.texture_id = kTexture_Wire_Underground;
      break;
    case kEditorObject_Wire_Underground_Exit:
      body->sprite.texture_id = kTexture_Wire_Underground_Exit;
      break;
  }
}

Wire::~Wire(){
  delete body;
}
void Wire::Rotate() {
  
}

void 
Wire::CheckIfHasEnergy(std::vector<std::array<Energy*, 4>>& energy_map) {
  //check which energy types are present on the cell
  //if there are multiple types (ie 0 and 1), then we choose one at random...
  
  //std::array<Energy*, 4>& energy_array = energy_map[position_in_array];
  bool has_high_energy = false;
  bool has_low_energy = false;
  int highest_energy = 0;
  
  for(int i = 0 ; i < 4 ; i++) {
    if(energy_map[position_in_array][i]) {
      
      
      energy_value = energy_map[position_in_array][i]->value; 
      if(energy_map[position_in_array][i]->state == kLogicalState_0) {
        has_low_energy = true;
      }
      if(energy_map[position_in_array][i]->state == kLogicalState_1) {
        has_high_energy = true;
      }
      if(energy_map[position_in_array][i]->state == kLogicalState_Error) {
        has_high_energy = true;
        has_low_energy = true;
      }
    }
  }
  
  
  if(has_high_energy && !has_low_energy) {
    logical_state = kLogicalState_1;
  }
  else if(!has_high_energy && has_low_energy) {
    logical_state = kLogicalState_0;
  }
  else if(has_high_energy && has_low_energy) {
    logical_state = kLogicalState_Error;
  }
  else if(!has_high_energy && ! has_low_energy) {
    logical_state = kLogicalState_Empty;
  }
  
  if(type == kEditorObject_Wire){
    switch(logical_state){
      case kLogicalState_0:
        body->sprite.texture_id = kTexture_Wire_0;
        break;
      case kLogicalState_1:
        body->sprite.texture_id = kTexture_Wire_1;
        break;
      case kLogicalState_Empty:
        body->sprite.texture_id = kTexture_Wire_Empty;
        break;
      case kLogicalState_Error:
        body->sprite.texture_id = kTexture_Wire_Error;
        break;
      default:
        std::cout << "Error, unknown state." << std::endl;
        break;
    }
  }
    
  if(type == kEditorObject_Wire_Underground){
    switch(logical_state){
      case kLogicalState_0:
        body->sprite.texture_id = kTexture_Wire_Underground_0;
        break;
      case kLogicalState_1:
        body->sprite.texture_id = kTexture_Wire_Underground_1;
        break;
      case kLogicalState_Empty:
        body->sprite.texture_id = kTexture_Wire_Underground;
        break;
      case kLogicalState_Error:
        body->sprite.texture_id = kTexture_Wire_Underground;
        break;
      default:
        std::cout << "Error, unknown state." << std::endl;
        break;
    }
  }
}


void 
Wire::MoveElectrons(std::vector<Wire*>& wire_map, const Veci& map_size) {
  Wire* wire_to_send_to = nullptr;
  switch(body->direction){
    case kDirection_Right:
      if(position_in_array + 1 < wire_map.size()) {
        wire_to_send_to = wire_map[position_in_array + 1];
      }
    case kDirection_Up:
      if(position_in_array - map_size.x > 0) {
        wire_to_send_to = wire_map[position_in_array - map_size.x];
      }
      break;
    case kDirection_Left:
      if(position_in_array - 1 < wire_map.size()) {
        wire_to_send_to = wire_map[position_in_array - 1];
      }
      break;
    case kDirection_Down:
      if(position_in_array + map_size.x < wire_map.size()) {
        wire_to_send_to = wire_map[position_in_array + map_size.x];
      }
      break;
  }
  
  if(wire_to_send_to){
    
  }
}

void 
Wire::ChangeState(Wire* parent, eLogicalState state, 
        std::vector<Wire*>& wire_map,
        const Veci& map_size) {
  logical_state = state;
  /*
  visited = true;
  parent = parent;
  logical_state = state;
  switch(state){
    case kLogicalState_0:
      body->sprite.texture_id = kTexture_Wire_0;
      break;
    case kLogicalState_1:
      body->sprite.texture_id = kTexture_Wire_1;
      break;
    case kLogicalState_Empty:
      body->sprite.texture_id = kTexture_Wire_Empty;
      break;
    default:
      std::cout << "Error, unknown state." << std::endl;
      break;
  }
  
  Wire* wire_right = nullptr;
  Wire* wire_up = nullptr;
  Wire* wire_left = nullptr;
  Wire* wire_down = nullptr;
  switch(body->direction){
    case kDirection_Right:
      if(position_in_array + 1 < wire_map.size()) {
        wire_right = wire_map[position_in_array + 1];
      }
      if(wire_right && !wire_right->visited && wire_right != parent){
        wire_right->ChangeState(this, logical_state, wire_map, map_size);        
      }
      break;
    case kDirection_Up:
      if(position_in_array - map_size.x > 0) {
        wire_up = wire_map[position_in_array - map_size.x];
      }
      if(wire_up && !wire_up->visited && wire_up != parent){
        wire_up->ChangeState(this, logical_state, wire_map, map_size);        
      }
      break;
    case kDirection_Left:
      if(position_in_array - 1 < wire_map.size()) {
        wire_left = wire_map[position_in_array - 1];
      }
      if(wire_left && !wire_left->visited && wire_left != parent){
        wire_left->ChangeState(this, logical_state, wire_map, map_size);        
      }
      break;
    case kDirection_Down:
      if(position_in_array + map_size.x < wire_map.size()) {
        wire_down = wire_map[position_in_array + map_size.x];
      }
      if(wire_down && !wire_down->visited && wire_down != parent){
        wire_down->ChangeState(this, logical_state, wire_map, map_size);        
      }
      break;
    
  }
 */
  
}

void 
Wire::CheckOutputToWire(std::vector<Wire*>& wire_map) {
  
}

void 
Wire::ResetState() {
  visited = false;
  parent = nullptr;
  logical_state = kLogicalState_Empty;
  body->sprite.texture_id = kTexture_Wire_Empty;;
}

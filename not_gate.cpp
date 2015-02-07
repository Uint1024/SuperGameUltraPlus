#include <iostream>
#include "not_gate.h"
#include "energy.h"
NotGate::NotGate(const Vecf& position, const eDirection direction, 
        const int position_in_array, const Veci& map_size):
LogicGate(position, direction, position_in_array, map_size, kTexture_Not, 
        Veci{CELLS_SIZE,CELLS_SIZE}, kEditorObject_Not) {
  Rotate(direction, map_size);
}

void NotGate::Rotate(const eDirection direction, const Veci& map_size) {
  body->direction = direction;
  switch(direction){
    case kDirection_Down:
      output_position_in_map_grid[0] = position_in_map_grid + map_size.x;
      break;
    case kDirection_Left:
      output_position_in_map_grid[0] = position_in_map_grid - 1 ;
      break;
    case kDirection_Right:
      output_position_in_map_grid[0] = position_in_map_grid + 1;
      break;
    case kDirection_Up:
      output_position_in_map_grid[0] = position_in_map_grid - map_size.x;
      break;
  }
  
  input_position_in_map_grid[0] = position_in_map_grid;
}


void 
NotGate::RunLogic(std::vector<std::array<Energy*, 4>>& energy_map) {
  bool has_high_energy = false;
  bool has_low_energy = false;
  bool error = false;
  for(int i = 0 ; i < 4 ; i++) {
    if(energy_map[input_position_in_map_grid[0]][i]) {
      energy_value = energy_map[input_position_in_map_grid[0]][i]->value;
      if(energy_map[input_position_in_map_grid[0]][i]->state == kLogicalState_0) {
        has_low_energy = true;
      }
      if(energy_map[input_position_in_map_grid[0]][i]->state == kLogicalState_1) {
        
        has_high_energy = true;
      }
      if(energy_map[input_position_in_map_grid[0]][i]->state == kLogicalState_Error) {
        
        error = true;
      }
    }
  }
  
  if(has_high_energy && !has_low_energy) {
    logical_state = kLogicalState_0;
  }
  else if(!has_high_energy && has_low_energy) {
    logical_state = kLogicalState_1;
  }
  else if(has_high_energy && has_low_energy) {
    logical_state = kLogicalState_Error;
    
  }
  else if(!has_high_energy && !has_low_energy) {
    logical_state = kLogicalState_Empty;
  } 
  else if(error){
    logical_state = kLogicalState_Error;
  }

  for(int i = 0 ; i < 4 ; i++) {
    delete energy_map[input_position_in_map_grid[0]][i];
    energy_map[input_position_in_map_grid[0]][i] = nullptr;
  }
  
  switch(logical_state){
    case kLogicalState_0:
      body->sprite.texture_id = kTexture_Not_0;
      break;
    case kLogicalState_1:
      body->sprite.texture_id = kTexture_Not_1;
      break;
    default:
      body->sprite.texture_id = kTexture_Not;
      break;
  }
      
}


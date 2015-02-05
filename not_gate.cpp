#include <iostream>
#include "not_gate.h"
#include "energy.h"
NotGate::NotGate(const Vecf& position, const eDirection direction, 
        const int position_in_array, const Veci& map_size):
LogicGate(position, direction, position_in_array, map_size, kTexture_Not, Veci{16,16}) {
  switch(direction){
    case kDirection_Down:
      output_position_in_map_grid[0] = position_in_array + map_size.x;
      break;
    case kDirection_Left:
      output_position_in_map_grid[0] = position_in_array - 1 ;
      break;
    case kDirection_Right:
      output_position_in_map_grid[0] = position_in_array + 1;
      break;
    case kDirection_Up:
      output_position_in_map_grid[0] = position_in_array - map_size.x;
      break;
  }
  
  input_position_in_map_grid[0] = position_in_array;
}

void 
NotGate::RunLogic(std::vector<std::array<Energy*, 4>>& energy_map) {
  bool has_high_energy = false;
  bool has_low_energy = false;
  
  for(int i = 0 ; i < 4 ; i++) {
    if(energy_map[input_position_in_map_grid[0]][i]) {

      if(energy_map[input_position_in_map_grid[0]][i]->state == kLogicalState_0) {
        has_low_energy = true;
      }
      if(energy_map[input_position_in_map_grid[0]][i]->state == kLogicalState_1) {
        
        has_high_energy = true;
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

  for(int i = 0 ; i < 4 ; i++) {
    delete energy_map[input_position_in_map_grid[0]][i];
    energy_map[input_position_in_map_grid[0]][i] = nullptr;
  }
  
}


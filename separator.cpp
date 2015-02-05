#include <iostream>
#include "separator.h"
#include "and_gate.h"
#include "energy.h"

Separator::Separator(const Vecf& position,
        const eDirection direction, const int position_in_array, 
        const Veci& map_size) :
LogicGate(position, direction, position_in_array, map_size, kTexture_Separator,
        Veci{16,16}){
    input_position_in_map_grid[0] = position_in_array;
    if(direction == kDirection_Down) {
      output_position_in_map_grid[0] = position_in_array - 1;
      output_position_in_map_grid[1] = position_in_array + 1;
    }
    if(direction == kDirection_Left) {
      output_position_in_map_grid[0] = position_in_array - map_size.x;
      output_position_in_map_grid[1] = position_in_array + map_size.x;
    }
    if(direction == kDirection_Right) {
            output_position_in_map_grid[0] = position_in_array - map_size.x;
      output_position_in_map_grid[1] = position_in_array + map_size.x;
    }
    if(direction == kDirection_Up) {
      output_position_in_map_grid[0] = position_in_array - 1;
      output_position_in_map_grid[1] = position_in_array + 1;
    }
}

void Separator::RunLogic(std::vector<std::array<Energy*, 4>>& energy_map) {
  int high_energy = 0;
  int low_energy = 0;
  bool error = false;
  
  for(int i = 0 ; i < 4 ; i++) {
    if(energy_map[input_position_in_map_grid[0]][i]) {
      if(energy_map[input_position_in_map_grid[0]][i]->state == kLogicalState_0) {
        ++low_energy;
      }
      if(energy_map[input_position_in_map_grid[0]][i]->state == kLogicalState_1) {
         
        ++high_energy;
      }
      if(energy_map[input_position_in_map_grid[0]][i]->state == kLogicalState_Error) {
        error = true;
      }
    }
  }
  
  if(low_energy > 0 && high_energy == 0){
    logical_state = kLogicalState_0;
  }
  else if(low_energy == 0 && high_energy == 0){
    logical_state = kLogicalState_1;
  }
  else if((low_energy > 0 && high_energy > 0) || error){
    logical_state == kLogicalState_Error;
  }
}


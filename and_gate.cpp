#include <iostream>
#include "and_gate.h"
#include "energy.h"

AndGate::AndGate(const Vecf& position,
        const eDirection direction, const int position_in_array, 
        const Veci& map_size) :
LogicGate(position, direction, position_in_array, map_size, kTexture_And, Veci{16,16}){
  input_position_in_map_grid[0] = position_in_array;
  input_position_in_map_grid[1] = position_in_array;
    
  if(direction == kDirection_Down){
    output_position_in_map_grid[0] = position_in_array + map_size.x; 
  }
  if(direction == kDirection_Left){
    
    output_position_in_map_grid[0] = position_in_array - 1 ;
  }
  if(direction == kDirection_Right){
    output_position_in_map_grid[0] = position_in_array + 1 ;
  }
  if(direction == kDirection_Up){
    output_position_in_map_grid[0] = position_in_array - map_size.x;
  }
}

void AndGate::RunLogic(std::vector<std::array<Energy*, 4>>& energy_map) {
  bool has_high_energy = false;
  bool has_low_energy = false;
  
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
  std::cout << "high = " << high_energy << std::endl;
 
  
  if(!error){
    if(low_energy > 0) {
      logical_state = kLogicalState_0;
    }
    if(high_energy >= 2 && low_energy == 0) {
      logical_state = kLogicalState_1;
    }
    else if(high_energy == 0 && low_energy == 0) {
      logical_state = kLogicalState_Empty;
    } 
    else if(high_energy == 1 && low_energy == 0) {
      logical_state = kLogicalState_0;
    } 
  }
  else {
    logical_state == kLogicalState_Error;
  }

  for(int i = 0 ; i < 4 ; i++) {
    delete energy_map[input_position_in_map_grid[0]][i];
    energy_map[input_position_in_map_grid[0]][i] = nullptr;
  }
  
  /*if(!energy_map[input_position_in_map_grid[0]] &&
          !energy_map[input_position_in_map_grid[1]]){
    logical_state = kLogicalState_Empty;
  }
  else if(energy_map[input_position_in_map_grid[0]] &&
          energy_map[input_position_in_map_grid[1]]) {
    if(energy_map[input_position_in_map_grid[0]]->state == kLogicalState_1 &&
          energy_map[input_position_in_map_grid[1]]->state == kLogicalState_1) {
      logical_state = kLogicalState_1;
    }
    else {
      logical_state = kLogicalState_0;
    }
  }
  
  if((energy_map[input_position_in_map_grid[0]] &&
          !energy_map[input_position_in_map_grid[1]]) ||
     (!energy_map[input_position_in_map_grid[0]] &&
          energy_map[input_position_in_map_grid[1]])){
    std::cout << "heh" << std::endl;
            
    logical_state = kLogicalState_0;
  }

  delete energy_map[input_position_in_map_grid[0]];
  energy_map[input_position_in_map_grid[0]] = nullptr;
  delete energy_map[input_position_in_map_grid[1]];
  energy_map[input_position_in_map_grid[1]] = nullptr;*/
}

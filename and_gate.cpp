#include <iostream>
#include "and_gate.h"
#include "energy.h"

AndGate::AndGate(const Vecf& position,
        const eDirection direction, const int position_in_array, 
        const Veci& map_size) :
LogicGate(position, direction, position_in_array, map_size, kTexture_And, 
        Veci{CELLS_SIZE,CELLS_SIZE}, kEditorObject_And){
  Rotate(direction, map_size);
}

void AndGate::Rotate(const eDirection direction, const Veci& map_size) {
  body->direction = direction;
  input_position_in_map_grid[0] = position_in_map_grid;
  input_position_in_map_grid[1] = position_in_map_grid;
    
  if(direction == kDirection_Down){
    output_position_in_map_grid[0] = position_in_map_grid + map_size.x; 
  }
  if(direction == kDirection_Left){
    
    output_position_in_map_grid[0] = position_in_map_grid - 1 ;
  }
  if(direction == kDirection_Right){
    output_position_in_map_grid[0] = position_in_map_grid + 1 ;
  }
  if(direction == kDirection_Up){
    output_position_in_map_grid[0] = position_in_map_grid - map_size.x;
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

 
  
  if(!error){
    if(low_energy >= 1 && high_energy == 0) {
      logical_state = kLogicalState_0;
    }
    else if(high_energy >= 2 && low_energy == 0) {
      logical_state = kLogicalState_1;
    }
    else if(high_energy > 0 && low_energy > 0) {
      logical_state = kLogicalState_0;
    }
    else if(high_energy == 0 && low_energy == 0) {
      logical_state = kLogicalState_Empty;
    } 
    else if(high_energy == 1 && low_energy == 0) {
      logical_state = kLogicalState_0;
    } 
    else if(high_energy == 0 && low_energy == 1) {
      logical_state = kLogicalState_0;
    } 
  }
  else {
    logical_state == kLogicalState_Error;
  }
  
  switch(logical_state){
    case kLogicalState_0:
      body->sprite.texture_id = kTexture_And_0;
      break;
    case kLogicalState_1:
      body->sprite.texture_id = kTexture_And_1;
      break;
    default:
      body->sprite.texture_id = kTexture_And;
      break;
  }

  for(int i = 0 ; i < 4 ; i++) {
    delete energy_map[input_position_in_map_grid[0]][i];
    energy_map[input_position_in_map_grid[0]][i] = nullptr;
  }
}

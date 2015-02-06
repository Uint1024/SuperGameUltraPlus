#include "constant_1.h"

Constant_1::Constant_1(const Vecf& position, const eDirection direction, 
        const int position_in_array, const Veci& map_size) :
LogicGate(position, direction, position_in_array, map_size, kTexture_Constant_1,
        Veci{CELLS_SIZE,CELLS_SIZE}, kEditorObject_Constant_1) {
  output_position_in_map_grid[0] = position_in_array + map_size.x;
  logical_state = kLogicalState_1;
}

void 
Constant_1::RunLogic(std::vector<std::array<Energy*, 4>>& energy_map) {
   switch(logical_state){
    case kLogicalState_0:
      body->sprite.texture_id = kTexture_Constant_0;
      break;
    case kLogicalState_1:
      body->sprite.texture_id = kTexture_Constant_1;
      break;
    default:
      body->sprite.texture_id = kTexture_Constant_0;
      break;
  }
}

void Constant_1::Rotate(const eDirection direction, const Veci& map_size) {
  output_position_in_map_grid[0] = position_in_map_grid + map_size.x;
}

#include "constant_1.h"

Constant_1::Constant_1(const Vecf& position, const eDirection direction, 
        const int position_in_array, const Veci& map_size) :
LogicGate(position, direction, position_in_array, map_size, kTexture_Constant_1,
        Veci{16,32}) {
  output_position_in_map_grid[0] = position_in_array + map_size.x * 2;
  logical_state = kLogicalState_1;
}

void 
Constant_1::RunLogic(std::vector<Energy*>& energy_map) {
  //do nothing
}

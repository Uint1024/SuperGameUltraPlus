#include <iostream>
#include "energy.h"
#include "wire.h"

Energy::Energy(const eLogicalState state, const int value):
state(state),
value(value){

}

void 
Energy::FollowWires(std::vector<Wire*>& wire_map, 
        const Veci& map_size) {

}

void 
Energy::Update() {
  //std::cout << value << " -" << g_delta_t << std::endl;
  value -= g_delta_t;
  //std::cout << "=" << value << std::endl;
}

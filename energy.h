/* 
 * File:   Energy.h
 * Author: John
 *
 * Created on 4 février 2015, 20:10
 */

#ifndef ENERGY_H
#define	ENERGY_H
#include <vector>
#include "utils.h"
class Wire;

struct Energy {
  Energy(const eLogicalState state);
  void FollowWires(std::vector<Wire*>& wire_map, const Veci& map_size);
  int position_in_map_grid;
  eLogicalState state;
};

#endif	/* ENERGY_H */


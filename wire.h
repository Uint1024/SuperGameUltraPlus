/* 
 * File:   Wire.h
 * Author: John
 *
 * Created on 3 février 2015, 04:41
 */

#ifndef WIRE_H
#define	WIRE_H
#include <vector>
#include "solid_body.h"

class Engine;

struct Wire {
  Wire(const Vecf& position, const int position_in_array,
          const eDirection direction);
  void ChangeState (Wire* parent, eLogicalState state, 
                      std::vector<Wire*>& wire_map,
                      const Veci& map_size);
  void CheckOutputToWire(std::vector<Wire*> wire_map);
  void ResetState();
  int position_in_array;
  SolidBody* body;
  Wire* parent;
  eLogicalState logical_state;
  //wires can only be visited once per frame
  bool visited;
};

#endif	/* WIRE_H */


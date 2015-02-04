/* 
 * File:   logicgate.h
 * Author: John
 *
 * Created on 2 février 2015, 19:12
 */

#ifndef LOGICGATE_H
#define	LOGICGATE_H
#include <array>
#include <vector>
#include <map>


#include "utils.h"
#include "solid_body.h"
class Wire;

struct LogicGate {
  LogicGate(const Vecf& position, const eEditorObject type,
            const eDirection direction);
  void CheckOutputToWires(std::vector<Wire*>& wire_map, const Veci& map_size);
  SolidBody* body;
  eGateType type;
  eLogicalState logical_state;
  Bbox* input_bbox[2];
  Bbox* output_bbox;
  //each logic gate is a 3*3, each cell can be an output, an input, or neutral
  //each logic gate has its own because it can be rotated
  std::array<eLogicGatePart, 9> parts_map;
  static std::map<eEditorObject, std::array<eLogicGatePart, 9> > part_map_blueprints;
};

#endif	/* LOGICGATE_H */


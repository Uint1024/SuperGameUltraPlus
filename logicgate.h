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
class Energy;

struct LogicGate {
  LogicGate(const Vecf& position,
            const eDirection direction, const int position_in_array,
            const Veci& map_size, const eTexture texture_id);
  void CheckOutputToWires(std::vector<Energy*>& energy_map, const Veci& map_size);
  SolidBody* body;
  eGateType type;
  eLogicalState logical_state;
  virtual void RunLogic(std::vector<Energy*>& energy_map) = 0;
  /*Bbox* input_bbox[2];
  Bbox* output_bbox;*/
  //each logic gate is a 3*3, each cell can be an output, an input, or neutral
  //each logic gate has its own because it can be rotated
  std::array<eLogicGatePart, 9> parts_map;
  static std::map<eEditorObject, std::array<eLogicGatePart, 9> > part_map_blueprints;
  
  //this is the top left corner of the gate in the map grid
  int position_in_map_grid;
  //position of the wire it will output to
  int output_position_in_map_grid;
  int input_position_in_map_grid[2];
};

#endif	/* LOGICGATE_H */


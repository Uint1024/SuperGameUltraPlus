/* 
 * File:   or_gate.h
 * Author: John
 *
 * Created on 5 février 2015, 00:01
 */

#ifndef OR_GATE_H
#define	OR_GATE_H

#include "logicgate.h"


struct NotGate : LogicGate {
  NotGate(const Vecf& position,
            const eDirection direction, const int position_in_array,
            const Veci& map_size);

  virtual void RunLogic(std::vector<std::array<Energy*, 4>>& energy_map);

};

#endif	/* OR_GATE_H */


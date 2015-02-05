/* 
 * File:   constant_1.h
 * Author: John
 *
 * Created on 4 février 2015, 23:35
 */

#ifndef CONSTANT_1_H
#define	CONSTANT_1_H

#include "logicgate.h"


struct Constant_1 : LogicGate{
  Constant_1(const Vecf& position,
            const eDirection direction, const int position_in_array,
            const Veci& map_size);
  virtual void RunLogic(std::vector<std::array<Energy*, 4>>& energy_map) override;

};

#endif	/* CONSTANT_1_H */


/* 
 * File:   separator.h
 * Author: John
 *
 * Created on 5 février 2015, 01:26
 */

#ifndef SEPARATOR_H
#define	SEPARATOR_H
#include "utils.h"
#include <vector>
#include "logicgate.h"
class Energy;

struct Separator : public LogicGate {
  Separator(const Vecf& position,
            const eDirection direction, const int position_in_array,
            const Veci& map_size);

  virtual void RunLogic(std::vector<std::array<Energy*, 4>>& energy_map) override;

  virtual void Rotate(const eDirection direction, const Veci& map_size);

};

#endif	/* SEPARATOR_H */


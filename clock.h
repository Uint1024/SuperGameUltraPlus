/* 
 * File:   clock.h
 * Author: John
 *
 * Created on 9 février 2015, 00:49
 */

#ifndef CLOCK_H
#define	CLOCK_H

#include "logicgate.h"

struct Clock : public LogicGate{
  Clock(const Vecf& position,
            const eDirection direction, const int position_in_array,
            const Veci& map_size);

  virtual void RunLogic(std::vector<std::array<Energy*, 4>>& energy_map) override;

  virtual void Rotate(const eDirection direction, const Veci& map_size);
  int switch_every_ms;
  int switch_timer;
  int reverse_input;
};

#endif	/* CLOCK_H */


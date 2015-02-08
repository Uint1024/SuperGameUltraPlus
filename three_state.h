/* 
 * File:   three_state.h
 * Author: John
 *
 * Created on 8 février 2015, 04:03
 */

#ifndef THREE_STATE_H
#define	THREE_STATE_H
#include <vector>
#include "utils.h"
#include "logicgate.h"
class Energy;

struct ThreeState : public LogicGate{
  ThreeState( const Vecf& position,
              const eDirection direction, 
              const int position_in_array,
              const Veci& map_size);

  virtual void RunLogic(std::vector<std::array<Energy*, 4>>& energy_map) override;

  virtual void Rotate(const eDirection direction, const Veci& map_size);
  
};

#endif	/* THREE_STATE_H */


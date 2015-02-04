/* 
 * File:   and.h
 * Author: John
 *
 * Created on 2 février 2015, 19:19
 */

#ifndef AND_H
#define	AND_H
#include "logicgate.h"

struct AndGate : public LogicGate{
  AndGate(const Vecf& position,
            const eDirection direction, const int position_in_array,
            const Veci& map_size);

  virtual void RunLogic();

};

#endif	/* AND_H */


/* 
 * File:   logicgate.h
 * Author: John
 *
 * Created on 2 février 2015, 19:12
 */

#ifndef LOGICGATE_H
#define	LOGICGATE_H
#include "utils.h"
#include "solid.h"

struct LogicGate {
  LogicGate(const Vecf& position);
  Bbox bbox;
};

#endif	/* LOGICGATE_H */


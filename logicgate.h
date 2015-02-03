/* 
 * File:   logicgate.h
 * Author: John
 *
 * Created on 2 février 2015, 19:12
 */

#ifndef LOGICGATE_H
#define	LOGICGATE_H
#include "utils.h"
#include "solid_body.h"

struct LogicGate {
  LogicGate(const Vecf& position, const eEditorObject type);
  SolidBody body;
  eGateType type;
  
};

#endif	/* LOGICGATE_H */


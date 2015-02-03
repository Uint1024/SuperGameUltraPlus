/* 
 * File:   Wire.h
 * Author: John
 *
 * Created on 3 février 2015, 04:41
 */

#ifndef WIRE_H
#define	WIRE_H

#include "solid_body.h"

class Engine;

struct Wire {
  Wire(const Vecf& position);
  SolidBody body;
};

#endif	/* WIRE_H */


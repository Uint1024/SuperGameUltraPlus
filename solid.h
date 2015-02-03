/* 
 * File:   solid.h
 * Author: John
 *
 * Created on 2 février 2015, 19:20
 */

#ifndef SOLID_H
#define	SOLID_H
#include "utils.h"
#include "bbox.h"

struct Solid {
  Solid(const Vecf& pos, const Veci& size);
  Vecf pos;
  Bbox bbox;
};

#endif	/* SOLID_H */


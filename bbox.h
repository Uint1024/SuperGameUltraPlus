/* 
 * File:   bbox.h
 * Author: John
 *
 * Created on 2 février 2015, 20:49
 */

#ifndef BBOX_H
#define	BBOX_H
#include "utils.h"

struct Bbox {
  Bbox(const Vecf& position, const Veci& size);
  void Move(const Vecf& movement);
  void MoveTo(const Vecf& position);
  bool CollisionWithPoint(const Vecf& point);
  bool CollisionWithBbox(const Bbox& other);
  void Resize(const Veci& size_);
  float top, left, bottom, right;
  Veci size;
};

#endif	/* BBOX_H */


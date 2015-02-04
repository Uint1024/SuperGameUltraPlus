/* 
 * File:   SolidBody.h
 * Author: John
 *
 * Created on 3 février 2015, 17:00
 */

#ifndef SOLIDBODY_H
#define	SOLIDBODY_H

#include "utils.h"
#include "bbox.h"
#include "dynamic_sprite.h"
class Engine;

struct SolidBody {
  SolidBody(const Vecf& position, const Veci& size, const eTexture texture_id,
            const eDirection direction);
  virtual void Render(Engine& engine) = 0;
  Bbox bbox;
  DynamicSprite sprite;
  eDirection direction;
};

#endif	/* SOLIDBODY_H */


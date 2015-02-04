/* 
 * File:   StaticBody.h
 * Author: John
 *
 * Created on 4 février 2015, 18:04
 */

#ifndef STATICBODY_H
#define	STATICBODY_H

#include "solid_body.h"

//bodies of the static objects, like logic gates and wires
struct StaticBody : public SolidBody {
  StaticBody(const Vecf& position, const Veci& size, const eTexture texture_id,
            const eDirection direction);
  virtual void Render(Engine& engine);
};

#endif	/* STATICBODY_H */


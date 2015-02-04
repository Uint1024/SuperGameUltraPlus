/* 
 * File:   DynamicBody.h
 * Author: John
 *
 * Created on 4 février 2015, 18:05
 */

#ifndef DYNAMICBODY_H
#define	DYNAMICBODY_H

#include "solid_body.h"


struct DynamicBody : public SolidBody {
  DynamicBody(const Vecf& position, const Veci& size, const eTexture texture_id,
            const eDirection direction);
  virtual void Render(Engine& engine);

};

#endif	/* DYNAMICBODY_H */


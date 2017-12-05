//
// A light circle is an object that emits light in a circle
// The light emitted from this circle is affected by other objects
//

#pragma once
#include "Object.h"

class LightCircle : public Object
{
private:
	float m_radius;
	const int m_rays;

public:
	// The ammount of rays the object has to display, the higher the more accurate round the object will appear
	LightCircle(const int rays = 100, const float radius = 0);
	
	//virtual void update(const float elapsedTime, const CollisionManager &collisionManager) override final;
	
	void setRadius(const float radius);
};
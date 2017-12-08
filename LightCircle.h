//
// A light circle is an object that emits light in a circle
// The light emitted from this circle is affected by other objects
//

#pragma once
#include "Object.h"

class LightCircle : public Object
{
private:
	// Takes a movement vector (magnitude), not a point of coordinate
	void rotateVector(sf::Vector2f &vector, const float degrees) const;

	float m_radius;
	const int m_rays;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override final;

public:
	// The ammount of rays the object has to display, the higher the more accurate round the object will appear
	LightCircle(const float radius = 0, const int rays = 100);
	
	virtual void hit() override final;
	virtual void update(const float elapsedTime, const CollisionManager &collisionManager) override final;
	
	void setRadius(const float radius);
};
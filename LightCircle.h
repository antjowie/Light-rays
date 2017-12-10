//
// A light circle is an object that emits light in a circle
// The light emitted from this circle is affected by other objects
//

#pragma once
#include "Object.h"

#include <SFML\Graphics\CircleShape.hpp>

class LightCircle : public Object
{
private:
	// This circle is used to calculate the points for all the triangles. 
	sf::CircleShape m_circle;

	// Takes a movement vector (magnitude), not a point of coordinate
	void rotateVector(sf::Vector2f &vector, const float degrees) const;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override final;

public:
	// The ammount of rays the object has to display, the higher the more round the object will appear
	// The ray value does not effect accuracy whatsoever because rays are calculated depending on surrouding objects
	LightCircle(const float radius = 0, const int rays = 100);
	
	virtual void hit() override final;
	virtual void update(const float elapsedTime, const CollisionManager &collisionManager) override final;
	
	void setRadius(const float radius);
};
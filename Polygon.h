#pragma once
#include "Object.h"

class Polygon:
	public Object

{
private:
	bool m_hit;

public:
	Polygon(const sf::PrimitiveType &type, sf::Texture &texture, const int collisionFilter = 10);

	virtual void hit() override;
	virtual void update(const float elapsedTime, const CollisionManager &collisionManager) override;

	void addVertex(const sf::Vertex &vertex);
	
	// Temp for mouse
	sf::Vertex *getLastVertex();
};


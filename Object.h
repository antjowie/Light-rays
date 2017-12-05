//
// An object is an in game object which acts upon collision.
// The life of objects are managed by the CollisionManager class.
//

#pragma once
#include "CollisionManager.h"

#include <SFML\Graphics\Transformable.hpp>
#include <SFML\Graphics\VertexArray.hpp>
#include <SFML\Graphics\Drawable.hpp>

class Object:
	public sf::Transformable, public sf::Drawable
{
private:
	const int m_collisionFilter;	// The collision filter value decides wether or not the collision happens with value, light for example passes through glass (2) but a player doesn't (1).
	sf::Texture const *m_texture;	// This texture reference is used for the vertices
	
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	
protected:
	//AnimationHandler m_animHandler;
	sf::VertexArray m_vertices;	// Contains our vertices and modifies them
	bool m_hit;					// Used when update needs to know this value

public:
	bool m_isDead;

	// CollisionFilter: when the collisionFilter of the compared object is higher, collision happens
	// Example: light(1) and stone(2), collision happens, but light(1) and glass(1) collision didn't happen
	// Example: player(0), everything collides except objects with value 0
	Object(const sf::PrimitiveType type, sf::Texture &texture, const int collisionFilter = 10);

	virtual void hit() = 0; // When the object has collided
	virtual void update(const float elapsedTime, const CollisionManager &collisionManager) = 0;

	const int getCollisionFilter() const;
	// Used for collision checking, more precise than AABB boxes
	const std::vector<sf::Vertex> getVertices() const;
};
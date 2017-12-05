//
// The CollisionManager handles all objects
// and all the collision related stuff.
//

#pragma once
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Vertex.hpp>
#include <SFML\Graphics\Drawable.hpp>
#include <list>
#include <vector>
#include <functional>

// The object class needs the CollisionManager class to be created so we declare that 
// because object also needs CollisionManager
class Object;

struct Collided
{
	float m_percentage{ 1 };		// The percentage of the vector until collision, will be inbetween 0-1 with 1 meaning no collision
	Object * m_collided{ nullptr };	// A pointer to the object which had been collided with. If no collision happened, this will be a nullptr
	sf::Vector2f m_point{ 0,0 };	// Collision point
};

class CollisionManager : public sf::Drawable
{
private:
	std::list<Object *> m_objects;

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

public:
	~CollisionManager();

	// Pushes a new object into the list
	void push(Object * const object); 
	
	// Updates all objects and kills those which are supposed to be killed
	void update(const float elapsedTime);

	// Check for collision on the given line, startpoint a to point b.
	// ThisObject is used to check if we're not comparing the same object
	Collided getCollision(const sf::Vector2f &begin, const sf::Vector2f &end, const Object* thisObject) const;

	// Internal function, get all vertices in the given rectangle
	std::vector<Object* > getSurroundingObjects(const sf::Vector2f &left, const sf::Vector2f &right, const Object* thisObject) const;
	// Internal function, get all vertices in the given circle
	std::vector<Object* > getSurroundingObjects(const sf::Vector2f &center, const float radius, const Object * thisObject) const;
};
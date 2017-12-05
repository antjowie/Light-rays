#include "CollisionManager.h"
#include "Object.h"

#include <SFML\Graphics\RenderTarget.hpp>

void CollisionManager::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	for (const auto& iter : m_objects)
		target.draw(*iter, states);
}

CollisionManager::~CollisionManager()
{
	for (auto &deleter : m_objects)
		delete deleter;
	m_objects.clear(); // Not necessary but I like to explicitly state it
}

void CollisionManager::push(Object * const object)
{
	m_objects.push_back(object);
}

void CollisionManager::update(const float elapsedTime)
{
	// Removes the object
	m_objects.remove_if([this] (Object *const object) 
	{	if (!object->m_isDead)
			return false;
		delete object;
		return true; } );

	for (const auto& iter : m_objects)
		iter->update(elapsedTime, *this);
}

Collided CollisionManager::getCollision(const sf::Vector2f & begin, const sf::Vector2f & end, const Object* thisObject) const
{
	// It is faster to check all objects insted of checking which objects are close and then check those objects
	Collided collided;
	const sf::Vector2f movement{ end - begin };

	for (auto &iter : m_objects)
	{
		if (iter == thisObject) continue;
		const std::vector<sf::Vertex> &vertices{ iter->getVertices() };
		for (size_t i = 0; i < vertices.size(); ++i)
		{
			const sf::Vector2f begin2{ vertices[i].position + iter->getPosition()}; // Line begin
			const sf::Vector2f movement2{ (vertices[(i + 1) % vertices.size()].position) + iter->getPosition() - begin2}; // Movement of the line

			// Calculating collision
			// A: object starting point
			// B: object vector 
			// C: object starting point
			// D: object vector
			//
			// Because this collision system is based on rays, we can write each ray like this:
			// A.x + B.x*T1 = C.x + D.x*T2
			// A.y + B.y*T1 = C.y + D.y*T2
			//
			// The T values are percentages. We first isolate T1 out of the formula
			// T1 = (C.x + D.x*T2 - A.x) / B.x = (C.y + D.y*T2 - A.y) / B.y
			//
			// We then need to calculate T2
			// B.y*C.x + T2*D.x*B.y - B.y*A.x = B.x*C.y + T2*D.y*B.x - B.x*A.y
			// T2*D.x*B.y - T2*D.y*B.x = B.x(C.y-A.y) + B.y(A.x - C.x)
			// T2(D.x*B.y - D.y*B.x) = ...
			// T2 = (B.x(C.y-A.y) + B.y(A.x - C.x)) / (D.x*B.y - D.y*B.x)
			//
			// If T1 is larger then 0, collision could happened
			// If T2 is inbetween 0-1, collision happened (because the object ray coulnd't reach it's end)

			const float t2{ (movement.x*(begin2.y - begin.y) + movement.y*(begin.x - begin2.x)) / (movement2.x*movement.y - movement2.y*movement.x) };
			const float t1{ (begin2.x + movement2.x*t2 - begin.x) / movement.x };

			if (t1 > 0 && t2 > 0 && t2 < 1.f && t1 < collided.m_percentage)
			{
				collided.m_percentage = t1;
				collided.m_collided = iter;
				collided.m_point = sf::Vector2f(begin.x + movement.x * t1, begin.y + movement.y * t1);
			}
		}
	}
	return collided;
}

std::vector<Object*> CollisionManager::getSurroundingObjects(const sf::Vector2f & left, const sf::Vector2f & right, const Object* thisObject) const
{
	std::vector<Object* > returner;

	for (const auto &object : m_objects)
	{
		if (object == thisObject) continue;
		for (const auto &iter : object->getVertices())
		{
			sf::Vector2f vertex{ iter.position + object->getPosition() };
			if (vertex.x > std::min(left.x, right.x) && vertex.x < std::max(left.x, right.x) &&
				vertex.y > std::min(left.y, right.y) && vertex.y < std::max(left.y, right.y))
			{
				returner.push_back(object);
				break;
			}
		}
	}
	return returner;
}

std::vector<Object* > CollisionManager::getSurroundingObjects(const sf::Vector2f & center, const float radius, const Object* thisObject) const
{
	std::vector<Object* > returner;
	for (const auto &object : m_objects)
	{
		if (object == thisObject) continue;
		for (const auto &iter: object->getVertices())
			if (std::pow(iter.position.x + object->getPosition().x - center.x,2) + std::pow(iter.position.y + object->getPosition().y - center.y,2) < std::pow(radius,2))
			{
				returner.push_back(object);
				break;
			}
	}
	return returner;
}
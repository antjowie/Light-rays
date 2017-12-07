#include "Object.h"

#include <SFML\Graphics\RenderTarget.hpp>

void Object::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform = getTransform();
	//states.texture = m_texture;
	target.draw(m_vertices, states);
}

Object::Object(const sf::PrimitiveType type, sf::Texture &texture, const int collisionFilter):
	m_vertices(type), m_collisionFilter(collisionFilter), m_texture(&texture), m_isDead(false), m_hit(false)
{	
}

const int Object::getCollisionFilter() const
{
	return m_collisionFilter;
}

const std::vector<sf::Vertex> Object::getVertices() const
{
	std::vector<sf::Vertex> temp;
	for (size_t i{ 0 }; i < m_vertices.getVertexCount(); ++i)
		temp.push_back(m_vertices[i]);
	return temp;
}

sf::Vector2f Object::localToWorld(const sf::Vector2f & point) const
{
	return point - getOrigin() + getPosition();
}

sf::Vector2f Object::worldToLocal(const sf::Vector2f & point) const
{
	return point - getPosition() + getOrigin();
}

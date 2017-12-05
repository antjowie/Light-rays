#include "Polygon.h"



Polygon::Polygon(const sf::PrimitiveType &type, sf::Texture &texture, const int collisionFilter):
	Object(type,texture,collisionFilter),
	m_hit(false)
{
}

void Polygon::hit()
{
	m_hit = true;
}

void Polygon::update(const float elapsedTime, const CollisionManager & collisionManager)
{
	if(m_hit)
		for (size_t i = 0; i < m_vertices.getVertexCount(); ++i)
			m_vertices[i].color = sf::Color(sf::Color::Red);
	else
		for (size_t i = 0; i < m_vertices.getVertexCount(); ++i)
			m_vertices[i].color = sf::Color(sf::Color::White);

	m_hit = false;
}

void Polygon::addVertex(const sf::Vertex & vertex)
{
	m_vertices.append(vertex);
}

sf::Vertex * Polygon::getLastVertex()
{
	return &m_vertices[m_vertices.getVertexCount() - 1];
}

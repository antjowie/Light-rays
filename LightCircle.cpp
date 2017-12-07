#include "LightCircle.h"

#include <SFML\Graphics\Texture.hpp>

#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\CircleShape.hpp>

#include <iostream>

static sf::Texture tex;

void LightCircle::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform = getTransform();
	//states.texture = m_texture;
	target.draw(m_vertices, states);

	sf::CircleShape temp(m_radius,120);
	temp.setOrigin(m_radius, m_radius);
	temp.setPosition(getPosition());
	temp.setFillColor(sf::Color(80, 80, 80, 100));
	temp.setOutlineThickness(1);
	temp.setOutlineColor(sf::Color::Red);
	target.draw(temp);
	return;
	if (!m_vertices.getVertexCount() > 1)
		return;
	sf::CircleShape end(10);
	end.setOrigin(10, 10);
	end.setPosition(m_vertices[1].position - getOrigin() + getPosition());
	end.setFillColor(sf::Color::Black);
	target.draw(end);
	
	sf::CircleShape beg(10);
	beg.setOrigin(10, 10);
	beg.setPosition(getPosition());
	beg.setFillColor(sf::Color::Black);
	target.draw(beg);
}

LightCircle::LightCircle(const float radius, const int rays):
	Object(sf::PrimitiveType::Lines,tex,1), // Should happen in an init phase
	m_rays(rays)
{
	setRadius(radius);
	tex.loadFromFile("");
}

void LightCircle::hit()
{
}

void LightCircle::update(const float elapsedTime, const CollisionManager & collisionManager)
{
	m_vertices.clear();
	for (const auto &object: collisionManager.getSurroundingObjects(getPosition(), m_radius, this))
		for (const auto &vertex : object->getVertices())
		{
			m_vertices.append(sf::Vertex(getOrigin(), sf::Color::Red));

			sf::Vertex temp(vertex);
			temp.color = sf::Color::Red;

			temp.position = worldToLocal(object->localToWorld(temp.position));

			// Turn into a unit vector and give radius as length
			sf::Vector2f movement{ temp.position - getOrigin() };
			movement = movement / std::sqrtf(movement.x * movement.x + movement.y * movement.y) * m_radius;
			
			Collided collided = collisionManager.getCollision(getPosition(), localToWorld(getOrigin() + movement), this);
			temp.position = worldToLocal(movement * collided.percentage + getPosition());
		
			m_vertices.append(temp);
		}
}

void LightCircle::setRadius(const float radius)
{
	m_radius = std::abs(radius);
	setOrigin(m_radius, m_radius);
}

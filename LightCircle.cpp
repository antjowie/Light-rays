#include "LightCircle.h"

#include <SFML\Graphics\Texture.hpp>

#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\CircleShape.hpp>

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
	{
		for (const auto &vertex : object->getVertices())
		{
			m_vertices.append(sf::Vertex(getOrigin(), sf::Color::Red));
			sf::Vertex temp(vertex);
			temp.color = sf::Color::Red;

			// Map local vector position to other local object coordinates
			temp.position += object->getPosition() - getPosition() +m_vertices[0].position;
			
			// Make it into a unit vector and convert to max range
			sf::Vector2f movement(temp.position - m_vertices[0].position);
			const float magnitude{ sqrtf(std::powf(movement.x, 2) + std::powf(movement.y, 2)) };
			movement = movement / magnitude * m_radius;
			temp.position = movement + m_vertices[0].position;
			
			// Fix position with collisions
			Collided collided = collisionManager.getCollision(getPosition(), temp.position + getPosition() - m_vertices[0].position, this);
			movement *= collided.percentage;
			temp.position = movement + m_vertices[0].position;

			m_vertices.append(temp);
		}
	}
}

void LightCircle::setRadius(const float radius)
{
	m_radius = std::abs(radius);
	setOrigin(m_radius, m_radius);
}

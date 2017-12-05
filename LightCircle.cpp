#include "LightCircle.h"

#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\Texture.hpp>

static sf::Texture tex;

LightCircle::LightCircle(const int rays, const float radius):
	Object(sf::PrimitiveType::TrianglesFan,tex,1), // Should happen in an init phase
	m_radius(radius),
	m_rays(rays)
{
	tex.loadFromFile("");
}
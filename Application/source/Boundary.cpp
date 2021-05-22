//////////////////////////////
// Author: Liam Bansal
// Date Created: 21/05/2021
//////////////////////////////

// File's header.
#include "Boundary.h"

Boundary::Boundary(glm::vec3 a_newPosition,
	glm::vec3 a_newDimensions) : m_position(a_newPosition),
	m_dimensions(a_newDimensions)
{}

// Is a 3D position within the boundary's dimensions?
bool Boundary::Contains(glm::vec3 a_position) const
{
	return (a_position.x >= m_position.x - m_dimensions.x &&
		a_position.x <= m_position.x + m_dimensions.x &&
		a_position.y >= m_position.y - m_dimensions.y &&
		a_position.y <= m_position.y + m_dimensions.y &&
		a_position.z >= m_position.z - m_dimensions.z &&
		a_position.z <= m_position.z + m_dimensions.z);
}

// Are two boundaries are intersecting? I.e. sharing the same volume of space.
bool Boundary::Intersects(Boundary a_otherBoundary) const
{
	// Check that at least one of the two boundaries' faces intersect.
	// Left face.
	return ((a_otherBoundary.GetPosition().x - a_otherBoundary.GetDimensions().x <= m_position.x + m_dimensions.x &&
		a_otherBoundary.GetPosition().x - a_otherBoundary.GetDimensions().x >= m_position.x - m_dimensions.x) ||
		// Right face.
		(a_otherBoundary.GetPosition().x + a_otherBoundary.GetDimensions().x <= m_position.x + m_dimensions.x &&
		a_otherBoundary.GetPosition().x + a_otherBoundary.GetDimensions().x >= m_position.x - m_dimensions.x) ||
		// Bottom face.
		(a_otherBoundary.GetPosition().y - a_otherBoundary.GetDimensions().y <= m_position.y + m_dimensions.y &&
			a_otherBoundary.GetPosition().y - a_otherBoundary.GetDimensions().y >= m_position.y - m_dimensions.y) ||
		// Top face.
		(a_otherBoundary.GetPosition().y + a_otherBoundary.GetDimensions().y <= m_position.x + m_dimensions.y &&
			a_otherBoundary.GetPosition().y + a_otherBoundary.GetDimensions().y >= m_position.y - m_dimensions.y) ||
		// Back face.
		(a_otherBoundary.GetPosition().z - a_otherBoundary.GetDimensions().x <= m_position.x + m_dimensions.x &&
			a_otherBoundary.GetPosition().z - a_otherBoundary.GetDimensions().x >= m_position.x - m_dimensions.x) ||
		// Forward face.
		(a_otherBoundary.GetPosition().z + a_otherBoundary.GetDimensions().z <= m_position.z + m_dimensions.z &&
			a_otherBoundary.GetPosition().z + a_otherBoundary.GetDimensions().z >= m_position.z - m_dimensions.z));
}
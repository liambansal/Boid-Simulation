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

// Checks if a position is within the boundary dimensions
bool Boundary::Contains(glm::vec3 a_position) const
{
	return (a_position.x >= m_position.x - m_dimensions.x &&
		a_position.x <= m_position.x + m_dimensions.x &&
		a_position.y >= m_position.y - m_dimensions.y &&
		a_position.y <= m_position.y + m_dimensions.y &&
		a_position.z >= m_position.z - m_dimensions.z &&
		a_position.z <= m_position.z + m_dimensions.z);
}
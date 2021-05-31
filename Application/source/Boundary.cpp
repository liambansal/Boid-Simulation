//////////////////////////////
// Author: Liam Bansal
// Date Created: 21/05/2021
//////////////////////////////

// File's header.
#include "Boundary.h"

Boundary::Boundary() : m_pPosition(new glm::vec3(1.0f)),
	m_dimensions(1.0f)
{}

Boundary::Boundary(glm::vec3 a_newPosition,
	glm::vec3 a_newDimensions) : m_pPosition(new glm::vec3(a_newPosition)),
	m_dimensions(a_newDimensions)
{}

Boundary::Boundary(glm::vec3* a_pNewPosition,
	glm::vec3 a_newDimensions) : m_pPosition(a_pNewPosition),
	m_dimensions(a_newDimensions)
{}

Boundary::~Boundary()
{}

// Is a 3D position within the boundary's dimensions?
bool Boundary::Contains(const glm::vec3& a_position) const
{
	return (a_position.x >= m_pPosition->x - m_dimensions.x &&
		a_position.x <= m_pPosition->x + m_dimensions.x &&
		a_position.y >= m_pPosition->y - m_dimensions.y &&
		a_position.y <= m_pPosition->y + m_dimensions.y &&
		a_position.z >= m_pPosition->z - m_dimensions.z &&
		a_position.z <= m_pPosition->z + m_dimensions.z);
}

// Returns true if two boundaries share the same volume of space.
bool Boundary::Overlaps(Boundary a_otherBoundary) const
{
	return (a_otherBoundary.GetPosition()->x - a_otherBoundary.GetDimensions().x <= m_pPosition->x + m_dimensions.x &&
		a_otherBoundary.GetPosition()->x + a_otherBoundary.GetDimensions().x >= m_pPosition->x - m_dimensions.x ||
		a_otherBoundary.GetPosition()->y - a_otherBoundary.GetDimensions().y <= m_pPosition->y + m_dimensions.y &&
		a_otherBoundary.GetPosition()->y + a_otherBoundary.GetDimensions().y >= m_pPosition->y - m_dimensions.y ||
		a_otherBoundary.GetPosition()->z - a_otherBoundary.GetDimensions().z <= m_pPosition->z + m_dimensions.z &&
		a_otherBoundary.GetPosition()->z + a_otherBoundary.GetDimensions().z >= m_pPosition->z - m_dimensions.z);
}
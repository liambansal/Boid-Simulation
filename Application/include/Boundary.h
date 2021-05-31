//////////////////////////////
// Author: Liam Bansal
// Date Created: 21/05/2021
//////////////////////////////

#ifndef BOUNDARY_H
#define BOUNDARY_H

#include "glm/glm.hpp"

// Represents a volume of space.
class Boundary
{
public:
	Boundary();
	Boundary(glm::vec3 a_newPosition,
		glm::vec3 a_newDimensions);
	Boundary(glm::vec3* a_pNewPosition,
		glm::vec3 a_newDimensions);
	~Boundary();

	// Returns true if a point is located within a boundary.
	bool Contains(const glm::vec3& a_position) const;
	// Returns true if two boundaries share the same volume of space.
	bool Overlaps(Boundary a_otherBoundary) const;

	inline void SetPosition(glm::vec3* a_pNewPosition);
	inline void SetDimensions(glm::vec3 a_newDimensions);

	inline const glm::vec3* GetPosition() const;
	inline const glm::vec3 GetDimensions() const;

private:
	glm::vec3* m_pPosition;
	// Measured outward from position in order of width, height and length.
	glm::vec3 m_dimensions;
};

void Boundary::SetPosition(glm::vec3* a_pNewPosition)
{
	m_pPosition = a_pNewPosition;
}

void Boundary::SetDimensions(glm::vec3 a_newDimensions)
{
	m_dimensions = a_newDimensions;
}

const glm::vec3* Boundary::GetPosition() const
{
	return m_pPosition;
}

const glm::vec3 Boundary::GetDimensions() const
{
	return m_dimensions;
}

#endif // !BOUNDARY_H

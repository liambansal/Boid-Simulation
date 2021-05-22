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
	Boundary(glm::vec3 a_newPosition,
		glm::vec3 a_newDimensions);
	~Boundary()
	{}

	bool Contains(glm::vec3 a_position) const;
	bool Intersects(Boundary a_otherBoundary) const;

	inline void SetPosition(glm::vec3 a_newPosition);
	inline void SetDimensions(glm::vec3 a_newDimensions);

	inline const glm::vec3 GetPosition() const;
	inline const glm::vec3 GetDimensions() const;

private:
	glm::vec3 m_position;
	// Measured outward from position in order of width, height and length.
	glm::vec3 m_dimensions;
};

void Boundary::SetPosition(glm::vec3 a_newPosition)
{
	m_position = a_newPosition;
}

void Boundary::SetDimensions(glm::vec3 a_newDimensions)
{
	m_dimensions = a_newDimensions;
}

const glm::vec3 Boundary::GetPosition() const
{
	return m_position;
}

const glm::vec3 Boundary::GetDimensions() const
{
	return m_dimensions;
}

#endif // !BOUNDARY_H

//////////////////////////////
// Author: Liam Bansal
// Date Created: 21/05/2021
//////////////////////////////

#ifndef BOUNDARY_H
#define BOUNDARY_H

// Header includes.
#include "glad/glad.h"

// Forward declarations.
class Framework;

const float vertices[] = {
	10.0f, -10.0f, 10.0f,
	-10.0f, -10.0f, 10.0f,
	-10.0f, -10.0f, -10.0f,
	10.0f, -10.0f, -10.0f,
	// Extra starting coordinate to join the last two vertices together.
	10.0f, -10.0f, 10.0f
};
const unsigned int coordinatesPerVertex = 3;
// Specifies the number of lines to draw for the scene's bounds.
const GLsizei drawCount = sizeof(vertices) / sizeof(float) / coordinatesPerVertex;

/// <summary>
/// Represents a volume of space with a central position.
/// </summary>
/// <typeparam name="TVector"> The type of vector that's used for storing the boundary's position and dimensions. </typeparam>
template <typename TVector>
class Boundary {
public:
	Boundary();
	Boundary(TVector a_newPosition,
		TVector a_newDimensions);
	Boundary(TVector* a_pNewPosition,
		TVector a_newDimensions);
	~Boundary();

	/// <summary>
	/// Draws a line along each edge of the boundary.
	/// </summary>
	void Draw();
	/// <summary>
	/// Returns true if the specified position is located within the boundary.
	/// </summary>
	/// <param name="a_position"> A position. </param>
	/// <returns> True if the position is located within the boundary's area. </returns>
	bool Contains(const TVector& a_position) const;
	/// <summary>
	/// Returns true if two boundaries share some the same space.
	/// </summary>
	/// <param name="a_otherBoundary"> The other boundary to check against this. </param>
	/// <returns> True if the two boundaries overlap. </returns>
	bool Overlaps(Boundary<TVector> a_otherBoundary) const;

	inline void SetPosition(TVector* a_pNewPosition);
	inline void SetDimensions(TVector a_newDimensions);
	void SetRenderingFramework(Framework* a_pRenderingFramework);

	inline const TVector* GetPosition() const;
	inline const TVector GetDimensions() const;
	const Framework* GetRenderingFramework() const;

private:
	/// <summary>
	/// The position at the centre of the boundary.
	/// </summary>
	TVector* m_pPosition;
	/// <summary>
	/// The width, height, and depth of the boundary as measured outward from its central position.
	/// </summary>
	TVector m_dimensions;
	Framework* m_pRenderingFramework;
};

template <typename TVector>
Boundary<TVector>::Boundary() : m_pPosition(new TVector(1.0f)),
m_dimensions(1.0f) {}

template <typename TVector>
Boundary<TVector>::Boundary(TVector a_newPosition,
	TVector a_newDimensions) : m_pPosition(new TVector(a_newPosition)),
	m_dimensions(a_newDimensions) {}

template <typename TVector>
Boundary<TVector>::Boundary(TVector* a_pNewPosition,
	TVector a_newDimensions) : m_pPosition(a_pNewPosition),
	m_dimensions(a_newDimensions) {}

template <typename TVector>
Boundary<TVector>::~Boundary() {}

template <typename TVector>
void Boundary<TVector>::Draw() {
	if (!m_pRenderingFramework) {
		m_pRenderingFramework = Framework::GetInstance();

		if (!m_pRenderingFramework) {
			return;
		}
	}

	m_pRenderingFramework->Draw(vertices, drawCount);
}

template <typename TVector>
bool Boundary<TVector>::Contains(const TVector& a_position) const {
	if (!m_pPosition) {
		return false;
	}

	// Checks if the argument position is located between the boundary's lower and upper extents (the distance that 
	// the boundary is projected across, from its central position) along each axis of the boundary.
	return (a_position.x >= m_pPosition->x - m_dimensions.x &&
		a_position.x <= m_pPosition->x + m_dimensions.x &&
		a_position.y >= m_pPosition->y - m_dimensions.y &&
		a_position.y <= m_pPosition->y + m_dimensions.y &&
		a_position.z >= m_pPosition->z - m_dimensions.z &&
		a_position.z <= m_pPosition->z + m_dimensions.z);
}

template <typename TVector>
bool Boundary<TVector>::Overlaps(Boundary a_otherBoundary) const {
	// If the left extent of this boundary is further left than the other boundary's right extent, and.
	// if the right extent of this boundary is further right than the other boundary's left extent then the 
	// boundaries must overlap along that axis.
	return (a_otherBoundary.GetPosition()->x - a_otherBoundary.GetDimensions().x <= m_pPosition->x + m_dimensions.x &&
		a_otherBoundary.GetPosition()->x + a_otherBoundary.GetDimensions().x >= m_pPosition->x - m_dimensions.x &&
		a_otherBoundary.GetPosition()->y - a_otherBoundary.GetDimensions().y <= m_pPosition->y + m_dimensions.y &&
		a_otherBoundary.GetPosition()->y + a_otherBoundary.GetDimensions().y >= m_pPosition->y - m_dimensions.y &&
		a_otherBoundary.GetPosition()->z - a_otherBoundary.GetDimensions().z <= m_pPosition->z + m_dimensions.z &&
		a_otherBoundary.GetPosition()->z + a_otherBoundary.GetDimensions().z >= m_pPosition->z - m_dimensions.z);
}

template <typename TVector>
void Boundary<TVector>::SetPosition(TVector* a_pNewPosition) {
	m_pPosition = a_pNewPosition;
}

template <typename TVector>
void Boundary<TVector>::SetDimensions(TVector a_newDimensions) {
	m_dimensions = a_newDimensions;
}

template <typename TVector>
void Boundary<TVector>::SetRenderingFramework(Framework* a_pRenderingFramework) {
	m_pRenderingFramework = a_pRenderingFramework;
}

template <typename TVector>
const TVector* Boundary<TVector>::GetPosition() const {
	return m_pPosition;
}

template <typename TVector>
const TVector Boundary<TVector>::GetDimensions() const {
	return m_dimensions;
}

template <typename TVector>
const Framework* Boundary<TVector>::GetRenderingFramework() const {
	return m_pRenderingFramework;
}

#endif // !BOUNDARY_H

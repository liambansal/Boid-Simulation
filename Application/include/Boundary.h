//////////////////////////////
// Author: Liam Bansal
// Date Created: 21/05/2021
//////////////////////////////

#ifndef BOUNDARY_H
#define BOUNDARY_H

// Header includes.
#include "Framework.h";
#include "glad/glad.h"
#include "glm/glm.hpp"

/// <summary>
/// Represents a volume of space with a central position.
/// </summary>
/// <typeparam name="TVector"> The type of vector that's used for storing the boundary's position and dimensions.
/// Must be a complete type. </typeparam>
template <typename TVector>
class Boundary {
public:
	Boundary();
	Boundary(TVector a_newPosition,
		TVector a_newDimensions);
	Boundary(TVector* a_pPositionToCopy,
		TVector a_newDimensions);
	~Boundary();

	/// <summary>
	/// Draws a line along each of the boundary's edges.
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

	inline const TVector* GetPosition() const;
	inline const TVector GetDimensions() const;
	const Framework* GetRenderingFramework() const;

private:
	/// <summary>
	/// Calculates where a boundary's vertices would be positioned.
	/// </summary>
	/// <param name="a_pBoundaryPosition"> The boundary's central position. </param>
	/// <param name="a_pBoundaryDimensions"> The boundary's width, height, and depth. </param>
	void FindVertexPositions(TVector* a_pBoundaryPosition, TVector* a_pBoundaryDimensions);
	void SetupRenderingBuffers();

	/// <summary>
	/// The position at the centre of the boundary.
	/// </summary>
	TVector* m_pPosition;
	/// <summary>
	/// The width, height, and depth of the boundary as measured outward from its central position.
	/// </summary>
	TVector m_dimensions;

	// Each boundary is a cube which means there's a maximum of 24 coordinates per boundary (3 per vertex).
	/// <summary>
	/// A collection of coordinates that make up each vertex of the boundary.
	/// </summary>
	float m_fVertexCoordinates[24];
	unsigned int m_uiCoordinatesPerVertex = 3;
	unsigned int m_uiCoordinatesCount = 0;
	/// <summary>
	/// Specifies the number of lines to draw for the scene's bounds.
	/// </summary>
	GLsizei m_iLineDrawCount = 0;
	unsigned int linesVAO = 0;
	unsigned int linesVBO = 0;
	Framework* m_pRenderingFramework;
};

template <typename TVector>
Boundary<TVector>::Boundary() : m_pPosition(new TVector(1.0f)),
	m_dimensions(1.0f),
	m_pRenderingFramework(Framework::GetInstance()) {
	FindVertexPositions(m_pPosition, &m_dimensions);
	SetupRenderingBuffers();
}

template <typename TVector>
Boundary<TVector>::Boundary(TVector a_pPositionToCopy,
	TVector a_newDimensions) : m_pPosition(new TVector(a_pPositionToCopy)),
	m_dimensions(a_newDimensions),
	m_pRenderingFramework(Framework::GetInstance()) {
	FindVertexPositions(m_pPosition, &m_dimensions);
	SetupRenderingBuffers();
}

template <typename TVector>
Boundary<TVector>::Boundary(TVector* a_newPosition,
	TVector a_newDimensions) : m_pPosition(a_newPosition),
	m_dimensions(a_newDimensions),
	m_pRenderingFramework(Framework::GetInstance()) {
	FindVertexPositions(m_pPosition, &m_dimensions);
	SetupRenderingBuffers();
}

template <typename TVector>
Boundary<TVector>::~Boundary() {
	glDeleteVertexArrays(1, &linesVAO);
	glDeleteBuffers(1, &linesVBO);
}

template <typename TVector>
void Boundary<TVector>::Draw() {
	if (!m_pRenderingFramework || !m_fVertexCoordinates || m_uiCoordinatesCount == 0 || m_iLineDrawCount == 0) {
		return;
	}

	m_pRenderingFramework->UseLineShader();
	glBindVertexArray(linesVAO);
	glDrawArrays(GL_LINE_STRIP, 0, m_iLineDrawCount);
	glBindVertexArray(0);
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

template <typename TVector>
void Boundary<TVector>::FindVertexPositions(TVector* a_pBoundaryPosition, TVector* a_pBoundaryDimensions) {
	memset(m_fVertexCoordinates, 0, sizeof(m_fVertexCoordinates));

	if (!a_pBoundaryPosition || !a_pBoundaryDimensions) {
		return;
	}

	const float half = 0.5f;
	unsigned int coordinateIndex = 0;
	// Bottom vertices.
	// Front left vertex.
	m_fVertexCoordinates[coordinateIndex++] = a_pBoundaryPosition->x - a_pBoundaryDimensions->x;
	m_fVertexCoordinates[coordinateIndex++] = a_pBoundaryPosition->y - a_pBoundaryDimensions->y;
	m_fVertexCoordinates[coordinateIndex++] = a_pBoundaryPosition->z + a_pBoundaryDimensions->z;
	// Back left vertex.
	m_fVertexCoordinates[coordinateIndex++] = a_pBoundaryPosition->x - a_pBoundaryDimensions->x;
	m_fVertexCoordinates[coordinateIndex++] = a_pBoundaryPosition->y - a_pBoundaryDimensions->y;
	m_fVertexCoordinates[coordinateIndex++] = a_pBoundaryPosition->z - a_pBoundaryDimensions->z;
	// Back right vertex.
	m_fVertexCoordinates[coordinateIndex++] = a_pBoundaryPosition->x + a_pBoundaryDimensions->x;
	m_fVertexCoordinates[coordinateIndex++] = a_pBoundaryPosition->y - a_pBoundaryDimensions->y;
	m_fVertexCoordinates[coordinateIndex++] = a_pBoundaryPosition->z - a_pBoundaryDimensions->z;
	// Front right vertex.
	m_fVertexCoordinates[coordinateIndex++] = a_pBoundaryPosition->x + a_pBoundaryDimensions->x;
	m_fVertexCoordinates[coordinateIndex++] = a_pBoundaryPosition->y - a_pBoundaryDimensions->y;
	m_fVertexCoordinates[coordinateIndex++] = a_pBoundaryPosition->z + a_pBoundaryDimensions->z;

	// Top vertices.
	// Front left vertex.
	m_fVertexCoordinates[coordinateIndex++] = a_pBoundaryPosition->x + a_pBoundaryDimensions->x;
	m_fVertexCoordinates[coordinateIndex++] = a_pBoundaryPosition->y + a_pBoundaryDimensions->y;
	m_fVertexCoordinates[coordinateIndex++] = a_pBoundaryPosition->z + a_pBoundaryDimensions->z;
	// Back left vertex.
	m_fVertexCoordinates[coordinateIndex++] = a_pBoundaryPosition->x + a_pBoundaryDimensions->x;
	m_fVertexCoordinates[coordinateIndex++] = a_pBoundaryPosition->y + a_pBoundaryDimensions->y;
	m_fVertexCoordinates[coordinateIndex++] = a_pBoundaryPosition->z - a_pBoundaryDimensions->z;
	// Back right vertex.
	m_fVertexCoordinates[coordinateIndex++] = a_pBoundaryPosition->x - a_pBoundaryDimensions->x;
	m_fVertexCoordinates[coordinateIndex++] = a_pBoundaryPosition->y + a_pBoundaryDimensions->y;
	m_fVertexCoordinates[coordinateIndex++] = a_pBoundaryPosition->z - a_pBoundaryDimensions->z;
	// Front right vertex.
	m_fVertexCoordinates[coordinateIndex++] = a_pBoundaryPosition->x - a_pBoundaryDimensions->x;
	m_fVertexCoordinates[coordinateIndex++] = a_pBoundaryPosition->y + a_pBoundaryDimensions->y;
	m_fVertexCoordinates[coordinateIndex++] = a_pBoundaryPosition->z + a_pBoundaryDimensions->z;
	m_uiCoordinatesCount = sizeof(m_fVertexCoordinates) / sizeof(float);
	m_iLineDrawCount = sizeof(m_fVertexCoordinates) / sizeof(float) / m_uiCoordinatesPerVertex;
}

template <typename TVector>
void Boundary<TVector>::SetupRenderingBuffers() {
	// Generate buffer objects.
	glGenVertexArrays(1, &linesVAO);
	glGenBuffers(1, &linesVBO);

	// Bind a buffer object to fill with data.
	glBindVertexArray(linesVAO);
	GLenum bufferType = GL_ARRAY_BUFFER;
	glBindBuffer(bufferType, linesVBO);

	// Fill the vertex buffer object with the line's vertex data.
	glBufferData(bufferType, sizeof(m_fVertexCoordinates), m_fVertexCoordinates, GL_STATIC_DRAW);
	int vertexAttributeIndex = 0;
	// Sets what type of vertices will be passed to the relevant vertex shader.
	glVertexAttribPointer(vertexAttributeIndex, m_uiCoordinatesPerVertex, GL_FLOAT, GL_FALSE, m_uiCoordinatesPerVertex * sizeof(float), (void*)0);
	glEnableVertexAttribArray(vertexAttributeIndex);

	// Unbinds the vertex buffer object.
	glBindBuffer(bufferType, 0);
	// Unbinds the vertex array object.
	glBindVertexArray(0);
}

#endif // !BOUNDARY_H

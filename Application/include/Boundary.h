//////////////////////////////
// Author: Liam Bansal
// Date Created: 21/05/2021
//////////////////////////////

#ifndef BOUNDARY_H
#define BOUNDARY_H

// Represents a volume of space.
template <typename TVector>
class Boundary
{
public:
	Boundary();
	Boundary(TVector a_newPosition,
		TVector a_newDimensions);
	Boundary(TVector* a_pNewPosition,
		TVector a_newDimensions);
	~Boundary();

	// Returns true if a point is located within a boundary.
	bool Contains(const TVector& a_position) const;
	// Returns true if two boundaries share the same volume of space.
	bool Overlaps(Boundary<TVector> a_otherBoundary) const;

	inline void SetPosition(TVector* a_pNewPosition);
	inline void SetDimensions(TVector a_newDimensions);

	inline const TVector* GetPosition() const;
	inline const TVector GetDimensions() const;

private:
	TVector* m_pPosition;
	// Measured outward from position in order of width, height and length.
	TVector m_dimensions;
};

template <typename TVector>
Boundary<TVector>::Boundary() : m_pPosition(new TVector(1.0f)),
m_dimensions(1.0f)
{}

template <typename TVector>
Boundary<TVector>::Boundary(TVector a_newPosition,
	TVector a_newDimensions) : m_pPosition(new TVector(a_newPosition)),
	m_dimensions(a_newDimensions)
{}

template <typename TVector>
Boundary<TVector>::Boundary(TVector* a_pNewPosition,
	TVector a_newDimensions) : m_pPosition(a_pNewPosition),
	m_dimensions(a_newDimensions)
{}

template <typename TVector>
Boundary<TVector>::~Boundary()
{}

template <typename TVector>
// Is a 3D position within the boundary's dimensions?
bool Boundary<TVector>::Contains(const TVector& a_position) const
{
	return (a_position.x >= m_pPosition->x - m_dimensions.x &&
		a_position.x <= m_pPosition->x + m_dimensions.x &&
		a_position.y >= m_pPosition->y - m_dimensions.y &&
		a_position.y <= m_pPosition->y + m_dimensions.y &&
		a_position.z >= m_pPosition->z - m_dimensions.z &&
		a_position.z <= m_pPosition->z + m_dimensions.z);
}

template <typename TVector>
// Returns true if two boundaries share the same volume of space.
bool Boundary<TVector>::Overlaps(Boundary a_otherBoundary) const
{
	return (a_otherBoundary.GetPosition()->x - a_otherBoundary.GetDimensions().x <= m_pPosition->x + m_dimensions.x &&
		a_otherBoundary.GetPosition()->x + a_otherBoundary.GetDimensions().x >= m_pPosition->x - m_dimensions.x ||
		a_otherBoundary.GetPosition()->y - a_otherBoundary.GetDimensions().y <= m_pPosition->y + m_dimensions.y &&
		a_otherBoundary.GetPosition()->y + a_otherBoundary.GetDimensions().y >= m_pPosition->y - m_dimensions.y ||
		a_otherBoundary.GetPosition()->z - a_otherBoundary.GetDimensions().z <= m_pPosition->z + m_dimensions.z &&
		a_otherBoundary.GetPosition()->z + a_otherBoundary.GetDimensions().z >= m_pPosition->z - m_dimensions.z);
}

template <typename TVector>
void Boundary<TVector>::SetPosition(TVector* a_pNewPosition)
{
	m_pPosition = a_pNewPosition;
}

template <typename TVector>
void Boundary<TVector>::SetDimensions(TVector a_newDimensions)
{
	m_dimensions = a_newDimensions;
}

template <typename TVector>
const TVector* Boundary<TVector>::GetPosition() const
{
	return m_pPosition;
}

template <typename TVector>
const TVector Boundary<TVector>::GetDimensions() const
{
	return m_dimensions;
}

#endif // !BOUNDARY_H

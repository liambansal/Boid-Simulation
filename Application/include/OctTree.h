//////////////////////////////
// Author: Liam Bansal
// Date Created: 21/05/2021
//////////////////////////////

#ifndef OCT_TREE_H
#define OCT_TREE_H

// Header includes.
#include "Boundary.h"
#include <map>
#include <vector>

template <typename TObject, typename TPosition>
class OctTree
{
public:
	// Digits represent x, y and z coordinates.
	enum SUB_TREE_POSITIONS
	{
		SUB_TREE_POSITIONS_000,
		SUB_TREE_POSITIONS_001,
		SUB_TREE_POSITIONS_101,
		SUB_TREE_POSITIONS_100,
		SUB_TREE_POSITIONS_010,
		SUB_TREE_POSITIONS_011,
		SUB_TREE_POSITIONS_111,
		SUB_TREE_POSITIONS_110,
		SUB_TREE_POSITIONS_COUNT
	};

	OctTree(unsigned int a_capacity,
		Boundary a_boundary);
	~OctTree()
	{}

	bool InsertObject(TObject* a_pObject,
		const TPosition& a_rPosition);
	void SubDivide();
	void Query(Boundary a_queryVolume,
		std::vector<TObject*>& a_rContainedEntities) const;

	inline const Boundary& GetBoundary() const;

private:
	// Number of objects held within a boundary before it subdivided.
	unsigned int m_uiCapacity;
	bool m_bSubdivided;
	// Measured width, height and depth.
	Boundary m_boundary;
	std::map<TObject*, const TPosition*> m_objects;
	OctTree* m_pSubTrees[8];
};

template <typename TObject, typename TPosition>
OctTree<TObject, TPosition>::OctTree(unsigned int a_capacity,
	Boundary a_boundary) : m_uiCapacity(a_capacity),
	m_bSubdivided(false),
	m_boundary(a_boundary),
	m_objects(),
	m_pSubTrees()
{}

template <typename TObject, typename TPosition>
bool OctTree<TObject, TPosition>::InsertObject(TObject* a_pObject,
	const TPosition& a_rPosition)
{
	if (!a_pObject)
	{
		return false;
	}

	// Check that the entities position is within the boundary.
	if (!m_boundary.Contains(a_rPosition))
	{
		return false;
	}

	if (m_objects.size() < m_uiCapacity)
	{
		m_objects.insert(std::pair<TObject*, const glm::vec4*>(a_pObject, &a_rPosition));
		return true;
	}
	else
	{
		if (!m_bSubdivided)
		{
			SubDivide();
		}

		// Try adding entity to one of the sub trees.
		if (m_pSubTrees[SUB_TREE_POSITIONS_000]->InsertObject(a_pObject, a_rPosition))
		{
			return true;
		}
		else if (m_pSubTrees[SUB_TREE_POSITIONS_001]->InsertObject(a_pObject, a_rPosition))
		{
			return true;
		}
		else if (m_pSubTrees[SUB_TREE_POSITIONS_101]->InsertObject(a_pObject, a_rPosition))
		{
			return true;
		}
		else if (m_pSubTrees[SUB_TREE_POSITIONS_100]->InsertObject(a_pObject, a_rPosition))
		{
			return true;
		}
		else if (m_pSubTrees[SUB_TREE_POSITIONS_010]->InsertObject(a_pObject, a_rPosition))
		{
			return true;
		}
		else if (m_pSubTrees[SUB_TREE_POSITIONS_011]->InsertObject(a_pObject, a_rPosition))
		{
			return true;
		}
		else if (m_pSubTrees[SUB_TREE_POSITIONS_111]->InsertObject(a_pObject, a_rPosition))
		{
			return true;
		}
		else if (m_pSubTrees[SUB_TREE_POSITIONS_110]->InsertObject(a_pObject, a_rPosition))
		{
			return true;
		}
	}

	return false;
}

template <typename TObject, typename TPosition>
void OctTree<TObject, TPosition>::SubDivide()
{
	// Bottom left back.
	glm::vec3 subTreePosition000(m_boundary.GetPosition()->x - m_boundary.GetDimensions().x / 2,
		m_boundary.GetPosition()->y - m_boundary.GetDimensions().y / 2,
		m_boundary.GetPosition()->z - m_boundary.GetDimensions().z / 2);
	OctTree* subTree000 = new OctTree(m_uiCapacity,
		Boundary(subTreePosition000,
			glm::vec3(m_boundary.GetDimensions() * 0.5f)));
	m_pSubTrees[SUB_TREE_POSITIONS_000] = subTree000;
	// Bottom left forward.
	glm::vec3 subTreePosition001(m_boundary.GetPosition()->x - m_boundary.GetDimensions().x / 2,
		m_boundary.GetPosition()->y - m_boundary.GetDimensions().y / 2,
		m_boundary.GetPosition()->z + m_boundary.GetDimensions().z / 2);
	OctTree* subTree001 = new OctTree(m_uiCapacity,
		Boundary(subTreePosition001,
			glm::vec3(m_boundary.GetDimensions() * 0.5f)));
	m_pSubTrees[SUB_TREE_POSITIONS_001] = subTree001;
	// Bottom right forward.
	glm::vec3 subTreePosition101(m_boundary.GetPosition()->x + m_boundary.GetDimensions().x / 2,
		m_boundary.GetPosition()->y - m_boundary.GetDimensions().y / 2,
		m_boundary.GetPosition()->z + m_boundary.GetDimensions().z / 2);
	OctTree* subTree101 = new OctTree(m_uiCapacity,
		Boundary(subTreePosition101,
			glm::vec3(m_boundary.GetDimensions() * 0.5f)));
	m_pSubTrees[SUB_TREE_POSITIONS_101] = subTree101;
	// Bottom right back.
	glm::vec3 subTreePosition100(m_boundary.GetPosition()->x + m_boundary.GetDimensions().x / 2,
		m_boundary.GetPosition()->y - m_boundary.GetDimensions().y / 2,
		m_boundary.GetPosition()->z - m_boundary.GetDimensions().z / 2);
	OctTree* subTree100 = new OctTree(m_uiCapacity,
		Boundary(subTreePosition100,
			glm::vec3(m_boundary.GetDimensions() * 0.5f)));
	m_pSubTrees[SUB_TREE_POSITIONS_100] = subTree100;

	// Top left back.
	glm::vec3 subTreePosition010(m_boundary.GetPosition()->x - m_boundary.GetDimensions().x / 2,
		m_boundary.GetPosition()->y + m_boundary.GetDimensions().y / 2,
		m_boundary.GetPosition()->z - m_boundary.GetDimensions().z / 2);
	OctTree* subTree010 = new OctTree(m_uiCapacity,
		Boundary(subTreePosition010,
			glm::vec3(m_boundary.GetDimensions() * 0.5f)));
	m_pSubTrees[SUB_TREE_POSITIONS_010] = subTree010;
	// Top left forward.
	glm::vec3 subTreePosition011(m_boundary.GetPosition()->x - m_boundary.GetDimensions().x / 2,
		m_boundary.GetPosition()->y + m_boundary.GetDimensions().y / 2,
		m_boundary.GetPosition()->z + m_boundary.GetDimensions().z / 2);
	OctTree* subTree011 = new OctTree(m_uiCapacity,
		Boundary(subTreePosition011,
			glm::vec3(m_boundary.GetDimensions() * 0.5f)));
	m_pSubTrees[SUB_TREE_POSITIONS_011] = subTree011;
	// Top right forward.
	glm::vec3 subTreePosition111(m_boundary.GetPosition()->x + m_boundary.GetDimensions().x / 2,
		m_boundary.GetPosition()->y + m_boundary.GetDimensions().y / 2,
		m_boundary.GetPosition()->z + m_boundary.GetDimensions().z / 2);
	OctTree* subTree111 = new OctTree(m_uiCapacity,
		Boundary(subTreePosition111,
			glm::vec3(m_boundary.GetDimensions() * 0.5f)));
	m_pSubTrees[SUB_TREE_POSITIONS_111] = subTree111;
	// Top right back.
	glm::vec3 subTreePosition110(m_boundary.GetPosition()->x + m_boundary.GetDimensions().x / 2,
		m_boundary.GetPosition()->y + m_boundary.GetDimensions().y / 2,
		m_boundary.GetPosition()->z - m_boundary.GetDimensions().z / 2);
	OctTree* subTree110 = new OctTree(m_uiCapacity,
		Boundary(subTreePosition110,
			glm::vec3(m_boundary.GetDimensions() * 0.5f)));
	m_pSubTrees[SUB_TREE_POSITIONS_110] = subTree110;
	m_bSubdivided = true;
}

template <typename TObject, typename TPosition>
void OctTree<TObject, TPosition>::Query(Boundary a_queryVolume,
	std::vector<TObject*>& a_rContainedEntities) const
{
	// Check the space being queried for objects actually intersects this oct tree.
	if (!m_boundary.Overlaps(a_queryVolume))
	{
		return;
	}
	else
	{
		for (std::pair<TObject*, const glm::vec4*> object : m_objects)
		{
			// Make sure the oct tree's objects are inside space being queried.
			if (a_queryVolume.Contains(*object.second))
			{
				a_rContainedEntities.push_back(object.first);
			}
		}

		if (m_bSubdivided)
		{
			// Add objects from subdivision boundaries.
			m_pSubTrees[SUB_TREE_POSITIONS_000]->Query(a_queryVolume, a_rContainedEntities);
			m_pSubTrees[SUB_TREE_POSITIONS_001]->Query(a_queryVolume, a_rContainedEntities);
			m_pSubTrees[SUB_TREE_POSITIONS_101]->Query(a_queryVolume, a_rContainedEntities);
			m_pSubTrees[SUB_TREE_POSITIONS_100]->Query(a_queryVolume, a_rContainedEntities);
			m_pSubTrees[SUB_TREE_POSITIONS_010]->Query(a_queryVolume, a_rContainedEntities);
			m_pSubTrees[SUB_TREE_POSITIONS_011]->Query(a_queryVolume, a_rContainedEntities);
			m_pSubTrees[SUB_TREE_POSITIONS_111]->Query(a_queryVolume, a_rContainedEntities);
			m_pSubTrees[SUB_TREE_POSITIONS_110]->Query(a_queryVolume, a_rContainedEntities);
		}
	}
}

template<typename TObject, typename TPosition>
const Boundary& OctTree<TObject, TPosition>::GetBoundary() const
{
	return m_boundary;
}

#endif // !OCT_TREE_H

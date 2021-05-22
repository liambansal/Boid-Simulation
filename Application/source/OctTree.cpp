//////////////////////////////
// Author: Liam Bansal
// Date Created: 21/05/2021
//////////////////////////////

// File's header.
#include "OctTree.h"

template <typename T>
OctTree<T>::OctTree(unsigned int a_capacity,
	Boundary a_boundary) : m_uiCapacity(a_capacity),
	m_bSubdivided(false),
	m_boundary(a_boundary),
	m_objects(),
	m_subTrees()
{}

template <typename T>
bool OctTree<T>::InsertObject(T* a_pObject,
	const glm::vec3* a_pPosition)
{
	if (!a_pObject || !a_pPosition)
	{
		return false;
	}

	// Check that the entities position is within the boundary.
	if (!m_boundary.Contains(*a_pPosition))
	{
		return false;
	}

	if (m_objects.size() < m_uiCapacity)
	{
		m_objects.push_back(std::pair<T*, glm::vec3*>(a_pObject, a_pPosition));
		return true;
	}
	else
	{
		if (!m_bSubdivided)
		{
			SubDivide();
		}

		// Try adding entity to one of the sub trees.
		if (m_subTrees[SUB_TREE_POSITIONS_000]->InsertObject(a_pObject))
		{
			return true;
		}
		else if (m_subTrees[SUB_TREE_POSITIONS_001]->InsertObject(a_pObject))
		{
			return true;
		}
		else if (m_subTrees[SUB_TREE_POSITIONS_101]->InsertObject(a_pObject))
		{
			return true;
		}
		else if (m_subTrees[SUB_TREE_POSITIONS_100]->InsertObject(a_pObject))
		{
			return true;
		}
		else if (m_subTrees[SUB_TREE_POSITIONS_010]->InsertObject(a_pObject))
		{
			return true;
		}
		else if (m_subTrees[SUB_TREE_POSITIONS_011]->InsertObject(a_pObject))
		{
			return true;
		}
		else if (m_subTrees[SUB_TREE_POSITIONS_111]->InsertObject(a_pObject))
		{
			return true;
		}
		else if (m_subTrees[SUB_TREE_POSITIONS_110]->InsertObject(a_pObject))
		{
			return true;
		}
	}
}

template <typename T>
void OctTree<T>::SubDivide()
{
	// Bottom left back.
	glm::vec3 subTreePosition000(m_boundary.GetPosition().x - m_boundary.GetDimensions().x / 2,
		m_boundary.GetPosition().y - m_boundary.GetDimensions().y / 2,
		m_boundary.GetPosition().z - m_boundary.GetDimensions().z / 2);
	OctTree* subTree000 = new OctTree(m_uiCapacity,
		Boundary(subTreePosition000,
			glm::vec3(m_boundary.GetDimensions() * 0.5f)));
	m_subTrees[SUB_TREE_POSITIONS_000] = subTree000;
	// Bottom left forward.
	glm::vec3 subTreePosition001(m_boundary.GetPosition().x - m_boundary.GetDimensions().x / 2,
		m_boundary.GetPosition().y - m_boundary.GetDimensions().y / 2,
		m_boundary.GetPosition().z + m_boundary.GetDimensions().z / 2);
	OctTree* subTree001 = new OctTree(m_uiCapacity,
		Boundary(subTreePosition001,
			glm::vec3(m_boundary.GetDimensions() * 0.5f)));
	m_subTrees[SUB_TREE_POSITIONS_001] = subTree001;
	// Bottom right forward.
	glm::vec3 subTreePosition101(m_boundary.GetPosition().x + m_boundary.GetDimensions().x / 2,
		m_boundary.GetPosition().y - m_boundary.GetDimensions().y / 2,
		m_boundary.GetPosition().z + m_boundary.GetDimensions().z / 2);
	OctTree* subTree101 = new OctTree(m_uiCapacity,
		Boundary(subTreePosition101,
			glm::vec3(m_boundary.GetDimensions() * 0.5f)));
	m_subTrees[SUB_TREE_POSITIONS_101] = subTree101;
	// Bottom right back.
	glm::vec3 subTreePosition100(m_boundary.GetPosition().x + m_boundary.GetDimensions().x / 2,
		m_boundary.GetPosition().y - m_boundary.GetDimensions().y / 2,
		m_boundary.GetPosition().z - m_boundary.GetDimensions().z / 2);
	OctTree* subTree100 = new OctTree(m_uiCapacity,
		Boundary(subTreePosition100,
			glm::vec3(m_boundary.GetDimensions() * 0.5f)));
	m_subTrees[SUB_TREE_POSITIONS_100] = subTree100;

	// Top left back.
	glm::vec3 subTreePosition010(m_boundary.GetPosition().x - m_boundary.GetDimensions().x / 2,
		m_boundary.GetPosition().y + m_boundary.GetDimensions().y / 2,
		m_boundary.GetPosition().z - m_boundary.GetDimensions().z / 2);
	OctTree* subTree010 = new OctTree(m_uiCapacity,
		Boundary(subTreePosition010,
			glm::vec3(m_boundary.GetDimensions() * 0.5f)));
	m_subTrees[SUB_TREE_POSITIONS_010] = subTree010;
	// Top left forward.
	glm::vec3 subTreePosition011(m_boundary.GetPosition().x - m_boundary.GetDimensions().x / 2,
		m_boundary.GetPosition().y + m_boundary.GetDimensions().y / 2,
		m_boundary.GetPosition().z + m_boundary.GetDimensions().z / 2);
	OctTree* subTree011 = new OctTree(m_uiCapacity,
		Boundary(subTreePosition011,
			glm::vec3(m_boundary.GetDimensions() * 0.5f)));
	m_subTrees[SUB_TREE_POSITIONS_011] = subTree011;
	// Top right forward.
	glm::vec3 subTreePosition111(m_boundary.GetPosition().x + m_boundary.GetDimensions().x / 2,
		m_boundary.GetPosition().y + m_boundary.GetDimensions().y / 2,
		m_boundary.GetPosition().z + m_boundary.GetDimensions().z / 2);
	OctTree* subTree111 = new OctTree(m_uiCapacity,
		Boundary(subTreePosition111,
			glm::vec3(m_boundary.GetDimensions() * 0.5f)));
	m_subTrees[SUB_TREE_POSITIONS_111] = subTree111;
	// Top right back.
	glm::vec3 subTreePosition110(m_boundary.GetPosition().x + m_boundary.GetDimensions().x / 2,
		m_boundary.GetPosition().y + m_boundary.GetDimensions().y / 2,
		m_boundary.GetPosition().z - m_boundary.GetDimensions().z / 2);
	OctTree* subTree110 = new OctTree(m_uiCapacity,
		Boundary(subTreePosition110,
			glm::vec3(m_boundary.GetDimensions() * 0.5f)));
	m_subTrees[SUB_TREE_POSITIONS_110] = subTree110;
	m_bSubdivided = true;
}

template <typename T>
std::vector<T*> OctTree<T>::Query(Boundary a_queryVolume)
{
	// Objects within the query volume and its surrounding oct tree sub divisions.
	std::vector<T*> containedObjects;

	if (!m_boundary.Intersects(a_queryVolume))
	{
		return containedObjects;
	}
	else
	{
		for (std::pair<T*, glm::vec3*> object : m_objects)
		{
			if (a_queryVolume.Contains(object->second))
			{
				containedObjects.push_back(object->first);
			}
		}

		if (m_bSubdivided)
		{
			// TODO: add objects from subdivision boundaries
		}

		return containedObjects;
	}
}
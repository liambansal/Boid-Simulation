//////////////////////////////
// Author: Liam Bansal
// Date Created: 21/05/2021
//////////////////////////////

// File's header.
#include "OctTree.h"
#include "Entity.h"
#include "TransformComponent.h"

OctTree::OctTree(unsigned int a_capacity,
	Boundary a_boundary) : m_uiCapacity(a_capacity),
	m_bSubdivided(false),
	m_boundary(a_boundary),
	m_entities()
{}

void OctTree::InsertObject(Entity* a_pEntity)
{
	TransformComponent* entityTransform = static_cast<TransformComponent*>(a_pEntity->GetComponentOfType(COMPONENT_TYPE_TRANSFORM));

	if (!entityTransform)
	{
		return;
	}

	// Check that the entities position is within the boundary.
	if (!m_boundary.Contains(entityTransform->GetMatrix()[MATRIX_ROW_POSITION_VECTOR]))
	{
		return;
	}

	if (m_entities.size() < m_uiCapacity)
	{
		m_entities.push_back(a_pEntity);
	}
	else
	{
		if (!m_bSubdivided)
		{
			SubDivide();
		}

		// TODO: Add point to children.
	}
}

void OctTree::SubDivide()
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
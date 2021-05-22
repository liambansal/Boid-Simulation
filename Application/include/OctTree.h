//////////////////////////////
// Author: Liam Bansal
// Date Created: 21/05/2021
//////////////////////////////

#ifndef OCT_TREE_H
#define OCT_TREE_H

// Header includes.
#include "Boundary.h"
#include <vector>

// Forward declarations.
class Entity;

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

	bool InsertObject(Entity* a_pEntity);
	void SubDivide();

private:
	// Number of objects held within a boundary before it subdivided.
	unsigned int m_uiCapacity;
	bool m_bSubdivided;
	// Measured width, height and depth.
	Boundary m_boundary;
	std::vector<Entity*> m_entities;
	OctTree* m_subTrees[8];
};

#endif // !OCT_TREE_H

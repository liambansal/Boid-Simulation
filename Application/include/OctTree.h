//////////////////////////////
// Author: Liam Bansal
// Date Created: 21/05/2021
//////////////////////////////

#ifndef OCT_TREE_H
#define OCT_TREE_H

// Header includes.
#include "Boundary.h"
#include "glm/glm.hpp"
#include <map>
#include <vector>

template <typename T>
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

	bool InsertObject(T* a_pObject,
		const glm::vec3* a_pPosition);
	void SubDivide();
	std::vector<T*> Query(Boundary a_queryVolume);

private:
	// Number of objects held within a boundary before it subdivided.
	unsigned int m_uiCapacity;
	bool m_bSubdivided;
	// Measured width, height and depth.
	Boundary m_boundary;
	std::map<T*, glm::vec3*> m_objects;
	OctTree* m_subTrees[8];
};

#endif // !OCT_TREE_H

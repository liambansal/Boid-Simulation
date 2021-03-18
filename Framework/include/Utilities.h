//////////////////////////////
// Author: Liam Bansal
// Date Created: 18/03/2021
//////////////////////////////

#ifndef UTILITIES_H
#define UTILITIES_H

#include "glm/glm.hpp"

// Storage class for utility type functions that don't fit into other classes.
class Utilities
{
public:
	// Returns an almost random numerical value between two ranges.
	template<typename T>
	static inline float RandomRange(T a_lowerRange, T a_upperRange);
};

// Returns an almost random numerical value between two ranges.
template<typename T>
float Utilities::RandomRange(T a_lowerRange, T a_upperRange)
{
	return rand() % (glm::abs(a_lowerRange - a_upperRange)) + a_lowerRange;
}

#endif // !UTILITIES_H

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
	// Returns a semi-random number between two ranges.
	template<typename T>
	static inline T RandomRange(T a_lowerRange,
		T a_upperRange);
};

// Returns a semi-random number between two ranges.
template<typename T>
T Utilities::RandomRange(T a_lowerRange,
	T a_upperRange)
{
	if (a_lowerRange == 0 && a_upperRange == 0)
	{
		return 0;
	}

	T wholeNumber = rand() % glm::abs((glm::int32)a_lowerRange - (glm::int32)a_upperRange) + a_lowerRange;
	// 1000 produces decent random values.
	unsigned int maxDecimalValue = 1000;
	// Produce the decimal part of the semi-random number.
	T decimals = rand() % maxDecimalValue;
	unsigned int significantFigures = log10(decimals) + 1;
	decimals *= 1 / pow(10, significantFigures);
	return wholeNumber += decimals;
}

#endif // !UTILITIES_H

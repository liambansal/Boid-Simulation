//////////////////////////////
// Author: Liam Bansal
// Date Created: 18/03/2021
//////////////////////////////

#ifndef UTILITIES_H
#define UTILITIES_H

// Header includes.
#include "glm/glm.hpp"

/// <summary>
/// Contains miscellaneous functions that have a general purpose.
/// </summary>
class Utilities {
public:
	static const float Half;
	static const float Third;
	static const float Quarter;

	static const glm::vec3 Right;
	static const glm::vec3 Left;
	static const glm::vec3 Up;
	static const glm::vec3 Down;
	static const glm::vec3 Forward;
	static const glm::vec3 Back;

	/// <summary>
	/// Returns a semi-random number within a specified range.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="a_lowerRange"> The minimum digit that can be returned. </param>
	/// <param name="a_upperRange"> The maximum digit that can be returned. </param>
	/// <returns> A semi-randomlly generated number. </returns>
	template<typename T>
	static inline T RandomRange(T a_lowerRange,
		T a_upperRange);
};

template<typename T>
T Utilities::RandomRange(T a_lowerRange,
	T a_upperRange) {
	if (a_lowerRange == 0 && a_upperRange == 0) {
		return 0;
	}

	T wholeNumber = rand() % glm::abs((glm::int32)a_lowerRange - (glm::int32)a_upperRange) + a_lowerRange;
	// 1000 produces decent random values because its a high value.
	unsigned int maxDecimalValue = 1000;
	// Produces the decimal part of the semi-random number.
	T decimals = rand() % maxDecimalValue;
	unsigned int significantFigures = log10(decimals) + 1;
	decimals *= 1 / pow(10, significantFigures);
	return wholeNumber += decimals;
}

#endif // !UTILITIES_H

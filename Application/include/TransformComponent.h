//////////////////////////////
// Author: Liam Bansal
// Date Created: 18/02/2021
//////////////////////////////

#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

// Header includes.
#include "Component.h"
#include "glm/ext.hpp"

// Forward declarations.
class Framework;

/// <summary>
/// Allows an entity to store position and rotation data.
/// </summary>
class TransformComponent : public Component
{
public:
	enum MATRIX_ROW
	{
		MATRIX_ROW_RIGHT_VECTOR,
		MATRIX_ROW_UP_VECTOR,
		MATRIX_ROW_FORWARD_VECTOR,
		MATRIX_ROW_POSITION_VECTOR,
		MATRIX_ROW_COUNT
	};

	TransformComponent(Entity* a_pOwner);
	TransformComponent(Entity* a_pOwner,
		TransformComponent& a_rTransformToCopy);
	~TransformComponent()
	{}

	virtual void Update(float a_fDeltaTime)
	{}
	virtual void Draw(Framework* a_pRenderingFramework)
	{}

	/// <summary>
	/// Sets a row within the transform component's position and rotation matrix.
	/// </summary>
	/// <param name="a_row"> The matrix row to change the value of. </param>
	/// <param name="a_vector"> The new value to set. </param>
	void SetMatrixRow(MATRIX_ROW a_row, glm::vec3 a_vector);

	inline const glm::mat4& GetMatrix() const;
	/// <summary>
	/// Returns a single row from the transform component's position and rotation matrix.
	/// </summary>
	/// <param name="a_row"> The type of row to get. </param>
	/// <returns> A matrix row. </returns>
	inline const glm::vec4& GetMatrixRow(MATRIX_ROW a_row) const;
	inline glm::vec3* GetPosition() const;

private:
	glm::mat4 m_matrix;
};

const glm::mat4& TransformComponent::GetMatrix() const
{
	return m_matrix;
}

const glm::vec4& TransformComponent::GetMatrixRow(MATRIX_ROW a_row) const
{
	return m_matrix[a_row];
}

glm::vec3* TransformComponent::GetPosition() const
{
	return &(glm::vec3&)m_matrix[TransformComponent::MATRIX_ROW_POSITION_VECTOR];
}

#endif // !TRANSFORM_COMPONENT_H.

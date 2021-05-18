//////////////////////////////
// Author: Liam Bansal
// Date Created: 18/02/2021
//////////////////////////////

#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

// Header includes.
#include "Component.h"
#include "glm/ext.hpp"

class Framework;

enum MATRIX_ROW
{
	MATRIX_ROW_RIGHT_VECTOR,
	MATRIX_ROW_UP_VECTOR,
	MATRIX_ROW_FORWARD_VECTOR,
	MATRIX_ROW_POSITION_VECTOR,
	MATRIX_ROW_COUNT
};

class TransformComponent : public Component
{
public:
	TransformComponent(Entity* a_pOwner);
	TransformComponent(Entity* a_pOwner,
		TransformComponent& a_rTransformToCopy);
	~TransformComponent()
	{}

	virtual void Update(float a_deltaTime)
	{}
	virtual void Draw(Framework* a_pRenderingFramework)
	{}

	void SetMatrixRow(MATRIX_ROW a_row, glm::vec3 a_vector);
	inline const glm::mat4& GetMatrix() const;

private:
	inline const glm::vec3 GetMatrixRow(MATRIX_ROW a_row) const;

	glm::mat4 m_matrix;
};

const glm::mat4& TransformComponent::GetMatrix() const
{
	return m_matrix;
}

const glm::vec3 TransformComponent::GetMatrixRow(MATRIX_ROW a_row) const
{
	return m_matrix[a_row];
}

#endif // !TRANSFORM_COMPONENT_H.

//////////////////////////////
// Author: Liam Bansal
// Date Created: 18/02/2021
//////////////////////////////

// File's header.
#include "TransformComponent.h"
#include "LearnOpenGL/shader.h"

// Typedefs.
typedef Component Parent;

TransformComponent::TransformComponent(Entity* a_pOwner) : Parent(a_pOwner),
m_matrix(glm::mat4(1.0f)) {
	m_componentType = COMPONENT_TYPE_TRANSFORM;
}

TransformComponent::TransformComponent(Entity* a_pOwner,
	TransformComponent& a_rTransformToCopy) : Parent(a_pOwner),
	m_matrix(a_rTransformToCopy.m_matrix) {
	m_componentType = a_rTransformToCopy.m_componentType;
}

void TransformComponent::SetMatrixRow(MATRIX_ROW a_row, glm::vec3 a_vector) {
	m_matrix[a_row] = glm::vec4(a_vector,
		(a_row == MATRIX_ROW_POSITION_VECTOR ? 1.0f : 0.0f));
}

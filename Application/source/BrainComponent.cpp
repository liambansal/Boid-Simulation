//////////////////////////////
// Author: Liam Bansal
// Date Created: 21/04/2021
//////////////////////////////

// File's header.
#include "BrainComponent.h"
#include "ColliderComponent.h"
#include "Entity.h"
#include "LearnOpenGL/shader.h"
#include "OctTree.h"
#include "Scene.h"
#include "TransformComponent.h"
#include "Utilities.h"

// Static variable initializations.
float BrainComponent::ms_fSeparationForce = 0.4f;
float BrainComponent::ms_fAlignmentForce = 0.2f;
float BrainComponent::ms_fCohesionForce = 0.6f;
float BrainComponent::ms_fWanderForce = 0.5f;

BrainComponent::BrainComponent(Entity* a_pOwner,
	Scene* a_pScene) : Component(a_pOwner),
	m_uiNeighbourCount(0),
	mc_fSpeed(1.0f),
	mc_fMaximumVelocity(1.5f),
	mc_fMaximumNeighbourDistance(8.0f),
	m_fLastUpdate(0.0f),
	m_currentMovementVelocity(0.0f),
	m_newMovementVelocity(0.0f),
	m_collisionSeparationVelocity(0.0f),
	m_pScene(a_pScene),
	m_pEntityCollider(static_cast<ColliderComponent*>(a_pOwner->GetComponentOfType(COMPONENT_TYPE_COLLIDER))) {
	m_componentType = COMPONENT_TYPE_BRAIN;
}

BrainComponent::BrainComponent(Entity* a_pOwner,
	BrainComponent& a_rBrainToCopy,
	Scene* a_pScene) : Component(a_pOwner),
	mc_fSpeed(a_rBrainToCopy.mc_fSpeed),
	mc_fMaximumVelocity(a_rBrainToCopy.mc_fMaximumVelocity),
	mc_fMaximumNeighbourDistance(a_rBrainToCopy.mc_fMaximumNeighbourDistance),
	m_uiNeighbourCount(a_rBrainToCopy.m_uiNeighbourCount),
	m_fLastUpdate(a_rBrainToCopy.m_fLastUpdate),
	m_currentMovementVelocity(a_rBrainToCopy.m_currentMovementVelocity),
	m_newMovementVelocity(a_rBrainToCopy.m_newMovementVelocity),
	m_collisionSeparationVelocity(a_rBrainToCopy.m_collisionSeparationVelocity),
	m_pScene(a_pScene),
	m_pEntityCollider(static_cast<ColliderComponent*>(a_pOwner->GetComponentOfType(COMPONENT_TYPE_COLLIDER))) {
	m_componentType = a_rBrainToCopy.m_componentType;
}

void BrainComponent::Update(float a_fDeltaTime) {
	m_fLastUpdate += a_fDeltaTime;
	const float updateStep = 0.15f;

	// Get this components owner entity.
	Entity* pOwnerEntity = GetEntity();

	if (!pOwnerEntity) {
		return;
	}

	TransformComponent* pOwnerTransform = static_cast<TransformComponent*>(pOwnerEntity->GetComponentOfType(COMPONENT_TYPE_TRANSFORM));

	if (!pOwnerTransform) {
		return;
	}

	glm::vec3 currentPosition = (glm::vec3)pOwnerTransform->GetMatrixRow(TransformComponent::MATRIX_ROW_POSITION_VECTOR);
	glm::vec3 forwardDirection = (glm::vec3)pOwnerTransform->GetMatrixRow(TransformComponent::MATRIX_ROW_FORWARD_VECTOR);

	if (m_fLastUpdate >= updateStep) {
		m_fLastUpdate = 0.0f;
		pEntityVector nearbyEntities = GetNeighbouringEntities(currentPosition);
		m_uiNeighbourCount = nearbyEntities.size();
		// Calculate our boid's behaviour.
		m_newMovementVelocity += CalculateNewMovementVelocity(currentPosition,
			forwardDirection,
			nearbyEntities);
	}

	// Check for any collisions.
	if (m_pEntityCollider && m_pEntityCollider->IsColliding()) {
		CalculateCollisionVelocity(currentPosition);
		m_currentMovementVelocity = m_collisionSeparationVelocity;
	} else {
		if (glm::length(m_collisionSeparationVelocity) != 0) {
			// Reset the collision separation velocity now that all collisions are over.
			m_collisionSeparationVelocity = glm::vec3(0.0f);
		}

		m_currentMovementVelocity = m_newMovementVelocity;
	}

	glm::vec3 nextPosition = currentPosition + m_currentMovementVelocity * a_fDeltaTime;

	// Check if the entity will move outside the scene's bounds.
	if (m_pScene && !m_pScene->GetOctTree().GetBoundary().Contains(nextPosition)) {
		// Reverse the boids movement directions.
		m_currentMovementVelocity = -m_currentMovementVelocity;
		m_newMovementVelocity = -m_newMovementVelocity;
	}

	m_currentMovementVelocity = glm::clamp(m_currentMovementVelocity,
		glm::vec3(-mc_fMaximumVelocity, -mc_fMaximumVelocity, -mc_fMaximumVelocity),
		glm::vec3(mc_fMaximumVelocity, mc_fMaximumVelocity, mc_fMaximumVelocity));
	currentPosition += m_currentMovementVelocity * a_fDeltaTime;
	// Update the entity's transform matrix.
	UpdateMatrix(pOwnerTransform,
		&currentPosition,
		&m_currentMovementVelocity);
}

glm::vec3 BrainComponent::CalculateSeekVelocity(const glm::vec3& a_rTargetPosition,
	const glm::vec3& a_rCurrentPosition) const {
	glm::vec3 targetDirection(a_rTargetPosition - a_rCurrentPosition);

	if (glm::length(targetDirection) > 0.0f) {
		targetDirection = glm::normalize(targetDirection);
	}

	// Calculate new velocity.
	glm::vec3 seekVelocity = targetDirection * mc_fSpeed - m_currentMovementVelocity;
	return seekVelocity;
}

glm::vec3 BrainComponent::CalculateWanderVelocity(const glm::vec3& a_rForwardDirection,
	const glm::vec3& a_rCurrentPosition) const {
	// Greater values result in wider turning angles.
	const float projectionDistance = 4.0f;
	// Project a point in front for the center of a sphere.
	glm::vec3 wonderPointOrigin = a_rCurrentPosition + a_rForwardDirection * projectionDistance;
	// Effects radius of sphere to cast forward.
	const float wanderRadius = 2.0f;
	// Find a random point on a forward casted sphere.
	glm::vec3 randomPoint = glm::sphericalRand(wanderRadius);
	// Gets random point in space ahead of the boid.
	glm::vec3 wanderPoint = wonderPointOrigin + randomPoint;
	// Direction to move along
	glm::vec3 targetDirection = glm::normalize(wanderPoint - wonderPointOrigin) * wanderRadius;
	// Calculate final target position
	wanderPoint = wonderPointOrigin + targetDirection;
	const float jitter = 0.5f;
	// Add jitter to movement
	wanderPoint += glm::sphericalRand(jitter);
	// Seek to the wander point
	return CalculateSeekVelocity(wanderPoint, a_rCurrentPosition);
}

glm::vec3 BrainComponent::CalculateSeparationVelocity(glm::vec3 a_separationVelocity,
	glm::vec3 a_targetVector,
	unsigned int a_uiNeighbourCount) const {
	a_separationVelocity += a_targetVector;

	if (glm::length(a_separationVelocity) > 0.0f && a_uiNeighbourCount > 0) {
		// Average the separation force
		a_separationVelocity /= a_uiNeighbourCount;
		// Only normalise vector with length greater than zero
		a_separationVelocity = glm::normalize(a_separationVelocity);
	}

	return a_separationVelocity;
}

glm::vec3 BrainComponent::CalculateAlignmentVelocity(glm::vec3 a_alignmentVelocity,
	glm::vec3 a_targetVector) const {
	a_alignmentVelocity += a_targetVector;

	if (glm::length(a_alignmentVelocity) > 0.0f && m_uiNeighbourCount > 0) {
		a_alignmentVelocity /= m_uiNeighbourCount;
		a_alignmentVelocity = glm::normalize(a_alignmentVelocity);
	}

	return a_alignmentVelocity;
}

glm::vec3 BrainComponent::CalculateCohesionVelocity(glm::vec3 a_cohesionVelocity,
	glm::vec3 a_cohesionChange,
	glm::vec3 a_currentPosition) const {
	a_cohesionVelocity += a_cohesionChange;

	if (glm::length(a_cohesionVelocity) > 0.0f && m_uiNeighbourCount > 0) {
		a_cohesionVelocity /= m_uiNeighbourCount;
		a_cohesionVelocity = glm::normalize(a_cohesionVelocity - a_currentPosition);
	}

	return a_cohesionVelocity;
}

glm::vec3 BrainComponent::CalculateNewMovementVelocity(glm::vec3& a_rCurrentPosition,
	glm::vec3& a_rEntityForward,
	pEntityVector nearbyEntities) const {
	// Get the component's owner entity.
	const Entity* pOwnerEntity = GetEntity();

	if (!pOwnerEntity) {
		return glm::vec3(0);
	}

	// Behavioural forces.
	glm::vec3 seperationVelocity(0.0f);
	glm::vec3 alignmentVelocity(0.0f);
	glm::vec3 cohesionVelocity(0.0f);

	// Loop over all of the nearvy entities in scene.
	for (const Entity* entity : nearbyEntities) {
		if (!entity ||
			// Check if an entity found itself.
			entity->GetID() == pOwnerEntity->GetID() ||
			entity->GetTag() != "Boid") {
			continue;
		}

		const TransformComponent* pNearbyTransform = static_cast<TransformComponent*>(entity->GetComponentOfType(COMPONENT_TYPE_TRANSFORM));
		const BrainComponent* pNearbyBrain = static_cast<BrainComponent*>(entity->GetComponentOfType(COMPONENT_TYPE_BRAIN));

		if (!pNearbyTransform || !pNearbyBrain) {
			continue;
		}

		const glm::vec3 nearbyEntityPosition = (const glm::vec3)pNearbyTransform->GetMatrixRow(TransformComponent::MATRIX_ROW_POSITION_VECTOR);
		const glm::vec3 directionToNearbyEntity = nearbyEntityPosition - a_rCurrentPosition;
		seperationVelocity = CalculateSeparationVelocity(seperationVelocity,
			// Creates a direction that points towards the other entity if they have different positions.
			a_rCurrentPosition == nearbyEntityPosition ? GetRandomNearbyPoint(a_rCurrentPosition) - a_rCurrentPosition : -directionToNearbyEntity,
			m_uiNeighbourCount);
		alignmentVelocity = CalculateAlignmentVelocity(alignmentVelocity,
			// Add the neighbouring boid's current velocity.
			pNearbyBrain->GetVelocity());
		cohesionVelocity = CalculateCohesionVelocity(cohesionVelocity,
			directionToNearbyEntity,
			a_rCurrentPosition);
	}

	seperationVelocity *= ms_fSeparationForce;
	alignmentVelocity *= ms_fAlignmentForce;
	cohesionVelocity *= ms_fCohesionForce;
	glm::vec3 wanderVelocity = CalculateWanderVelocity(a_rEntityForward, a_rCurrentPosition) * ms_fWanderForce;
	glm::vec3 changeInMovement(wanderVelocity + cohesionVelocity + alignmentVelocity + seperationVelocity);
	return changeInMovement;
}

void BrainComponent::CalculateCollisionVelocity(const glm::vec3 a_entityPosition) {
	for (std::vector<ColliderComponent*>::const_iterator iterator = m_pEntityCollider->GetCollisions().begin();
		iterator != m_pEntityCollider->GetCollisions().end();
		++iterator) {
		Entity* pCollidingEntity = (*iterator)->GetEntity();

		if (!pCollidingEntity) {
			return;
		}

		TransformComponent* pCollidingEntityTransform = static_cast<TransformComponent*>(pCollidingEntity->GetComponentOfType(COMPONENT_TYPE_TRANSFORM));

		if (!pCollidingEntityTransform) {
			return;
		}

		glm::vec3 targetVector = (a_entityPosition == *pCollidingEntityTransform->GetPosition()) ?
			// Get nearby point encase entities spawn on top of each other.
			GetRandomNearbyPoint(a_entityPosition) - a_entityPosition :
			// Normal collision avoidance behaviour.
			a_entityPosition - *pCollidingEntityTransform->GetPosition();
		// Calculate where to move towards.
		m_collisionSeparationVelocity = CalculateSeparationVelocity(m_collisionSeparationVelocity,
			targetVector,
			m_pEntityCollider->GetCollisions().size());
	}
}

void BrainComponent::UpdateMatrix(TransformComponent* a_pTransform,
	glm::vec3* a_pPosition,
	glm::vec3* a_pForward) {
	if (!a_pTransform || !a_pPosition || !a_pForward) {
		return;
	}

	if (glm::length(*a_pForward) > 0.0f) {
		*a_pForward = glm::normalize(*a_pForward);
	}

	glm::vec3 upDirection = (glm::vec3)a_pTransform->GetMatrixRow(TransformComponent::MATRIX_ROW_UP_VECTOR);
	upDirection -= (*a_pForward) * glm::dot(*a_pForward, upDirection);

	if (glm::length(upDirection) > 0.0f) {
		glm::normalize(upDirection);
	}

	glm::vec3 rightDirection = glm::cross(upDirection, *a_pForward);

	if (glm::length(rightDirection) > 0.0f) {
		glm::normalize(rightDirection);
	}

	// Update transform matrix.
	a_pTransform->SetMatrixRow(TransformComponent::MATRIX_ROW_UP_VECTOR, upDirection);
	a_pTransform->SetMatrixRow(TransformComponent::MATRIX_ROW_RIGHT_VECTOR, rightDirection);
	a_pTransform->SetMatrixRow(TransformComponent::MATRIX_ROW_FORWARD_VECTOR, *a_pForward);
	a_pTransform->SetMatrixRow(TransformComponent::MATRIX_ROW_POSITION_VECTOR, *a_pPosition);
}

pEntityVector BrainComponent::GetNeighbouringEntities(glm::vec3 a_currentPosition) const {
	pEntityVector nearbyEntities;
	m_pScene->GetOctTree().Query(Boundary<glm::vec3>(a_currentPosition,
		glm::vec3(mc_fMaximumNeighbourDistance)),
		nearbyEntities);
	return nearbyEntities;
}

glm::vec3 BrainComponent::GetRandomNearbyPoint(glm::vec3 a_originPosition) const {
	const float absoluteRange = 1.0f;
	return glm::vec3(a_originPosition.x + Utilities::RandomRange(-absoluteRange, absoluteRange),
		a_originPosition.y + Utilities::RandomRange(-absoluteRange, absoluteRange),
		a_originPosition.z + Utilities::RandomRange(-absoluteRange, absoluteRange));
}
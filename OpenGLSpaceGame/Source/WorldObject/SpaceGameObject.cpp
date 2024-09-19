#include "SpaceGameObject.hpp"
#include "../Delta/Delta.hpp"
#include "../Delta/DeltaProvider.hpp"
#include "../Delta/DeltaTarget.hpp"
#include "../Delta/DeltaProviders/Vec3Provider.hpp"
#include "../Delta/DeltaTargets/Vec3Target.hpp"
#include "../Delta/DeltaProviders/DragProvider.hpp"
#include "../Delta/DeltaTargets/WorldObjectAngleTarget.hpp"
#include "../Delta/DeltaTargets/WorldObjectPositionTarget.hpp"
#include "../Delta/DeltaProviders/SpacegameObjectVelocityProvider.hpp"
#include "../Delta/DeltaProviders/SpacegameObjectRotationalVelocityProvider.hpp"
#include "../Model/Model.hpp"
#include "../main.hpp"

SpaceGameObject::SpaceGameObject(Model* model, glm::vec3 position, glm::vec3 velocity, glm::vec3 rotVelocity, glm::vec3 angle, glm::vec3 scale, GLuint modelViewLocation, unordered_set<tag> tags) : WorldObject(model, position, angle, scale, modelViewLocation, tags)
{
	this->velocity = velocity;
	velocityDelta = new Delta<glm::vec3>(
		new SpaceGameObjectVelocityProvider(this),
		{ new WorldObjectPositionTarget(this) },
		{ } //drag delta
	);
	this->rotationalVelocity = rotVelocity;
	rotationalVelocityDelta = new Delta<glm::vec3>(
		new SpaceGameObjectRotationalVelocityProvider(this),
		{ new WorldObjectAngleTarget(this) },
		{ } //drag delta
	);
	this->originalColorMask = model->meshes[0]->colorMask;
	this->stunnedDuration = 0;
	this->fireDelay = 0;
}

glm::vec3 SpaceGameObject::getVelocity()
{
	return velocity;
}
void SpaceGameObject::setVelocity(glm::vec3 velocity)
{
	this->velocity = velocity;
}

glm::vec3 SpaceGameObject::getRotationalVelocity()
{
	return rotationalVelocity;
}
void SpaceGameObject::setRotationalVelocity(glm::vec3 rotationalVelocity)
{
	this->rotationalVelocity = rotationalVelocity;
}

void SpaceGameObject::addDelta(Delta<glm::vec3>* delta)
{
	deltas.push_back(delta);
}

void SpaceGameObject::Tick()
{
	velocityDelta->Tick();
	rotationalVelocityDelta->Tick();
	//apply all deltas, deleting any that have expired
	for (size_t i = 0; i < deltas.size(); i++)
	{
		if (deltas.at(i)->Tick()) //returns true when expired
		{
			deltas.erase(deltas.begin() + i);
			i--;
		}
	}

	if (stunnedDuration > 0)
	{
		stunnedDuration--;
		if (stunnedDuration == 0)
		{
			model->meshes[0]->colorMask = originalColorMask;
		}
		else if (stunnedDuration % (FPS / 4) == 0)
		{
			if (stunnedDuration % (FPS / 2) == 0)
			{
				model->meshes[0]->colorMask = originalColorMask;
			}
			else
			{
				model->meshes[0]->colorMask = COLOR_FLASH;
			}
		} 
	}
	if (fireDelay > 0)
	{
		fireDelay--;
	}
}

bool SpaceGameObject::IsStunned() const
{
	return stunnedDuration > 0;
}

void SpaceGameObject::StunFor(size_t duration)
{
	if (this->stunnedDuration < duration)
	{
		this->stunnedDuration = duration;
	}
}

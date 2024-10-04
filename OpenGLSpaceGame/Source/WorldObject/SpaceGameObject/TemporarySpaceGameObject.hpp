#pragma once
#include "../SpaceGameObject.hpp"

 class TemporarySpaceGameObject : public SpaceGameObject
{
	public:
		void Tick() override;
		TemporarySpaceGameObject(Model* model, glm::vec3 position, glm::vec3 velocity, glm::vec3 rotVelocity, glm::vec3 angle, glm::vec3 scale, GLuint modelViewLocation, unordered_set<tag> tags, glm::vec4 colorMask, long long int duration);
		~TemporarySpaceGameObject();
	private:
		long long int ticksRemaining;
};
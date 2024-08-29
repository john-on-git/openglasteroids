#pragma once
#include "../WorldObject/WorldObject.hpp"

 class TemporaryWorldObject : public WorldObject
{
	public:
		void Tick() override;
		TemporaryWorldObject(Model* model, glm::vec3 position, glm::vec3 angle, glm::vec3 scale, GLuint projectionLocation, GLuint viewLocation, GLuint modelLocation, vector<tag> tags, long long int duration);
	private:
		long long int ticksRemaining;
};
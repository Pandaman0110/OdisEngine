#include "World.h"

using namespace OdisEngine;

World::World()
{

}

entity World::create_entity()
{
	return world_entities.create();
}

void World::destroy_entity(entity entity)
{
	world_entities.destroy(entity);
}

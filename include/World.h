#ifndef ODISWORLD_H
#define ODISWORLD_H

#include <entt.hpp>

namespace OdisEngine
{
	enum class entity : uint32_t {};
	typedef entt::basic_registry<entity> WorldEntities;

	class World
	{
	public:
		World();

		template <typename Type, typename...Args>
		inline decltype(auto) add_component(entity entity, Args&&... arg)
		{
			world_entities.emplace
		}

		entity create_entity();
		void destroy_entity(entity entity);
	private:
		WorldEntities world_entities;

	};
}


#endif
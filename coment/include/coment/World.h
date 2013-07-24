#ifndef __WORLD_H__
#define __WORLD_H__

#include <vector>

#include "Entity.h"
#include "Component.h"

#include "systems/EntityProcessingSystem.h"

#include "managers/TagManager.h"
#include "managers/GroupManager.h"
#include "managers/EntityManager.h"
#include "managers/SystemManager.h"
#include "managers/ManagerManager.h"
#include "managers/VariableManager.h"
#include "managers/ComponentManager.h"
#include "managers/ComponentTypeManager.h"

#include "exceptions/CompMapUnregistered.h"

namespace coment
{
	// The world that contains all of the entities and systems
	class World
	{
	public:
		// Constructor
		World();

		// Destructor
		~World();

		/* Entity functions */
		// Create a new entity
		Entity createEntity();

		// Check if entity is alive
		bool isAlive(Entity e);

		// Queue an entity for removal
		void remove(Entity e);

		// Refresh an entity
		void refresh(Entity e);

		/* Component functions */
		// Add a default constructed component to an entity
		template <typename T>
		T* addComponent(Entity e);

		// Get a component from an entity
		template <typename T>
		T* getComponent(Entity e);

		// Remove a component from an entity
		template <typename T>
		void removeComponent(Entity e);

		// Remove all components from an entity
		void removeComponents(Entity e);

		/* System functions */
		// Register a system to the system manager
		template <typename T>
		T* registerSystem(T& system);

		// Retrieve a system from the system manager
		template <typename T>
		T* getSystem();

		/* Manager functions */
		// Add a manager
		template <typename T>
		T* registerManager(T& manager);

		// Get a manager
		template <typename T>
		T* getManager();

		/* Tag functions */
		// Set tag for entity
		void setTag(Entity e, std::string);

		// Get tag from entity
		std::string getTag(Entity e);

		// Get entities by tag
		const std::vector<Entity>& getEntitiesByTag(std::string tag);

		/* World functions */
		// Must be called at the start of each loop to update refreshed/deleted entities
		void loopStart();

		// Sets the delta for this frame
		void setDelta(float delta);

		// Returns the delta
		float getDelta();

		// Update all the systems
		void update();

	protected:
		/* Internal entity functions (called on loopStart) */
		// Remove entity (called automatically on queued entities)
		void removeEntity(Entity e);

		// Refresh entity (called automatically on queued entities)
		void refreshEntity(Entity e);

	private:
		// The manager instances
		TagManager _tagManager;
		GroupManager _groupManager;
		EntityManager _entityManager;
		SystemManager _systemManager;
		ManagerManager _managerManager;
		VariableManager _variableManager;
		ComponentManager _componentManager;
		ComponentTypeManager _componentTypeManager;

		// Entities due to be removed
		std::vector<Entity> _removed;

		// Entities due to be refreshed
		std::vector<Entity> _refreshed;

		// The difference in time since the last frame
		float _delta;
	};

	// Add a manager
	template <typename T>
	T* World::registerManager(T& manager)
	{
		return _managerManager.registerManager(manager);
	}

	// Get a manager
	template <typename T>
	T* World::getManager()
	{
		return _managerManager.getManager<T>();
	}

	// Add the component to the component manager
	template <typename T>
	T* World::addComponent(Entity e)
	{
		return _componentManager.addComponent<T>(_entityManager.getEntityInfo(e));
	}

	// Register a system with the system manager
	template <typename T>
	T* World::registerSystem(T& system)
	{
		return _systemManager.registerSystem(system);
	}

	// Retrieve a system from the system manager
	template <typename T>
	T* World::getSystem()
	{
		return _systemManager.getSystem<T>();
	}

	// Get a component from an entity
	template <typename T>
	T* World::getComponent(Entity e)
	{
		return _componentManager.getComponent<T>(_entityManager.getEntityInfo(e));
	}

	// Remove a component from an entity
	template <typename T>
	void World::removeComponent(Entity e)
	{
		_componentManager.removeComponent<T>(_entityManager.getEntityInfo(e));
	}
}

#endif /* __WORLD_H__ */
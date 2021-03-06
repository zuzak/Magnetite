#ifndef _BASEENTITY_H_
#define _BASEENTITY_H_

#include "prerequisites.h"
#include "Components/ComponentEvents.h"
#include "World.h"
#include "Component.h"

namespace Magnetite {
	
	/**
	 * @class BaseEntity
	 * 
	 * BaseEntity is an umbrella class that provides the management of Entity components,
	 * Allowing any class inheriting it to easily manage it's components.
	 */
	class BaseEntity {
		
		/**
		 * Entity id.
		 */
		EntityID mID;
		
		/**
		 * Name of the entity
		 */
		String mName;
		
		/**
		 * Pointer to the world
		 */
		World* mWorld;
		
		/**
		 * Last last position update that was recived.
		 */
		Vector3 mLastPosition;
		 
		/**
		 * A list of components attached to this entity
		 */
		ComponentList mComponents;
	public:
		/**
		 * Adds a component of the given type to this entity.
		 */
		template<class T> T* addComponent() 
		{
			T *c = new T(this);
			mComponents.push_back(c);
			return c;
		}

	public:
		
		BaseEntity( World* world );
		
		virtual ~BaseEntity();
		
		/**
		 * Returns the ID of the entity
		 */
		EntityID getID() const;
		
		/**
		 * Returns the name of the entity
		 */
		String getName() const;
		
		/**
		 * Gets the type name of the entity
		 */
		virtual String getType() { return "base"; }
		
		/**
		 * Calls when the entitiy is due to spawn.
		 */
		virtual void create();
		
		/**
		 * Propagates an event through all of the child components.
		 * @param ev the event
		 */
		void fireEvent( const BaseEvent& event );
		
		/**
		 * Thinks all of the components.
		 */
		virtual void think( float dt );
		
		/**
		 * Draws all of the components
		 */
		void draw( const Component::DrawInfo& info, float dt );
		
		/**
		 * For convenience, fires a 'position updated' event.
		 */
		void updatePosition( const Vector3& newpos );
		
		/**
		 * Returns the latest position
		 */
		Vector3 getLastPosition() {
			return mLastPosition;
		}
	};
	
};

#endif
#ifndef _COLLISION_COMPONENT_H
#define _COLLISION_COMPONENT_H

#include <SFML/System.hpp>

#include <Noah/EntitySystem.h>
#include "../CollisionState.h"
#include "PositionComponent.h"
#include "PhysicsComponent.h"

class CollisionComponent;
class PhysicsComponent;
class PositionComponent;

/**
 * Collision Component System
 */
class CollisionComponentSystem : public noah::ComponentSystem <CollisionComponent>
{
  public:
    static FamilyId family_id_;

    CollisionComponentSystem( void );
    ~CollisionComponentSystem( void );
    
    void Update( GameState *state = 0 );
};


/** 
 * Collision Component
 */
class CollisionComponent : public noah::Component <CollisionComponentSystem>
{
  public:
    CollisionComponent( void );

    //noah::SafePtr<PhysicsComponent> physics_component_;
    //noah::SafePtr<PositionComponent> position_component_;
};


#endif
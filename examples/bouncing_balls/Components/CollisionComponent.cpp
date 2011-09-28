#include "CollisionComponent.h"
#include "../GridSpace.h"

/**
 * Collision Component System
 */
FamilyId CollisionComponentSystem::family_id_ = 0;

CollisionComponentSystem::CollisionComponentSystem( void ) { }

void CollisionComponentSystem::Update( GameState *state )
{
  CollisionState collision_state;

  rde::hash_map<EntityId, noah::SafePtr<CollisionComponent>>::iterator i = components_.begin();
  for ( ; i != components_.end(); ++i )
  {
    
  }
}

/**
 * Collision Component
 */
CollisionComponent::CollisionComponent( void ) : Component( "Collision" ) { }

#include "PhysicsComponent.h"

/**
 * Physics Component System
 */
FamilyId PhysicsComponentSystem::family_id_ = 0;

PhysicsComponentSystem::PhysicsComponentSystem( void ) { }

void PhysicsComponentSystem::Update( GameState *state )
{
  stdext::hash_map< EntityId, noah::SafePtr<PhysicsComponent> >::iterator i = components_.begin();
  for ( ; i != components_.end(); ++i )
  {
    i->second->BroadcastMessage( i->second->entity_, "MoveBy", i->second->velocity_ );
  }
}

/**
 *
 */
PhysicsComponent::PhysicsComponent( void ) : Component( "Physics" ), velocity_( sf::Vector2f(0.0, 0.0) ) { }
PhysicsComponent::PhysicsComponent( sf::Vector2f v ) : Component( "Physics" ), velocity_( v ) { }
PhysicsComponent::PhysicsComponent( float x, float y ) : Component( "Physics" ), velocity_( sf::Vector2f(x, y) ) { }

void PhysicsComponent::Registered( void )
{
  HandleMessage( "HasMoved", &PhysicsComponent::HasMoved );
}

void PhysicsComponent::HasMoved( noah::Message const &msg )
{
  position_ = boost::any_cast<sf::Vector2f>( msg.payload_ );
}

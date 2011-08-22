#include "PhysicsComponent.h"

struct PhysicsComponentUpdater
{
  void operator()( EntityId eid, noah::SafePtr<PhysicsComponent> component )
  { }
};

/**
 * Physics Component System
 */
FamilyId PhysicsComponentSystem::family_id_ = 0;

PhysicsComponentSystem::PhysicsComponentSystem( void )
{ }

void PhysicsComponentSystem::Update( GameState *state )
{
  stdext::hash_map<EntityId, noah::SafePtr<PhysicsComponent>>::iterator i = components_.begin();
  for ( ; i != components_.end(); ++i )
  {
    if ( i->second->position_component_ == NULL )
      continue;
    
   i->second->position_component_->UpdatePosition( i->second->velocity_ );
  }
}

void PhysicsComponentSystem::Initialize( EntityId eid, GameState *state )
{
  noah::SafePtr<PhysicsComponent> c = GetComponent( eid );

  entity_system_->FulfillDependency<PositionComponent>( eid, &(c->position_component_) );
}

/**
 *
 */
PhysicsComponent::PhysicsComponent( void )
  : velocity_( sf::Vector2f(0.0, 0.0) )
{ }

PhysicsComponent::PhysicsComponent( float x, float y )
  : velocity_( sf::Vector2f(x, y) )
{ }

PhysicsComponent::PhysicsComponent( sf::Vector2f v )
  : velocity_( v )
{ }

void PhysicsComponent::Registered( void )
{
  //WatchComponent(entity_, "Position", &PhysicsComponent::UpdatePosition);
  //WatchMessage(entity_, "Moved", &PhysicsComponent::UpdatePosition);
}

/*void PhysicsComponent::UpdatePosition( Message const &message )
{

}*/

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
    //std::cerr << "PHYSICS::UPDATE -- 1" << std::endl;
    i->second->BroadcastMessage( i->second->entity_, "MoveBy", i->second->velocity_ );
    //i->second->entity_->BroadcastMessage( "MoveBy", noah::Message(noah::MESSAGE, this, i->second->velocity_) );
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
  WatchComponent( "Position", &PhysicsComponent::SyncPosition, entity_ );
  HandleMessage( "HasMoved", &PhysicsComponent::HasMoved );
}

void PhysicsComponent::SyncPosition( noah::Message const &msg )
{
  //std::cerr << "CALLBACK -- PHYSICS::SyncPosition -- 1" << std::endl;
  if ( msg.type_ == noah::CREATE )
  {
    position_ = ((PositionComponent*)msg.sender_)->position_;
  }
}

void PhysicsComponent::HasMoved( noah::Message const &msg )
{
  sf::Vector2f p = boost::any_cast<sf::Vector2f>( msg.payload_ );
  //std::cerr << "CALLBACK -- PHYSICS::HasMoved -- 1 -- x == " << p.x << " -- y == " << p.y << std::endl;
  position_ = p;
}

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

void PhysicsComponentSystem::Update( noah::GameState *state )
{
  EntityId eid;
  PhysicsComponent *c;
  PositionComponent *position;

  float x, y, vx, vy;

  std::map<EntityId, noah::SafePtr<PhysicsComponent> >::iterator i = components_.begin();
  for ( ; i != components_.end(); ++i )
  {
    eid = i->first;
    c = i->second;
    position = c->position_component_;

    if ( position == 0 )
      continue;
   
   x = position->position_.x;
   y = position->position_.y;

   vx = c->velocity_.x;
   vy = c->velocity_.y;

   //std::cerr << vx << std::endl;

   // X
   if ( x <= 0.0 && vx < 0.0 )
    c->velocity_ = sf::Vector2f( vx * -1.0, vy );

   if ( x >= 1000.0 && vx > 0.0 )
    c->velocity_ = sf::Vector2f( vx * -1.0, vy );

   // Y
   if ( y <= 0.0  && vy < 0 )
    c->velocity_ = sf::Vector2f( vx, vy * -1.0 );

   if ( y >= 700.0 && vy > 0 )
    c->velocity_ = sf::Vector2f( vx, vy * -1.0 );

   position->UpdatePosition( c->velocity_ );
  }
}

/**
 *
 */
PhysicsComponent::PhysicsComponent( void )
  : Component( "Physics" ), velocity_( sf::Vector2f(0, 0) )
{ }

PhysicsComponent::PhysicsComponent( float x, float y )
  : Component( "Physics" ), velocity_( sf::Vector2f(x, y) )
{ }

PhysicsComponent::PhysicsComponent( sf::Vector2f v )
  : Component( "Physics" ), velocity_( v )
{ }

void PhysicsComponent::Registered( void )
{
  Requires( &position_component_ );
}

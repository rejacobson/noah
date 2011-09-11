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
  noah::Entity *e;
  PhysicsComponent *c;
  sf::Vector2f *p;
  sf::Vector2f *v;
  
  std::map<EntityId, noah::SafePtr<PhysicsComponent>>::iterator i = components_.begin();
  for ( ; i != components_.end(); ++i )
  {
    c = i->second;
    e = c->entity_;
    p = e->GetAttr<sf::Vector2f>( c->position_id_ );
    v = e->GetAttr<sf::Vector2f>( c->velocity_id_ );

    e->SetAttr( c->position_id_ , (*p) + (*v) );

    //if ( i->second->position_component_ == 0 )
    //  continue;
    
    //i->second->position_component_->UpdatePosition( i->second->velocity_ );
  }
}

/*void PhysicsComponentSystem::Initialize( noah::Entity *entity, GameState *state )
{
  noah::SafePtr<PhysicsComponent> c = GetComponent( entity->id_ );

  entity_system_->FulfillDependency<PositionComponent>( entity->id_, &(c->position_component_) );
}*/

/**
 *
 */
PhysicsComponent::PhysicsComponent( void )
  : velocity_( sf::Vector2f(0, 0) )
{ }

PhysicsComponent::PhysicsComponent( float x, float y )
  : velocity_( sf::Vector2f(x, y) )
{ }

PhysicsComponent::PhysicsComponent( sf::Vector2f v )
  : velocity_( v )
{ }

void PhysicsComponent::Registered( void )
{
  entity_->RegisterAttr( "velocity", new sf::Vector2f(velocity_) );
  entity_->RegisterAttr( "acceleration", new sf::Vector2f(0, 0) );
  entity_->RegisterAttr( "mass", new int(10) );

  position_id_ = entity_->FetchAttrId( "position" );
  velocity_id_ = entity_->FetchAttrId( "velocity" );
}


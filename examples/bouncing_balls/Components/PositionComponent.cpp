#include "PositionComponent.h"

/**
 * Position Component System
 */
FamilyId PositionComponentSystem::family_id_ = 0;

PositionComponentSystem::PositionComponentSystem( void )
{ }

void PositionComponentSystem::Update( noah::GameState *state )
{
  EntityId eid;
  PositionComponent *c;

  std::map<EntityId, noah::SafePtr<PositionComponent> >::iterator i = components_.begin();
  for ( ; i != components_.end(); ++i )
  {
    eid = i->first;
    c = i->second;

    if ( c->changed == false )
      continue;
    
   c->changed = false;

   //state->world_->UpdateEntity( eid, c->position_, c->old_position_ );
  }
}


/**
 * Position Component
 */
PositionComponent::PositionComponent( void )
  : Component( "Position" ), changed( false )
{ }

PositionComponent::PositionComponent( sf::Vector2f pos )
  : Component( "Position" ), position_( pos ), changed( false )
{ }

PositionComponent::PositionComponent( float x, float y )
  : Component( "Position" ), position_( sf::Vector2f( x, y ) ), changed( false )
{ }

void PositionComponent::UpdatePosition( float x, float y )
{
  UpdatePosition( sf::Vector2f(x, y) );
}

void PositionComponent::UpdatePosition( sf::Vector2f p )
{
  old_position_ = position_;
  position_ += p;
  changed = true;
}

void PositionComponent::Registered( void )
{
  //noah::EntitySystem::world_->RegisterEntity( entity_->id_, position_ );
}

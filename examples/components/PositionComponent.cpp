#include "PositionComponent.h"

/**
 * Position Component System
 */
FamilyId PositionComponentSystem::family_id_ = 0;

PositionComponentSystem::PositionComponentSystem( void )
{ }

void PositionComponentSystem::Initialize( EntityId eid, GameState *state )
{
  noah::SafePtr<PositionComponent> c = GetComponent( eid );

  if ( c == 0 )
    return;

  state->world_->RegisterEntity( eid, c->position_ );
}

void PositionComponentSystem::Update( GameState *state )
{
  stdext::hash_map<EntityId, noah::SafePtr<PositionComponent>>::iterator i = components_.begin();
  for ( ; i != components_.end(); ++i )
  {
    if ( i->second->changed == false )
      continue;
    
   i->second->changed = false;

   state->world_->UpdateEntity( i->first, i->second->position_, i->second->old_position_ );
  }
}

/**
 * Position Component
 */
PositionComponent::PositionComponent( void )
  : changed( false )
{ }

PositionComponent::PositionComponent( sf::Vector2f pos )
  : position_( pos ), changed( false )
{ }

PositionComponent::PositionComponent( float x, float y )
  : position_( sf::Vector2f( x, y ) ), changed( false )
{ }

void PositionComponent::UpdatePosition( float x, float y )
{
  UpdatePosition( sf::Vector2f( x, y ) );
}

void PositionComponent::UpdatePosition( sf::Vector2f p )
{
  old_position_ = position_;
  position_ += p;
  changed = true;
}
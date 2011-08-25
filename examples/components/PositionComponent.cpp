#include "PositionComponent.h"

FamilyId PositionComponentSystem::family_id_ = 0;

////////////////////////////////////////////////////////////////
PositionComponentSystem::PositionComponentSystem( void )
{ }

////////////////////////////////////////////////////////////////
void PositionComponentSystem::Initialize( EntityId eid, GameState *state )
{
  noah::SafePtr<PositionComponent> c = GetComponent( eid );

  if ( c == 0 )
    return;

  state->world_->RegisterEntity( eid, c->position_ );
}

////////////////////////////////////////////////////////////////
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


////////////////////////////////////////////////////////////////
PositionComponent::PositionComponent( void ) : changed( false ) { }
PositionComponent::PositionComponent( sf::Vector2f pos ) : position_( pos ), changed( false ) { }
PositionComponent::PositionComponent( float x, float y ) : position_( sf::Vector2f( x, y ) ), changed( false ) { }

////////////////////////////////////////////////////////////////
void PositionComponent::SetPosition( float x, float y )
{
  SetPosition( sf::Vector2f( x, y ) );
}

////////////////////////////////////////////////////////////////
void PositionComponent::SetPosition( sf::Vector2f p )
{
  old_position_ = position_;
  position_ += p;
  changed = true;
}

////////////////////////////////////////////////////////////////
void PositionComponent::Registered( void )
{
  HandleMessage( "MoveBy", &PositionComponent::MoveBy );
}

////////////////////////////////////////////////////////////////
void PositionComponent::MoveBy( noah::Message const &msg )
{
  SetPosition( position_ + boost::any_cast<sf::Vector2f>( msg.payload_ ) );
}

////////////////////////////////////////////////////////////////
void PositionComponent::MoveTo( noah::Message const &msg )
{
  SetPosition( boost::any_cast<sf::Vector2f>( msg.payload_ ) );
}
#include "PositionComponent.h"

FamilyId PositionComponentSystem::family_id_ = 0;
unsigned int PositionComponent::has_moved_id_ = 0;

////////////////////////////////////////////////////////////////
PositionComponentSystem::PositionComponentSystem( void )
{
  PositionComponent::has_moved_id_ = entity_system_->GetHandlerId( "HasMoved" );
}

////////////////////////////////////////////////////////////////
void PositionComponentSystem::Initialize( EntityId eid, GameState *state )
{
  /*noah::SafePtr<PositionComponent> c = GetComponent( eid );

  if ( c == 0 )
    return;

  state->world_->RegisterEntity( eid, c->position_ );*/
}

////////////////////////////////////////////////////////////////
void PositionComponentSystem::Update( GameState *state )
{
  rde::hash_map< EntityId, noah::SafePtr<PositionComponent> >::iterator i = components_.begin();
  for ( ; i != components_.end(); ++i )
  {
    if ( i->second->changed == false )
      continue;
    
   i->second->changed = false;

   state->world_->UpdateEntity( i->first, i->second->position_, i->second->old_position_ );
  }
}


////////////////////////////////////////////////////////////////
PositionComponent::PositionComponent( sf::Vector2f pos ) : Component( "Position" ), position_( pos ), changed( false ) { }
PositionComponent::PositionComponent( float x, float y ) : Component( "Position" ), position_( sf::Vector2f( x, y ) ), changed( false ) { }

////////////////////////////////////////////////////////////////
void PositionComponent::SetPosition( float x, float y )
{
  SetPosition( sf::Vector2f( x, y ) );
}

////////////////////////////////////////////////////////////////
void PositionComponent::SetPosition( sf::Vector2f p )
{
  old_position_ = position_;
  position_ = p;
  changed = true;

  BroadcastMessage( entity_, has_moved_id_, position_ );
}

////////////////////////////////////////////////////////////////
void PositionComponent::Registered( void )
{
  HandleMessage( "MoveBy", &PositionComponent::MoveBy );
}

////////////////////////////////////////////////////////////////
void PositionComponent::MoveBy( noah::Message const &msg )
{
  sf::Vector2f v = boost::any_cast<sf::Vector2f>( msg.payload_ );
  //std::cerr << "CALLBACK -- POSITION::MoveBy -- 1 -- x == " << v.x << " -- y == " << v.y << std::endl;
  SetPosition( position_ + v );
}

////////////////////////////////////////////////////////////////
void PositionComponent::MoveTo( noah::Message const &msg )
{
  //std::cerr << "CALLBACK -- POSITION::MoveTo -- 1" << std::endl;
  SetPosition( boost::any_cast<sf::Vector2f>( msg.payload_ ) );
}

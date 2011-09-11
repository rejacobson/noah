#include "PositionComponent.h"

/**
 * Position Component System
 */
FamilyId PositionComponentSystem::family_id_ = 0;

PositionComponentSystem::PositionComponentSystem( void )
{ }

/*void PositionComponentSystem::Initialize( noah::Entity *entity, GameState *state )
{
  noah::SafePtr<PositionComponent> c = GetComponent( entity->id_ );

  if ( c == 0 )
    return;

  entity->RegisterAttr( "position", new sf::Vector2f(0, 0) );

  //state->world_->RegisterEntity( entity->id_, c->position_ );
}*/

void PositionComponentSystem::Update( GameState *state )
{
  std::map<EntityId, noah::SafePtr<PositionComponent>>::iterator i = components_.begin();
  for ( ; i != components_.end(); ++i )
  {
    /*if ( i->second->changed == false )
      continue;
    
    i->second->changed = false;

    state->world_->UpdateEntity( i->first, i->second->position_, i->second->old_position_ );*/
  }
}

/**
 * Position Component
 */
PositionComponent::PositionComponent( sf::Vector2f pos )
  : position_( pos ), changed( false )
{ }

PositionComponent::PositionComponent( float x, float y )
  : position_( sf::Vector2f( x, y ) ), changed( false )
{ }

void PositionComponent::Registered( void )
{
  entity_->RegisterAttr( "position", new sf::Vector2f(position_), &PositionComponent::Moved );
  entity_->RegisterAttr( "old_position", new sf::Vector2f(position_) );
}

void PositionComponent::Moved( void *new_position )
{
  changed = true;

  //std::cerr << "Saw POSITION change" << std::endl;
}

/*void PositionComponent::UpdatePosition( float x, float y )
{
  UpdatePosition( sf::Vector2f( x, y) );
}

void PositionComponent::UpdatePosition( sf::Vector2f p )
{
  old_position_ = position_;
  position_ += p;
  changed = true;
}*/

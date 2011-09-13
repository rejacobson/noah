#include "PositionComponent.h"

/**
 * Position Component System
 */
FamilyId PositionComponentSystem::family_id_ = 0;

PositionComponentSystem::PositionComponentSystem( World *world )
  : world_( world )
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
  PositionComponent *c;
  noah::Entity *e;
  sf::Vector2f *p;

  std::map<EntityId, noah::SafePtr<PositionComponent>>::iterator i = components_.begin();
  for ( ; i != components_.end(); ++i )
  {
    c = i->second;

    if ( c->changed_ == false ) continue;

    e = c->entity_;
    p = e->GetAttr<sf::Vector2f>( c->position_id_ );

    c->changed_ = false;

    state->world_->UpdateEntity( i->first, *p, c->old_position_ );
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
  : position_( pos ), changed_( false )
{ }

PositionComponent::PositionComponent( float x, float y )
  : position_( sf::Vector2f( x, y ) ), changed_( false )
{ }

void PositionComponent::Registered( void )
{
  entity_->RegisterAttr( "position", new sf::Vector2f(position_), CreateCallback(&PositionComponent::Moved) );
  entity_->RegisterAttr( "old_position", new sf::Vector2f(position_) );

  position_id_ = entity_->FetchAttrId( "position" );

  component_system_->world_->RegisterEntity( entity_->id_, *entity_->GetAttr<sf::Vector2f>("position") );
}

void PositionComponent::Moved( void *new_position )
{
  changed_ = true;

  //std::cerr << "Moved Callback eid == " << entity_->id_ << std::endl;

  old_position_ = *entity_->GetAttr<sf::Vector2f>( "position" );

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

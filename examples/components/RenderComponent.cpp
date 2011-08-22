#include "RenderComponent.h"
#include "../GridSpace.h"

/**
 * Render Component System
 */
FamilyId RenderComponentSystem::family_id_ = 0;

RenderComponentSystem::RenderComponentSystem( void )
{
  text = new sf::Text();

  noah::SafePtr<sf::Font> font = noah::ResourceManager::Load<sf::Font>( "assets/Vera.ttf" );

  if ( font != 0 )
  {
    text->SetFont( *font );
    text->SetCharacterSize( 12 );
  }
}

void RenderComponentSystem::Update( GameState *state )
{
  stdext::hash_map<EntityId, noah::SafePtr<RenderComponent>>::iterator i = components_.begin();
  for ( ; i != components_.end(); ++i )
  {
    if ( i->second->physics_component_ == NULL ||
         i->second->physics_component_->position_component_ == NULL ||
         i->second->sprite_ == 0 )
      continue;

    sf::Vector2f pos = i->second->physics_component_->position_component_->position_;
    sf::Vector2f vel = i->second->physics_component_->velocity_;

    pos += ( vel * state->interpolation_ );

    i->second->sprite_->SetPosition( pos );

    state->window_->Draw( *(i->second->sprite_) );


    GridCell *cell = state->world_->world_space_->GetCell( i->first );

    text->SetString( cell->text_index_->GetString() );
    text->SetPosition(
      i->second->physics_component_->position_component_->position_.x,
      i->second->physics_component_->position_component_->position_.y );

    state->window_->Draw( *text );
  }
}

void RenderComponentSystem::Initialize( EntityId eid, GameState *state )
{
  noah::SafePtr<RenderComponent> c = GetComponent( eid );

  entity_system_->FulfillDependency<PhysicsComponent>( eid, &(c->physics_component_) );
}

/**
 * Render Component
 */
RenderComponent::RenderComponent( std::string image )
{
  if ( image.size() != 0 )
  {
    sprite_ = noah::ResourceManager::Load<sf::Sprite>( image );
    sf::Vector2f size = sprite_->GetSize();
    sprite_->SetOrigin( size.x / 2, size.y / 2 );
  }
}

#include "RenderComponent.h"

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

void RenderComponentSystem::Update( noah::GameState *state )
{
  EntityId eid;
  RenderComponent *c;
  PhysicsComponent *physics;
  PositionComponent *position;

  std::map<EntityId, noah::SafePtr<RenderComponent>>::iterator i = components_.begin();
  for ( ; i != components_.end(); ++i )
  {
    eid = i->first;
    c = i->second;
    physics = c->physics_component_;
    position = c->position_component_;

    if ( physics == 0 || position == 0 || c->sprite_ == 0 )
      continue;

    sf::Vector2f pos = position->position_;

    pos += ( physics->velocity_ * state->interpolation_ );

    c->sprite_->SetPosition( pos );

    state->window_->Draw( *c->sprite_ );
  }
}

/**
 * Render Component
 */
RenderComponent::RenderComponent( std::string image )
  : Component( "Render" )
{
  if ( image.size() != 0 )
  {
    sprite_ = noah::ResourceManager::Load<sf::Sprite>( image );
    sf::Vector2f size = sprite_->GetSize();
    sprite_->SetOrigin( size.x / 2, size.y / 2 );
  }
}

void RenderComponent::Registered( void )
{
  Requires( &position_component_ );
  Requires( &physics_component_ );
}

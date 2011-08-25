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
  sf::Vector2f pos;

  stdext::hash_map<EntityId, noah::SafePtr<RenderComponent>>::iterator i = components_.begin();
  for ( ; i != components_.end(); ++i )
  {
    pos = i->second->position_ + ( i->second->velocity_ * state->interpolation_ );

    i->second->sprite_->SetPosition( pos );

    state->window_->Draw( *(i->second->sprite_) );


    GridCell *cell = state->world_->world_space_->GetCell( i->first );

    text->SetString( cell->text_index_->GetString() );
    text->SetPosition( i->second->position_.x, i->second->position_.y );

    state->window_->Draw( *text );
  }
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

void RenderComponent::Registered( void )
{
  HandleMessage( "HasMoved", &RenderComponent::HasMoved );
}

void RenderComponent::HasMoved( noah::Message const &msg )
{
  position_ = boost::any_cast<sf::Vector2f>( msg.payload_ );
}

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

  rde::hash_map<EntityId, noah::SafePtr<RenderComponent> >::iterator i = components_.begin();
  for ( ; i != components_.end(); ++i )
  {
    //std::cerr << "RENDER::UPDATE -- 1 -- x == " << i->second->position_.x << " -- y == " << i->second->position_.y << std::endl;
    //pos = i->second->position_ + ( i->second->velocity_ * state->interpolation_ );

    i->second->sprite_->SetPosition( i->second->position_ + ( i->second->velocity_ * state->interpolation_ ) );

    state->window_->Draw( *(i->second->sprite_) );

    /*GridCell *cell = state->world_->world_space_->GetCell( i->first );

    text->SetString( cell->text_index_->GetString() );
    text->SetPosition( i->second->position_.x, i->second->position_.y );

    state->window_->Draw( *text );*/
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
//std::cerr << "RenderComponent::__constructor() -- 1" << std::endl;
    sprite_ = noah::ResourceManager::Load<sf::Sprite>( image );
//std::cerr << "RenderComponent::__constructor() -- 2" << std::endl;
    sf::Vector2f size = sprite_->GetSize();
//std::cerr << "RenderComponent::__constructor() -- 3" << std::endl;
    sprite_->SetOrigin( size.x / 2, size.y / 2 );
//std::cerr << "RenderComponent::__constructor() -- 4" << std::endl;
  }
}

void RenderComponent::Registered( void )
{
  WatchComponent( "Position", &RenderComponent::SyncPosition, entity_ );
  WatchComponent( "Physics", &RenderComponent::SyncVelocity, entity_ );
  HandleMessage( "HasMoved", &RenderComponent::HasMoved );
}

void RenderComponent::SyncPosition( noah::Message const &msg )
{
  //std::cerr << "CALLBACK -- RENDER::SyncPosition -- 1" << std::endl;
  if ( msg.type_ == noah::CREATE )
  {
    position_ = ((PositionComponent*)msg.sender_)->position_;
  }
}

void RenderComponent::SyncVelocity( noah::Message const &msg )
{
  //std::cerr << "CALLBACK -- RENDER::SyncVelocity -- 1" << std::endl;
  if ( msg.type_ == noah::CREATE )
  {
    velocity_ = ((PhysicsComponent*)msg.sender_)->velocity_;
  }
}

void RenderComponent::HasMoved( noah::Message const &msg )
{
  sf::Vector2f p = boost::any_cast<sf::Vector2f>( msg.payload_ );
  //std::cerr << "CALLBACK -- RENDER::HasMoved -- 1 -- x == " << p.x << " -- y == " << p.y << std::endl;
  position_ = p;
}

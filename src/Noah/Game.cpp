#include <Noah/Game.h>
//#include "Components/AllComponents.h"

namespace noah
{

Game::Game( sf::RenderWindow *display_window )
  : window_(display_window), game_is_running_(false)
{
  game_state_ = new noah::GameState;

  game_state_->window_ = window_;
  game_state_->game_ = this;
  game_state_->frame_count_ = 0;

  entity_system_ = EntitySystem( this );
}

/*void Game::RegisterEntity( EntityId eid, sf::Vector2f position )
{
  world_space_->RegisterEntity( eid, position );
}

void Game::UpdateEntity( EntityId eid, sf::Vector2f position, sf::Vector2f old_position )
{
  //world_space_->UpdateEntity( eid, position, old_position );
}

bool Game::CollidesWithTerrain( EntityId entity_id, CollisionState &collision_state )
{
  return false;
}*/

int Game::Run( void )
{
  const int TICKS_PER_SECOND = 25;
  const int SKIP_TICKS = 1000 / TICKS_PER_SECOND;
  const int MAX_FRAMESKIP = 5;

  DWORD next_game_tick = GetTickCount();
  //DWORD ticks = 0;
  int loops;

  while ( window_->IsOpened() )
  {
    // Handle events
    sf::Event event;
    while ( window_->PollEvent(event) )
    {
        // Window closed or escape key pressed : exit
        if ( ( event.Type == sf::Event::Closed ) || ( ( event.Type == sf::Event::KeyPressed ) && ( event.Key.Code == sf::Keyboard::Escape ) ) )
        {
            window_->Close();
            break;
        }
    }

    loops = 0;
    
    while ( GetTickCount() > next_game_tick && loops < MAX_FRAMESKIP )
    {
        game_state_->frame_count_++;

        //entity_system_.Update( "integration", game_state_ );

        //if ( ticks % 2 == 0 ) entity_system_.Update( "position", game_state_ );

        //if ( (ticks+1) % 2 == 0 ) entity_system_.Update( "physics", game_state_ );

        next_game_tick += SKIP_TICKS;

        Update();

        loops++;
        //++ticks;
    }

    game_state_->interpolation_ = float( GetTickCount() + SKIP_TICKS - next_game_tick )
                    / float( SKIP_TICKS );

    // Clear screen
    window_->Clear();

    //terrain_.Render( game_state_ );

    //world_space_->Render( game_state_ );

    //entity_system_.Update( "render", game_state_ );
    Render();

    window_->Display();
  }

  return 0;
}

}
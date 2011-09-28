#include <Noah/Game.h>

namespace noah
{

Game::Game( sf::RenderWindow *display_window )
  : window_(display_window), game_is_running_(false), current_scene_( 0 ), next_scene_( 0 )
{
  game_state_ = new noah::GameState;

  game_state_->window_ = window_;
  game_state_->game_ = this;
  game_state_->frame_count_ = 0;
}

void Game::SetScene( Scene *scene )
{
  scene->game = this;

  if ( 0 == current_scene_ )
  {
    current_scene_ = scene;
    return;
  }

  next_scene_ = scene;
}

int Game::Run( void )
{
  const int TICKS_PER_SECOND = 25;
  const int SKIP_TICKS = 1000 / TICKS_PER_SECOND;
  const int MAX_FRAMESKIP = 5;

  DWORD next_game_tick = GetTickCount();
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

        next_game_tick += SKIP_TICKS;

        if ( 0 != current_scene_ )
        {
          current_scene_->Update( game_state_ );
        }

        loops++;
    }

    game_state_->interpolation_ = float( GetTickCount() + SKIP_TICKS - next_game_tick )
                    / float( SKIP_TICKS );

    // Clear screen
    window_->Clear();

    //terrain_.Render( game_state_ );

    //world_space_->Render( game_state_ );

    //entity_system_.Update( "render", game_state_ );
    if ( 0 != current_scene_ )
    {
      current_scene_->Render( game_state_ );
    }

    window_->Display();

    if ( 0 != next_scene_ )
    {
      current_scene_ = next_scene_;
      next_scene_ = 0;
    }
  }

  return 0;
}

}
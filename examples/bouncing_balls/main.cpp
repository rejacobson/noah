#include <SFML/Graphics.hpp>
#include <Noah/EntitySystem.h>
#include "Components/AllComponents.h"

class BallScene : public noah::Scene
{
  public:
    BallScene( void ) : noah::Scene()
    {
      Initialize();
    }

    void Initialize( void )
    {
      
      /////////////////////////////////////////////////////////
    }

    void Update( noah::GameState *state )
    {
      entity_system.Update( "integration", state );
    }

    void Render( noah::GameState *state )
    {
      entity_system.Update( "render", state );
    }
};


int main()
{
  // Create the main window
  sf::RenderWindow window( sf::VideoMode(1000, 700), "SFML window" );

  noah::EntitySystem entity_system;

  std::cerr << "Setting up entity manager and component systems" << std::endl;

  entity_system.RegisterComponentSystem( new PositionComponentSystem(), "integration" );
  entity_system.RegisterComponentSystem( new PhysicsComponentSystem(), "integration" );
  entity_system.RegisterComponentSystem( new RenderComponentSystem(), "render" );

  std::cerr << "Setting up entity manager and component systems -- 3" << std::endl;
  /////////////////////////////////////////////////////////  
  
  noah::Entity *e;

  srand( (unsigned) time(0) );

  float x, y;

  float speed = 10.0;

  float center_x = 500.0; //(float)window_->GetWidth() / 2;
  float center_y = 350.0; //(float)window_->GetHeight() / 2;

  for ( int i = 0; i < 10; i++ )
  {
    e  = entity_system.NewEntity();

    x = ((float)rand())/RAND_MAX * (speed*2) - speed;
    y = ((float)rand())/RAND_MAX * (speed*2) - speed;

    entity_system.AddComponent( e, new PositionComponent(center_x, center_y) );
    entity_system.AddComponent( e, new PhysicsComponent(x, y) );
    //entity_system.AddComponent( e, new CollisionComponent() );
    entity_system.AddComponent( e, new RenderComponent("Assets/bluesphere.png") );
  } 



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

    //entity_system.Update( "render", game_state_ );
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


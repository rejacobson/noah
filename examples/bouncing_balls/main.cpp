#include <Windows.h>
#include <SFML/Graphics.hpp>
#include <Noah/EntitySystem.h>
#include "Components/AllComponents.h"


int main()
{
  // Create the main window
  sf::RenderWindow window( sf::VideoMode(1000, 700), "SFML window" );

  // Create an Entity System
  noah::EntitySystem entity_system;

  // The game state passes information about the game along to the components and component systems
  noah::GameState game_state;
  game_state.window_ = &window;

  entity_system.RegisterComponentSystem( new PositionComponentSystem(), "integration" );
  entity_system.RegisterComponentSystem( new PhysicsComponentSystem(), "integration" );
  entity_system.RegisterComponentSystem( new RenderComponentSystem(), "render" );

  // Set up a bunch of entities to throw around the screen
  noah::Entity *e;

  srand( (unsigned) time(0) );

  float x, y;

  float speed = 10.0;

  float center_x = (float)window.GetWidth() / 2;
  float center_y = (float)window.GetHeight() / 2;

  for ( int i = 0; i < 10; i++ )
  {
    e  = entity_system.NewEntity();

    x = ((float)rand())/RAND_MAX * (speed*2) - speed;
    y = ((float)rand())/RAND_MAX * (speed*2) - speed;

    entity_system.AddComponent( e, new PositionComponent(center_x, center_y) );
    entity_system.AddComponent( e, new PhysicsComponent(x, y) );
    entity_system.AddComponent( e, new RenderComponent("Assets/bluesphere.png") );
  } 


  // Main game loop
  const int TICKS_PER_SECOND = 25;
  const int SKIP_TICKS = 1000 / TICKS_PER_SECOND;
  const int MAX_FRAMESKIP = 5;

  DWORD next_game_tick = GetTickCount();
  int loops;

  while ( window.IsOpened() )
  {
    // Handle events
    sf::Event event;
    while ( window.PollEvent(event) )
    {
        // Window closed or escape key pressed : exit
        if ( ( event.Type == sf::Event::Closed ) || ( ( event.Type == sf::Event::KeyPressed ) && ( event.Key.Code == sf::Keyboard::Escape ) ) )
        {
            window.Close();
            break;
        }
    }

    loops = 0;
    
    while ( GetTickCount() > next_game_tick && loops < MAX_FRAMESKIP )
    {
        game_state.frame_count_++;

        next_game_tick += SKIP_TICKS;

        // Update the position and physics components
        entity_system.Update( "integration", &game_state );

        loops++;
    }

    game_state.interpolation_ = float( GetTickCount() + SKIP_TICKS - next_game_tick )
                    / float( SKIP_TICKS );

    window.Clear();

    // Render the entities
    entity_system.Update( "render", &game_state );

    window.Display();
  }

  return 0;
}


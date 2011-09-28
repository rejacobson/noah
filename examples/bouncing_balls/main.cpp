#include <SFML/Graphics.hpp>
#include <Noah/Game.h>
#include <Noah/Scene.h>
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
      std::cerr << "Setting up terrain and level data" << std::endl;

      //terrain_.LoadTerrainData( "assets/terrain/level1.json" );
      //world_space_ = terrain_.CreateGridSpace( 50, 50 );

      std::cerr << "Setting up entity manager and component systems" << std::endl;

      entity_system_->RegisterComponentSystem( new PositionComponentSystem(), "integration" );
      entity_system_->RegisterComponentSystem( new PhysicsComponentSystem(), "integration" );
      entity_system_->RegisterComponentSystem( new RenderComponentSystem(), "render" );

      std::cerr << "Setting up entity manager and component systems -- 3" << std::endl;

      /////////////////////////////////////////////////////////
      /*
      noah::Entity *e = entity_system_.NewEntity();

      std::cerr << "Setting up entity manager and component systems -- 4" << std::endl;

      std::cerr << "Setting up entity manager and component systems -- 5" << std::endl;

      entity_system_.AddComponent( e, new PositionComponent(200.0f, 150.0f) );
      entity_system_.AddComponent( e, new PhysicsComponent(1, 1) );
      entity_system_.AddComponent( e, new RenderComponent("assets/bluesphere.png") );

      std::cerr << "Setting up entity manager and component systems -- 6" << std::endl;
  

      //entity_system_.AddComponent( e, new PhysicsComponent(3, 3) );
      //entity_system_.AddComponent( e, new CollisionComponent() );
      //entity_system_.AddComponent( e, new RenderComponent("assets/bluesphere.png") );

      //entity_system_.InitializeEntity( e, game_state_ );
      */

      /////////////////////////////////////////////////////////  
  
      noah::Entity *e;

      srand( (unsigned) time(0) );

      float x, y;

      float speed = 10.0;

      float center_x = 500.0; //(float)window_->GetWidth() / 2;
      float center_y = 350.0; //(float)window_->GetHeight() / 2;

      for ( int i = 0; i < 10; i++ )
      {
        e  = entity_system_->NewEntity();

        x = ((float)rand())/RAND_MAX * (speed*2) - speed;
        y = ((float)rand())/RAND_MAX * (speed*2) - speed;

        entity_system_->AddComponent( e, new PositionComponent(center_x, center_y) );
        entity_system_->AddComponent( e, new PhysicsComponent(x, y) );
        //entity_system_.AddComponent( e, new CollisionComponent() );
        entity_system_->AddComponent( e, new RenderComponent("Assets/bluesphere.png") );
      }  
      /////////////////////////////////////////////////////////
    }

    void Update( noah::GameState *state )
    {
      entity_system_->Update( "integration", state );
    }

    void Render( noah::GameState *state )
    {
      entity_system_->Update( "render", state );
    }
};


int main()
{
  // Create the main window
  sf::RenderWindow window( sf::VideoMode(1000, 700), "SFML window" );

  //Balls game( &window );
  noah::Game *game = new noah::Game( &window );
  game->SetScene( new BallScene() );

  return game->Run();
}


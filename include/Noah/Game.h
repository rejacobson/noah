#ifndef _GAME_H
#define _GAME_H

#include <SFML/System/Err.hpp>
#include <SFML/Graphics.hpp>
#include <time.h>
#include <Windows.h>

#include <Noah/GameState.h>
#include <Noah/EntitySystem.h>

//#include "GridSpace.h"
//#include "Terrain.h"
//#include "CollisionState.h"

namespace noah
{

class Game
{
  public:
    Game( sf::RenderWindow *display_window );

    void RegisterEntity( EntityId, sf::Vector2f position );
    void UpdateEntity( EntityId, sf::Vector2f position, sf::Vector2f old_position );
    int Run( void );

    virtual void Update() { std::cerr << "Updating base Game class. Inherit your own main game class from this one" << std::endl; }
    virtual void Render() { std::cerr << "Rendering base Game class. Inherit your own main game class from this one" << std::endl; }

    //bool CollidesWithTerrain( EntityId entity_id, CollisionState &collision_state );
    //bool CollidesWithEntity( EntityId entity_id, CollisionState &collision_state );

    GameState *game_state_;
    //GridSpace *world_space_;

  public:
    sf::RenderWindow *window_;
    EntitySystem entity_system_;

  private:
    void Initialize( void );
  
    bool game_is_running_;
    //Terrain terrain_;
};

}


#endif


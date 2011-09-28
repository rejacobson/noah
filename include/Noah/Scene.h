#ifndef _SCENE_H
#define _SCENE_H

#include <iostream>
#include <Noah/EntitySystem.h>

namespace noah
{

class Game;

class Scene
{
  public:
    Scene( void );

    virtual void Update( GameState *state ) { std::cerr << "Updating base Scene class. Inherit your own scene class from this one" << std::endl; }
    virtual void Render( GameState *state ) { std::cerr << "Rendering base Scene class. Inherit your own scene class from this one" << std::endl; }

    EntitySystem *entity_system_;

  protected:
    friend class Game;
    Game *game;

};

}

#endif
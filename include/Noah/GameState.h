#ifndef _GAME_STATE_H
#define _GAME_STATE_H

#include <SFML/Graphics.hpp>

namespace noah
{

struct GameState
{
  float interpolation_;
  sf::RenderWindow *window_;
  unsigned long frame_count_;
};

}

#endif
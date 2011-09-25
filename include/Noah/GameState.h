#ifndef _GAME_STATE_H
#define _GAME_STATE_H

#include <SFML/Graphics.hpp>

namespace noah
{

class Game;

struct GameState
{
  float interpolation_;
  sf::RenderWindow *window_;
  sf::Sprite *sprite_;
  Game *game_;
  unsigned long frame_count_;
};

}

#endif
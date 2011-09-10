#ifndef _POSITION_COMPONENT_H
#define _POSITION_COMPONENT_H

#include <iostream>
#include <string>

#include <SFML/System.hpp>

#include <Noah/EntitySystem.h>
#include "../World.h"

class PositionComponent;

/**
 * Position Component System
 */
class PositionComponentSystem : public noah::ComponentSystem <PositionComponent>
{
  public:
    static FamilyId family_id_;

    PositionComponentSystem( void );
    ~PositionComponentSystem( void );

    void Initialize( EntityId, GameState* );
    void Update( GameState* );
};


/**
 * Position Component
 */
class PositionComponent : public noah::Component <PositionComponentSystem>
{
  public:
    PositionComponent( void );
    PositionComponent( sf::Vector2f );
    PositionComponent( float, float );

    void SetPosition( float, float );
    void SetPosition( sf::Vector2f );

    void Registered( void );
    void MoveBy( noah::Message const &msg );
    void MoveTo( noah::Message const &msg );

    sf::Vector2f position_;
    sf::Vector2f old_position_;
    bool changed;

    static unsigned int has_moved_id_;
};


#endif
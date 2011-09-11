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

    //void Initialize( noah::Entity *entity, GameState *state );
    void Update( GameState* );
};


/**
 * Position Component
 */
class PositionComponent : public noah::Component <PositionComponentSystem>
{
  public:
    PositionComponent( sf::Vector2f );
    PositionComponent( float, float );
    void Registered( void );

    void Moved( void *new_position );

    //void UpdatePosition( float, float );
    //void UpdatePosition( sf::Vector2f );

    sf::Vector2f position_;
    sf::Vector2f old_position_;

    bool changed;
};


#endif
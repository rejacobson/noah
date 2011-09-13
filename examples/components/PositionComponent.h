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

    PositionComponentSystem( World *world );
    ~PositionComponentSystem( void );

    void Update( GameState *state );

    World *world_;
};


/**
 * Position Component
 */
class PositionComponent : public noah::Component <PositionComponentSystem>
{
  public:
    PositionComponent( sf::Vector2f pos );
    PositionComponent( float x, float y );
    void Registered( void );

    void Moved( void *new_position );

    sf::Vector2f position_;
    sf::Vector2f old_position_;

    bool changed_;

    noah::AttrId position_id_;
};


#endif
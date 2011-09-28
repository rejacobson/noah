#ifndef _PHYSICS_COMPONENT_H
#define _PHYSICS_COMPONENT_H

#include <Noah/EntitySystem.h>
#include "PositionComponent.h"
#include <string>
#include <vector>

class PhysicsComponent;
class PositionComponent;

/**
 * Physics Component System
 */
class PhysicsComponentSystem : public noah::ComponentSystem <PhysicsComponent>
{
  public:
    static FamilyId family_id_;

    PhysicsComponentSystem( void );
    ~PhysicsComponentSystem( void );
    
    void Update( noah::GameState *state = 0 );
};


/** 
 * Physics Component
 */
class PhysicsComponent : public noah::Component <PhysicsComponentSystem>
{
  public:
    PhysicsComponent( void );
    PhysicsComponent( float, float );
    PhysicsComponent( sf::Vector2f );

    void Registered( void );

    PositionComponent *position_component_;
    sf::Vector2f velocity_;
    float width_, height_, mass_;
};


#endif
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
    
    void Update( GameState *state = 0 );
    //void Initialize( noah::Entity *entity, GameState *state );
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

    sf::Vector2f velocity_;
    float width_, height_, mass_;

    noah::AttrId position_id_, velocity_id_;
};


#endif
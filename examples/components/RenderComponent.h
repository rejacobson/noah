#ifndef _RENDER_COMPONENT_H
#define _RENDER_COMPONENT_H

#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>

#include <Noah/EntitySystem.h>
#include "../Resources.h"
#include "../World.h"
#include "PositionComponent.h"
#include "PhysicsComponent.h"

class RenderComponent;
class PhysicsComponent;

/**
 * Render Component System
 */
class RenderComponentSystem : public noah::ComponentSystem <RenderComponent>
{
  public:
    static FamilyId family_id_;
    sf::Text *text;

    RenderComponentSystem( void );
    ~RenderComponentSystem( void );
    
    void Update( GameState *state = 0 );
    //void Initialize( noah::Entity *entity, GameState *state );
};


/** 
 * Render Component
 */
class RenderComponent : public noah::Component <RenderComponentSystem>
{
  public:
    RenderComponent( std::string image = "" );

   void Registered( void );

    noah::SafePtr<sf::Sprite> sprite_;
    noah::AttrId position_id_, velocity_id_;
};


#endif
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
    void Initialize( EntityId, GameState* );
};


/** 
 * Render Component
 */
class RenderComponent : public noah::Component <RenderComponentSystem>
{
  public:
    RenderComponent( std::string image = "" );

    noah::SafePtr<sf::Sprite> sprite_;

    noah::SafePtr<PhysicsComponent> physics_component_;
};


#endif
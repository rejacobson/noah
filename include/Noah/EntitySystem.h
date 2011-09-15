////////////////////////////////////////////////////////////
//
// Noah - A Component Based Entity System and Game Engine
// Ryan Jacobson (rejacobson@gmail.com)
//
// Include this file to get both the entities and component systems
//
////////////////////////////////////////////////////////////

#ifndef _ENTITY_SYSTEM_H
#define _ENTITY_SYSTEM_H

////////////////////////////////////////////////////////////
/// \brief GameState forward declaration
///
/// The GameState object is used to carry information about the world and rest of the game from
/// the game loop to the component systems.
///
////////////////////////////////////////////////////////////
struct GameState;

////////////////////////////////////////////////////////////
/// \brief Entity unique id
///
////////////////////////////////////////////////////////////
typedef unsigned int EntityId;

////////////////////////////////////////////////////////////
/// \brief Component system unique id
///
////////////////////////////////////////////////////////////
typedef unsigned int FamilyId;

namespace noah
{
  class Entity;
  class EntitySystem;

  class ComponentSystemBase;
  class ComponentBase;

  struct Dependency
  {
    ComponentBase *requester_;
    ComponentBase *target_;
    FamilyId requester_family_id_;
    FamilyId target_family_id_;
  };
}

#include <Noah/SafePtr.h>
#include <Noah/Entity.h>
#include <Noah/Component.h>

#endif

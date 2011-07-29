////////////////////////////////////////////////////////////
//
// Noah - A Component Based Entity System and Game Engine
// Ryan Jacobson (rejacobson@gmail.com)
//
////////////////////////////////////////////////////////////

#ifndef _ENTITY_SYSTEM_H
#define _ENTITY_SYSTEM_H

struct GameState;

typedef unsigned int EntityId;
typedef unsigned int FamilyId;

namespace noah
{
  class Entity;
  class EntitySystem;

  class ComponentSystemBase;
  class ComponentBase;
}

#include <Noah/SafePtr.h>
#include <Noah/Entity.h>
#include <Noah/Component.h>

#endif
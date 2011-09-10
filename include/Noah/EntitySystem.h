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

#include <rdestl/hash_map.h>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>

#include <boost/function.hpp>
#include <boost/any.hpp>
#include <boost/bind.hpp>

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

  struct Message;
  struct Handler;
}

//#include <Noah/SafePtr.h>
//#include <Noah/Message.h>
//#include <Noah/Notifier.h>
#include <Noah/Entity.h>
#include <Noah/Component.h>

#endif
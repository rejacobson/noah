#ifndef _COMPONENT_H
#define _COMPONENT_H

#include <Noah/EntitySystem.h>
#include <iostream>
#include <string>

namespace noah
{

// Component System Base
//   This is used to keep heterogeneous lists of Component Systems
class ComponentSystemBase
{
  public:
    virtual void RegisterComponent( Entity*, ComponentBase* ) { }
    virtual void KillComponent( EntityId ) { }
    virtual void Update( GameState *state = 0 ) { }
    virtual void Initialize( EntityId, GameState* ) { }
};



// Component System
//   This is the class that custom Component Systems should derive from.
template <class TComponent>
class ComponentSystem : public ComponentSystemBase
{
  public:
    // The list of components in this system.
    std::map<EntityId, noah::safe_ptr<TComponent>> components_;

    EntitySystem *entity_system_;

    // Add a new component for an entity in the system.
    void RegisterComponent( Entity *e, ComponentBase *c )
    {
      components_.insert( std::pair<EntityId, safe_ptr<TComponent>>( e->id_, safe_ptr<TComponent>((TComponent*)c) ) );
    }

    // Loop through each component and run a functor for it.
    template <typename TFunctor>
    void EachComponent( TFunctor *functor )
    {
      std::map<EntityId, safe_ptr<TComponent>>::iterator i = components_.begin();
      for( ; i != components_.end(); ++i )
      {
        (*functor)( i->first, i->second );
      }
    }

    void KillComponent( EntityId eid )
    {
      std::map<EntityId, safe_ptr<TComponent>>::iterator toKill = components_.find(eid);
      toKill->second.clear();
      components_.erase(toKill);
    }

    safe_ptr<TComponent> GetComponent( EntityId eid )
    { 
      return components_[eid];
    }

    int Size( void )
    {
      return components_.size();
    }
};



//
// Component System Base
//
class ComponentBase { };

//
// Component
//
template <typename TSystem>
class Component : public ComponentBase
{
  public:

    static FamilyId GetFamilyId( void )
    {
      return TSystem::family_id_;
    }

    static TSystem *Cast( ComponentSystemBase *s )
    {
      return (TSystem*)s;
    }

    bool operator<(Component rhs) { return GetFamilyId() < rhs.GetFamilyId(); }
};

} // namespace noah

#endif
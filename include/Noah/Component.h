////////////////////////////////////////////////////////////
//
// Noah - A Component Based Entity System and Game Engine
// Ryan Jacobson (rejacobson@gmail.com)
//
////////////////////////////////////////////////////////////

#ifndef _COMPONENT_H
#define _COMPONENT_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////

#include <Noah/EntitySystem.h>
#include <hash_map>
#include <iostream>
#include <string>

namespace noah
{

////////////////////////////////////////////////////////////
/// \brief Base class allows for heterogeneous lists of component systems
///
////////////////////////////////////////////////////////////
class ComponentSystemBase
{
  public:    
    ////////////////////////////////////////////////////////////
    /// \brief Add a component to this component system
    ///
    /// \param entity A pointer to the entity to add a component for
    /// \param component A pointer to the component being added
    ///
    ////////////////////////////////////////////////////////////
    virtual void RegisterComponent( Entity *entity, ComponentBase *component ) { }
    
    ////////////////////////////////////////////////////////////
    /// \brief Remove a component from the system
    ///
    /// \param entity_id The id of the entity to remove the component from
    ///
    ////////////////////////////////////////////////////////////
    virtual void KillComponent( EntityId entity_id ) { }

    ////////////////////////////////////////////////////////////
    /// \brief Update the system
    ///
    /// Typically this is used to update the system once per frame
    ///
    /// \param state A pointer to the current GameState
    ///
    ////////////////////////////////////////////////////////////
    virtual void Update( GameState *state = 0 ) { }

    ////////////////////////////////////////////////////////////
    /// \brief Run initialization code for an entity.
    ///
    /// This is intended to be run once for an entity after it's
    /// components have been set.
    ///
    /// \param entity_id The id of the entity to initialize
    /// \param state A pointer to the current GameState
    ///
    ////////////////////////////////////////////////////////////
    virtual void Initialize( EntityId entity_id, GameState *state ) { }
};

////////////////////////////////////////////////////////////
/// \brief Component system manages a list of components of one type
///
/// This is the class that custom Component Systems should derive from.
///
////////////////////////////////////////////////////////////
template <class TComponent>
class ComponentSystem : public ComponentSystemBase
{
  public:
    ////////////////////////////////////////////////////////////
    /// \brief Add a new component to an entity
    ///
    /// \param entity A pointer to the entity
    /// \param component A pointer to the component to add
    ///
    ////////////////////////////////////////////////////////////
    void RegisterComponent( Entity *entity, ComponentBase *component )
    {
      components_.insert( std::pair<EntityId, SafePtr<TComponent>>( entity->id_, SafePtr<TComponent>((TComponent*)component) ) );

      component->Registered( entity );
    }

    ////////////////////////////////////////////////////////////
    /// \brief Run a functor on each component
    ///
    /// \param function Pointer to a functor that processes the component
    ///
    ////////////////////////////////////////////////////////////
    template <typename TFunctor>
    void EachComponent( TFunctor *functor )
    {
      std::map<EntityId, SafePtr<TComponent>>::iterator i = components_.begin();
      for( ; i != components_.end(); ++i )
      {
        (*functor)( i->first, i->second );
      }
    }

    ////////////////////////////////////////////////////////////
    /// \brief Remove a component from an entity
    ///
    /// \param entity_id The id of the entity to remove the component for
    ///
    ////////////////////////////////////////////////////////////
    void KillComponent( EntityId entity_id )
    {
      std::map<EntityId, SafePtr<TComponent>>::iterator toKill = components_.find( entity_id );

      // Delete the component held by the SafePtr
      toKill->second.clear();

      // Remove the component from the master list
      components_.erase( toKill );
    }
    
    ////////////////////////////////////////////////////////////
    /// \brief Get a component by entity_id
    ///
    /// \param entity_id The id of the entity to get a component from
    ///
    /// \return Returns a SafePtr to the component
    ///
    ////////////////////////////////////////////////////////////
    SafePtr<TComponent> GetComponent( EntityId entity_id )
    { 
      return components_[ entity_id ];
    }
    
    ////////////////////////////////////////////////////////////
    /// \brief Get the number of components in the system
    ///
    /// \return Returns the number of components in the system
    ///
    ////////////////////////////////////////////////////////////
    int Size( void )
    {
      return components_.size();
    }
    
    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    stdext::hash_map<EntityId, noah::SafePtr<TComponent>> components_; //< The list of components in this system
    EntitySystem *entity_system_;                               //< The entity system managing this component systme
};



////////////////////////////////////////////////////////////
/// \brief Base component class
///
////////////////////////////////////////////////////////////
class ComponentBase
{
  public:
    virtual void Registered( Entity *entity ) { }
};

////////////////////////////////////////////////////////////
/// \brief The basic component object, managed by a component system
///
////////////////////////////////////////////////////////////
template <typename TSystem>
class Component : public ComponentBase
{
  public:
    ////////////////////////////////////////////////////////////
    /// \brief Get the family id (component system id) that this component belongs to
    ///
    /// \return Returns the FamilyId of the component system
    ///
    ////////////////////////////////////////////////////////////
    static FamilyId GetFamilyId( void )
    {
      return TSystem::family_id_;
    }

    ////////////////////////////////////////////////////////////
    /// \brief Cast a ComponentSystemBase into this component's component system 
    ///
    /// \param base_system A pointer to a ComponentSystemBase object
    ///
    /// \return Returns the base object cast into the derived object
    ///
    ////////////////////////////////////////////////////////////
    static TSystem *Cast( ComponentSystemBase *base_system )
    {
      return (TSystem*)base_system;
    }

    ////////////////////////////////////////////////////////////
    /// \brief Less than operator 
    ///
    /// Used for sorting algorithms
    ///
    /// \param Right hand side component
    ///
    /// \return Returns true if the lhs family_id is less than the rhs family_id
    ///
    ////////////////////////////////////////////////////////////
    bool operator<(Component rhs) { return GetFamilyId() < rhs.GetFamilyId(); }

    void WatchMessage( std::string message_name, Callback callback )
    {
      Handler handler;
      handler.component = this;
      handler.callback_ = callback;

      component_system_->entity_system->RegisterMessageHandler( message_name, handler );
    }

    void WatchMessage( Entity *entity, std::string message_name, Callback callback )
    {
      Handler handler;
      handler.component = this;
      handler.callback_ = callback;

      component_system_->entity_system->RegisterMessageHandler( entity, message_name, handler );
    }

    void WatchComponent( std::string component_name, Callback callback )
    {

    }

    void WatchComponent( Entity *entity, std::string component_name, Callback callback )
    {

    }

  private:
    TSystem *component_system_;
};

} // namespace noah

#endif

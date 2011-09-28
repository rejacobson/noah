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
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    ComponentSystem( void ) : entity_system_( 0 ) { }

    ////////////////////////////////////////////////////////////
    /// \brief Add a new component to an entity
    ///
    /// \param entity A pointer to the entity
    /// \param component A pointer to the component to add
    ///
    ////////////////////////////////////////////////////////////
    void RegisterComponent( Entity *entity, ComponentBase *component )
    {
      TComponent *c = (TComponent*)component;

      c->entity_ = entity;
      c->component_system_ = TComponent::Cast( this );

      components_.insert( std::pair<EntityId, SafePtr<TComponent>>( entity->id_, SafePtr<TComponent>((TComponent*)component) ) );

      c->Registered();
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
    std::map<EntityId, noah::SafePtr<TComponent>> components_; //< The list of components in this system
    EntitySystem *entity_system_;                               //< The entity system managing this component system
};



////////////////////////////////////////////////////////////
/// \brief Base component class
///
////////////////////////////////////////////////////////////
class ComponentBase
{
  public:
    ComponentBase( std::string name ) : name_( name ), entity_( 0 ), missing_dependencies_( 0 ) { }
    virtual void Registered( void ) { }

    std::string name_;
    Entity *entity_;
    int missing_dependencies_;
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
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    Component( std::string name ) : ComponentBase( name ) { }

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

    ////////////////////////////////////////////////////////////
    /// \brief Set up a component dependency
    ///
    /// 
    ///
    ////////////////////////////////////////////////////////////
    template <typename TComponent>
    void Requires( TComponent **target )
    {
      missing_dependencies_++;
      *target = 0;
      entity_->RequireComponent( this, target );
    }

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    TSystem *component_system_;  //< The component system this component belongs to
};

} // namespace noah

#endif

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
      TComponent *c = (TComponent*)component;

      c->entity_ = entity;
      c->component_system_ = TComponent::Cast( this );

      components_.insert( std::pair<EntityId, SafePtr<TComponent>>( entity->id_, SafePtr<TComponent>(c) ) );

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
      stdext::hash_map<EntityId, SafePtr<TComponent>>::iterator i = components_.begin();
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
      stdext::hash_map<EntityId, SafePtr<TComponent>>::iterator toKill = components_.find( entity_id );

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
    
    

    /*void WatchComponent( std::string component_name, Callback callback )
    {

    }

    void WatchComponent( Entity *entity, std::string component_name, Callback callback )
    {

    }*/

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    stdext::hash_map< EntityId, noah::SafePtr<TComponent> > components_; //< The list of components in this system
    EntitySystem *entity_system_;                                        //< The entity system managing this component systme
};



////////////////////////////////////////////////////////////
/// \brief Base component class
///
////////////////////////////////////////////////////////////
class ComponentBase
{
  public:
    virtual void Registered( void ) { }
};

////////////////////////////////////////////////////////////
/// \brief The basic component object, managed by a component system
///
////////////////////////////////////////////////////////////
template <typename TSystem>
class Component : public ComponentBase
{
  public:
    ComponentSystem( std::string name ) : name_( name ) {}

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

    void HandleMessage( std::string message, Callback callback )
    {
      Handler handler;
      handler.component_ = this;
      handler.callback_ = callback;

      component_system_->entity_system_->RegisterMessageHandler( message, handler );
      entity_->RegisterMessageHandler( message, handler );
    }

    template<class TSystem>
    void HandleMessage( std::string message, void (TSystem::*f)(Message const &))
    {
      HandleMessage(message, boost::bind(f, (TSystem*)(this), _1));
    }

    void BroadcastMessage( std::string message, boost::any payload )
    {
      component_system_->entity_system_->BroadcastMessage( message, this, payload );
    }

    void BroadcastMessage( Entity *entity, std::string message, boost::any payload )
    {
      component_system_->entity_system_->BroadcastMessage( entity, message, this, payload );
    }

    Entity *entity_;
    TSystem *component_system_;
    std::string name_;
};

} // namespace noah

#endif

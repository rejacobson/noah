////////////////////////////////////////////////////////////
//
// Noah - A Component Based Entity System and Game Engine
// Ryan Jacobson (rejacobson@gmail.com)
//
////////////////////////////////////////////////////////////

#ifndef _ENTITY_H
#define _ENTITY_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////

#include <Noah/EntitySystem.h>
#include <map>
#include <vector>
#include <algorithm>

namespace noah
{

////////////////////////////////////////////////////////////
/// \brief Manages entities, components and component systems
///
////////////////////////////////////////////////////////////
class EntitySystem
{
  public:
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// This constructor creates an Entity System.
    ///
    ////////////////////////////////////////////////////////////
    EntitySystem( void );
        
    ////////////////////////////////////////////////////////////
    /// \brief Update all component systems
    ///
    /// \param state The current GameState.
    ///
    ////////////////////////////////////////////////////////////
    void Update( GameState *state = 0 );

    ////////////////////////////////////////////////////////////
    /// \brief Update component systems with a particular label
    ///
    /// \param label Update component systems with this label.
    /// \param state The current GameState.
    ///
    ////////////////////////////////////////////////////////////
    void Update( std::string label, GameState *state = 0 );

    ////////////////////////////////////////////////////////////
    /// \brief Update a list of component systems
    ///
    /// \param systems Update component systems with this label.
    /// \param state The current GameState.
    ///
    ////////////////////////////////////////////////////////////
    void Update( std::vector<safe_ptr<ComponentSystemBase>>* systems, GameState *state = 0 );

    ////////////////////////////////////////////////////////////
    /// \brief Register a component system
    ///
    /// Component systems are indexed by the order they're added to the entity manager.
    ///
    /// \param system A pointer to the component system.
    /// \param label Assign a label to this component system.
    ///
    ////////////////////////////////////////////////////////////
    template <typename TSystem>
    void RegisterComponentSystem( TSystem *system, std::string label = "" )
    {
      // Return if a Component System of the same type has already been added.
      if ( TSystem::family_id_ != 0 ) return;

      // Increment the Component System family id
      // Start at 0 so the Component Systems vector can be used like an array
      TSystem::family_id_ = component_system_count_++;

      // Connect the component system to this entity system.
      system->entity_system_ = this;

      // Create a safe_ptr to push onto the component systems map, and the labeled component systems map.
      safe_ptr<ComponentSystemBase> s( system );

      // Save the Component System into the map
      component_systems_.push_back( s );

      // If a label was also passed, create an entry in the labeled component systems map.
      if ( label.size() != 0 )
      {
        labeled_component_systems_[ label ].push_back( s );
      }
    }
   
    ////////////////////////////////////////////////////////////
    /// \brief Get a component system of a particular type
    ///
    /// Use the template parameter to select which component system to return.
    /// eg. GetSystem<PositionComponentSystem>();
    ///
    /// \return A component system
    ///
    ////////////////////////////////////////////////////////////
    template <typename TSystem>
    TSystem *GetSystem( void )
    {
      return (TSystem*)( (ComponentSystemBase*)component_systems_[ TSystem::family_id_ ] );
    }

    ////////////////////////////////////////////////////////////
    /// \brief Get a Component System by it's family id
    ///
    /// \param family_id The unique id of the component system
    ///
    /// \return A safe_ptr to a ComponentSystemBase
    ///
    ////////////////////////////////////////////////////////////    
    safe_ptr<ComponentSystemBase> GetSystem( FamilyId family_id );

    ////////////////////////////////////////////////////////////
    /// \brief Get a component by entity id
    ///
    /// Use the template parameter to specify the type of component
    /// eg. GetComponent<PositionComponent>( entity_id );
    ///
    /// \param entity_id The entity id
    ///
    /// \return A safe_ptr to a component
    ///
    ////////////////////////////////////////////////////////////    
    template <typename TComponent>
    safe_ptr<TComponent> GetComponent( EntityId entity_id )
    {
      ComponentSystemBase *s = component_systems_[ TComponent::GetFamilyId() ];

      // Hackish method to cast a component into it's derived object
      return TComponent::Cast(s)->GetComponent( entity_id );
    }
    
    ////////////////////////////////////////////////////////////
    /// \brief Add a component to an entity
    ///
    /// Use the template parameter to specify the type of component
    /// eg. GetComponent<PositionComponent>( entity_id );
    ///
    /// \param entity_id The entity id
    ///
    /// \return A safe_ptr to a component
    ///
    ////////////////////////////////////////////////////////////    
    template <typename TComponent>
    void AddComponent( Entity *e, TComponent *c )
    {
      component_systems_[ TComponent::GetFamilyId() ]->RegisterComponent( e, c );

      // Register the component system's family id with the Entity
      // so it's easier to find this Entity's components later on.
      e->family_ids_.push_back( TComponent::GetFamilyId() );
    }

    ////////////////////////////////////////////////////////////
    /// \brief Find a component on which another component depends on
    ///
    /// Some components require other components to exist in order
    /// to function.  For example, the Physics component requires
    /// the Position component in order to have a position in the
    /// world to update.
    ///
    /// \param entity_id The entity id
    /// \param safe_ptr<DependantComponent>* A pointer which gets assigned the dependant component.
    ///
    /// \return Returns true if the dependant was found, false otherwise.
    ///
    ////////////////////////////////////////////////////////////    
    template <typename TComponent>
    bool FulfillDependency( EntityId entity_id, safe_ptr<TComponent> *component )
    {
      if ( (*component) != NULL )
        return true;

      safe_ptr<TComponent> c = GetComponent<TComponent>( entity_id );

      if ( c == NULL )
        return false;

      (*component) = c;

      return true;
    }

    ////////////////////////////////////////////////////////////
    /// \brief Register an entity
    ///
    /// \param entity A pointer to the entity
    ///
    /// \return A safe_ptr to the entity
    ///
    ////////////////////////////////////////////////////////////    
    safe_ptr<Entity> RegisterEntity( Entity* entity );

    ////////////////////////////////////////////////////////////
    /// \brief Create and register a new Entity
    ///
    /// Don't call RegisterEntity if you use this
    ///
    /// \return A safe_ptr to the new entity
    ///
    ////////////////////////////////////////////////////////////
    safe_ptr<Entity> NewEntity( void );

    ////////////////////////////////////////////////////////////
    /// \brief Run the initialize method on each entity's components
    ///
    /// \param entity The entity to initialize
    /// \param state The current state of the game
    ///
    ////////////////////////////////////////////////////////////
    void InitializeEntity( Entity* entity, GameState* state );

    ////////////////////////////////////////////////////////////
    /// \brief Get an entity by entity_id
    ///
    /// \param entity_id The id of the entity to retrieve
    /// 
    /// \return A safe_ptr to the entity
    ///
    ////////////////////////////////////////////////////////////
    safe_ptr<Entity> GetEntity( EntityId );

    ////////////////////////////////////////////////////////////
    /// \brief Get the next entity_id
    ///
    /// \return An EntityId
    ///
    ////////////////////////////////////////////////////////////
    EntityId GetNextAvailablentity_id( void );

    ////////////////////////////////////////////////////////////
    /// \brief Remove an entity and it's components
    ///
    ////////////////////////////////////////////////////////////
    void KillEntity( EntityId );
    
  private:
    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////

    static int component_system_count_;                                                           ///< The number of component systems already registered
    std::map<EntityId, safe_ptr<Entity>> entities_;                                               ///< Master list of Entities
    std::vector<safe_ptr<ComponentSystemBase>> component_systems_;                                ///< Master list of Component Systems
    std::map<std::string, std::vector<safe_ptr<ComponentSystemBase>>> labeled_component_systems_; ///< List of labeled Component Systems
};


////////////////////////////////////////////////////////////
/// \brief Entity - The basic game object
///
////////////////////////////////////////////////////////////
class Entity
{
  public:
    ////////////////////////////////////////////////////////////
    /// \brief Construct a new entity with an id
    ///
    /// \param entity_id The id to create the entity with
    ///
    ////////////////////////////////////////////////////////////
    Entity( EntityId entity_id );
    
    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    EntityId id_;                         ///< The unique id of the entity
    static EntitySystem *entity_system_; ///< Pointer to the entity system managing this entity
    std::vector<FamilyId> family_ids_;    ///< List of component system ids from which this entity has components from
};

} // namespace noah

#endif

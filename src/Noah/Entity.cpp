#include <Noah/EntitySystem.h>

namespace noah
{

EntityId EntitySystem::entity_counter_ = 0;

////////////////////////////////////////////////////////////////
EntitySystem::EntitySystem( void )
  : scene_( 0 ), component_system_count_( 0 )
{ }

////////////////////////////////////////////////////////////////
EntitySystem::EntitySystem( Scene *scene )
  : scene_( scene ), component_system_count_( 0 )
{ }

////////////////////////////////////////////////////////////////
void EntitySystem::Update( GameState *state )
{
  Update( &component_systems_, state );
}

////////////////////////////////////////////////////////////////
void EntitySystem::Update( std::string label, GameState *state )
{
  Update( &(labeled_component_systems_[ label ]), state );
}

////////////////////////////////////////////////////////////////
void EntitySystem::Update( std::vector<SafePtr<ComponentSystemBase>> *systems, GameState *state )
{
  std::vector<SafePtr<ComponentSystemBase>>::const_iterator it = systems->begin();
  for ( ; it != systems->end(); ++it )
  {
    (*it)->Update( state );
  }
}

////////////////////////////////////////////////////////////////
SafePtr<ComponentSystemBase> EntitySystem::GetSystem( FamilyId family_id )
{
  return component_systems_[ family_id ];
}

////////////////////////////////////////////////////////////////
SafePtr<Entity> EntitySystem::RegisterEntity( Entity *entity )
{
  SafePtr<Entity> e(entity);

  entities_.insert( std::pair<EntityId, SafePtr<Entity>>( entity->id_, e ) );

  return e;
}

////////////////////////////////////////////////////////////////
SafePtr<Entity> EntitySystem::NewEntity( void )
{
  return RegisterEntity( new Entity( GetNextAvailablentity_id(), this ) );
}

////////////////////////////////////////////////////////////////
SafePtr<Entity> EntitySystem::GetEntity( EntityId entity_id )
{
   return entities_[entity_id];
}

////////////////////////////////////////////////////////////////
EntityId EntitySystem::GetNextAvailablentity_id( void )
{
  return ++entity_counter_;
}

////////////////////////////////////////////////////////////////
void EntitySystem::KillEntity( EntityId entity_id )
{
  SafePtr<Entity> entity = GetEntity( entity_id );

  // Destroy all of the Entity's components
  std::vector<FamilyId>::iterator it = entity->family_ids_.begin();
  for ( ; it != entity->family_ids_.end(); ++it )
  {
    GetSystem( *it )->KillComponent( entity->id_ );
  }

  // Remove and destroy the Entity
  std::map<EntityId, SafePtr<Entity> >::iterator toKill = entities_.find(entity_id);
  toKill->second.clear();
  entities_.erase( toKill );
}

////////////////////////////////////////////////////////////////
Entity::Entity( EntityId entity_id )
  : id_( entity_id )
{ }

////////////////////////////////////////////////////////////////
Entity::Entity( EntityId entity_id, EntitySystem *system )
  : id_( entity_id ), entity_system_( system )
{ }

////////////////////////////////////////////////////////////////
ComponentBase *Entity::GetComponent( FamilyId id )
{
  if ( id >= components_.size() )
    return 0;

  return components_[ id ];
}

} // namespace noah

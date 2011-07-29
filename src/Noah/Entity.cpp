#include <Noah/EntitySystem.h>

namespace noah
{

int EntitySystem::component_system_count_ = 0;
EntitySystem *Entity::entity_system_ = 0;

////////////////////////////////////////////////////////////////
EntitySystem::EntitySystem( void )
{
  Entity::entity_system_ = this;
}

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
void EntitySystem::Update( std::vector<safe_ptr<ComponentSystemBase>> *systems, GameState *state )
{
  std::vector<safe_ptr<ComponentSystemBase>>::const_iterator it = systems->begin();
  for ( ; it != systems->end(); ++it )
  {
    (*it)->Update( state );
  }
}

////////////////////////////////////////////////////////////////
safe_ptr<ComponentSystemBase> EntitySystem::GetSystem( FamilyId family_id )
{
  return component_systems_[ family_id ];
}

////////////////////////////////////////////////////////////////
safe_ptr<Entity> EntitySystem::RegisterEntity( Entity *entity )
{
  safe_ptr<Entity> e(entity);

  entities_.insert( std::pair<EntityId, safe_ptr<Entity>>( entity->id_, e ) );

  return e;
}

////////////////////////////////////////////////////////////////
safe_ptr<Entity> EntitySystem::NewEntity( void )
{
  return RegisterEntity( new Entity( GetNextAvailablentity_id() ) );
}

////////////////////////////////////////////////////////////////
void EntitySystem::InitializeEntity( Entity *entity, GameState *state )
{
  std::vector<FamilyId>::iterator it = entity->family_ids_.begin();
  for ( ; it != entity->family_ids_.end(); ++it )
  {
    GetSystem( *it )->Initialize( entity->id_, state );
  }
}

////////////////////////////////////////////////////////////////
safe_ptr<Entity> EntitySystem::GetEntity( EntityId entity_id )
{
   return entities_[entity_id];
}

////////////////////////////////////////////////////////////////
EntityId EntitySystem::GetNextAvailablentity_id( void )
{
  static EntityId counter = 0;
  return ++counter;
}

////////////////////////////////////////////////////////////////
void EntitySystem::KillEntity( EntityId entity_id )
{
  safe_ptr<Entity> entity = GetEntity( entity_id );

  // Destroy all of the Entity's components
  std::vector<FamilyId>::iterator it = entity->family_ids_.begin();
  for ( ; it != entity->family_ids_.end(); ++it )
  {
    GetSystem( *it )->KillComponent( entity->id_ );
  }

  // Remove and destroy the Entity
  std::map<EntityId, safe_ptr<Entity>>::iterator toKill = entities_.find(entity_id);
  toKill->second.clear();
  entities_.erase(toKill);
}

////////////////////////////////////////////////////////////////
Entity::Entity( EntityId entity_id )
{
  id_ = entity_id;
}

} // namespace noah
//#include <Noah/EntitySystem.h>
#include <Noah/Entity.h>

namespace noah
{

int EntitySystem::component_system_count_ = 0;
EntitySystem *Entity::entity_system_ = 0;

////////////////////////////////////////////////////////////////
EntitySystem::EntitySystem( void )
{
  Entity::entity_system_ = this;
}

/*EntitySystem::~EntitySystem( void )
{
  //entities_.clear();
  //component_systems_.clear();
}*/

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
  std::vector< SafePtr<ComponentSystemBase> >::const_iterator it = systems->begin();
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
Entity *EntitySystem::RegisterEntity( Entity *entity )
{
  //std::cerr << "EntitySystem::RegisterEntity -- 1 -- id_ == " << entity->id_ << std::endl;

  SafePtr<Entity> e(entity);

  entities_.insert( rde::pair< EntityId, SafePtr<Entity> >( entity->id_, e ) );

  return entity;
}

////////////////////////////////////////////////////////////////
Entity *EntitySystem::NewEntity( void )
{
  //std::cerr << "EntitySystem::NewEntity -- 1" << std::endl;

  return RegisterEntity( new Entity( GetNextAvailablEntityId() ) );
}

////////////////////////////////////////////////////////////////
/*void EntitySystem::InitializeEntity( Entity *entity, GameState *state )
{
  std::vector<FamilyId>::iterator it = entity->family_ids_.begin();
  for ( ; it != entity->family_ids_.end(); ++it )
  {
    GetSystem( *it )->Initialize( entity->id_, state );
  }
}
void EntitySystem::InitializeEntity( Entity *entity, GameState *state )
{
  rde::hash_map<std::string, ComponentBase*>::iterator it = entity->components_.begin();
  for ( ; it != entity->components_.end(); ++it )
  {
    it->second->component_system_->Initialize( entity->id_, state );
  }
}*/

////////////////////////////////////////////////////////////////
SafePtr<Entity> EntitySystem::GetEntity( EntityId entity_id )
{
   return entities_[entity_id];
}

////////////////////////////////////////////////////////////////
EntityId EntitySystem::GetNextAvailablEntityId( void )
{
  //std::cerr << "EntitySystem::GetNextAvailablEntityId -- 1" << std::endl;
  static EntityId counter = 0;
  //std::cerr << "EntitySystem::GetNextAvailablEntityId -- 2 -- counter == " << (counter+1) << std::endl;
  return ++counter;
}

////////////////////////////////////////////////////////////////
/*void EntitySystem::KillEntity( EntityId entity_id )
{
  SafePtr<Entity> entity = GetEntity( entity_id );

  // Destroy all of the Entity's components
  std::vector<FamilyId>::iterator it = entity->family_ids_.begin();
  for ( ; it != entity->family_ids_.end(); ++it )
  {
    GetSystem( *it )->KillComponent( entity->id_ );
  }

  // Remove and destroy the Entity
  rde::hash_map<EntityId, SafePtr<Entity>>::iterator toKill = entities_.find(entity_id);
  toKill->second.clear();
  entities_.erase(toKill);
}*/
void EntitySystem::KillEntity( EntityId entity_id )
{
  // Destroy all of the Entity's components
  std::vector< SafePtr<ComponentSystemBase> >::iterator it = component_systems_.begin();
  for ( ; it != component_systems_.end(); ++it )
  {
    (*it)->KillComponent( entity_id );
  }

  // Remove and destroy the Entity
  rde::hash_map<EntityId, SafePtr<Entity> >::iterator toKill = entities_.find(entity_id);
  toKill->second.clear();
  entities_.erase(toKill);
}

/*
void EntitySystem::RegisterMessageHandler( std::string message_name, Handler handler )
{
  //std::cerr << "EntitySystem::HandleMessage -- system -- 1 -- " << message_name << std::endl;

  global_message_handlers_[ message_name ].push_back( handler );

  //std::cerr << "EntitySystem::HandleMessage -- system -- 2" << std::endl;
}

void EntitySystem::RegisterMessageHandler( Entity *entity, std::string message_name, Handler handler )
{
  //std::cerr << "EntitySystem::HandleMessage -- entity -- 1 -- " << message_name << std::endl;

  entity->RegisterMessageHandler( message_name, handler );

  //std::cerr << "EntitySystem::HandleMessage -- entity -- 2" << std::endl;
}



void EntitySystem::BroadcastMessage( std::string message, Message const &msg )
{
  for (std::vector<Handler>::iterator it = global_message_handlers_[message].begin(); it != global_message_handlers_[message].end(); ++it)
  {
    (*it).callback_( msg );
  }
}

void EntitySystem::ExecuteHandlers( Message const &msg, std::vector<Handler> *handlers )
{
  for (std::vector<Handler>::iterator it = handlers->begin(); it != handlers->end(); ++it)
  {
    (*it).callback_( msg );
  }
}

std::vector<Handler> *EntitySystem::GetHandlers( std::string message )
{
  return &( global_message_handlers_[ message ] );
}
*/

////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////
Entity::Entity( EntityId entity_id ) : id_( entity_id )
{
  //std::cerr << "Entity::Entity -- 1 -- id_ == " << id_ << std::endl;
}

Entity::~Entity( void )
{
  // Destroy all of the Entity's components
  /*rde::hash_map< std::string, SafePtr<ComponentBase>, hash >::iterator it = components_.begin();
  for ( ; it != components_.end(); ++it )
  {
    it->second->Kill();
  }*/
}

/*
void Entity::RegisterMessageHandler( std::string message_name, Handler handler )
{
  //std::cerr << "Entity::HandleMessage -- 1 -- message == " << message_name << std::endl;
  //std::cerr << "Entity::HandleMessage -- 2 -- id_ == " << id_ << std::endl;

  //message_handlers_INT[ message_name ].push_back( 4 );
  message_handlers_[ message_name ].push_back( handler );

  //std::cerr << "Entity::HandleMessage -- 3" << std::endl;
}

void Entity::BroadcastMessage( std::string message, Message const &msg )
{
  //std::cerr << "Entity -- Broadcasting to an entity: id_ == " << id_ << " -- message == " << message << std::endl;
  //std::cerr << "number of handlers in list: " << message_handlers_.size() << std::endl;

  for (std::vector<Handler>::iterator it = message_handlers_[message].begin(); it != message_handlers_[message].end(); ++it)
  {
    (*it).callback_( msg );
  }
}

void Entity::ExecuteHandlers( Message const &msg, std::vector<Handler> *handlers )
{
  for (std::vector<Handler>::iterator it = handlers->begin(); it != handlers->end(); ++it)
  {
    (*it).callback_( msg );
  }
}

std::vector<Handler> *Entity::GetHandlers( std::string message )
{
  return &( message_handlers_[ message ] );
}
*/

} // namespace noah

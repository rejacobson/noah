#include <Noah/Notifier.h>

namespace noah
{

unsigned int Notifier::message_counter_ = 0;
rde::hash_map<std::string, unsigned int, hash> Notifier::message_names_;

unsigned int Notifier::GetHandlerId( std::string name )
{
  // if it doesn't exist, create it
  if ( message_names_.find(name) == message_names_.end() )
  {
    message_names_[ name ] = ++message_counter_;
    return message_counter_;
  }

  // it exists, just return it
  return message_names_[ name ];
}

void Notifier::RegisterMessageHandler( std::string name, Handler handler )
{
  unsigned int handler_id = GetHandlerId( name );

  if ( message_handlers_.size() <= handler_id )
  {
    message_handlers_.resize( handler_id + 1 );
  }

  message_handlers_[ handler_id ].push_back( handler );
}

void Notifier::BroadcastMessage( std::string name, Message const &msg )
{
  unsigned int handler_id = GetHandlerId( name );

  BroadcastMessage( handler_id, msg );
}

void Notifier::BroadcastMessage( unsigned int id, Message const &msg )
{
  if ( message_handlers_.size() <= id )
    return;

  for (std::vector<Handler>::iterator it = message_handlers_[id].begin(); it != message_handlers_[id].end(); ++it)
  {
    (*it).callback_( msg );
  }
}

/*
void Notifier::ExecuteHandlers( Message const &msg, std::vector<Handler> *handlers )
{
  for (std::vector<Handler>::iterator it = handlers->begin(); it != handlers->end(); ++it)
  {
    (*it).callback_( msg );
  }
}

std::vector<Handler> *Notifier::GetHandlers( std::string message )
{
  return &( message_handlers_[ message ] );
}
*/

}
#include <Noah/EntitySystem.h>

namespace noah
{

void Notifier::RegisterMessageHandler( std::string message_name, Handler handler )
{
  message_handlers_[ message_name ].push_back( handler );
}

void Notifier::BroadcastMessage( std::string message, Message const &msg )
{
  for (std::vector<Handler>::iterator it = message_handlers_[message].begin(); it != message_handlers_[message].end(); ++it)
  {
    (*it).callback_( msg );
  }
}

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

}
#ifndef _NOTIFIER_H
#define _NOTIFIER_H

#include <Noah/Message.h>

namespace noah
{

class Notifier
{
  public:
    void RegisterMessageHandler( std::string message_name, Handler handler );
    void BroadcastMessage( std::string message, Message const &msg );
    void ExecuteHandlers( Message const &msg, std::vector<Handler> *handlers );
    std::vector<Handler> *GetHandlers( std::string message );

  private:
    stdext::hash_map< std::string, std::vector<Handler> > message_handlers_;
};

}

#endif
#ifndef _NOTIFIER_H
#define _NOTIFIER_H

#include <rdestl/hash_map.h>

#include <Noah/Message.h>
#include <Noah/Utility.h>

#include <vector>
#include <string>

namespace noah
{

struct Handler;
struct Message;

class Notifier
{
  public:
    unsigned int GetHandlerId( std::string name );

    void RegisterMessageHandler( std::string name, Handler handler );
    void BroadcastMessage( std::string name, Message const &msg );
    void BroadcastMessage( unsigned int id, Message const &msg );

    /*void ExecuteHandlers( Message const &msg, std::vector<Handler> *handlers );
    std::vector<Handler> *GetHandlers( std::string message );*/

  private:
    //rde::hash_map< std::string, std::vector<Handler>, hash > message_handlers_;
    std::vector< std::vector<Handler> > message_handlers_;

    static rde::hash_map<std::string, unsigned int, hash> message_names_;
    static unsigned int message_counter_;
};

}

#endif
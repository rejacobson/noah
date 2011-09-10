#ifndef _MESSAGE_H
#define _MESSAGE_H

//#include <Noah/Component.h>

#include <boost/function.hpp>
#include <boost/any.hpp>
#include <boost/bind.hpp>

namespace noah
{

class ComponentBase;
struct Message;

typedef boost::function<void(Message const &)> Callback;

struct Handler
{
  ComponentBase *component_;
  Callback callback_;
  //Handler( ComponentBase *c, Callback b ) : component_( c ), callback_( b ) {};
};

enum MessageType {
  CREATE,
  DESTROY,
  MESSAGE
};

struct Message
{
  MessageType type_;
  ComponentBase *sender_;
  boost::any payload_;
  Message( MessageType t ) : type_( t ) {};
  Message( MessageType t, ComponentBase *c ) : type_( t ), sender_( c ) {};
  Message( MessageType t, ComponentBase *c, boost::any p ) : type_( t ), sender_( c ), payload_( p ) {};
};

} // namespace noah

#endif

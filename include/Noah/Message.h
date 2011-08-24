#ifndef _MESSAGE_H
#define _MESSAGE_H

#include <Noah/EntitySystem.h>

namespace noah
{

typedef boost::function<void(Message const &)> Callback;

struct Handler
{
  ComponentBase *component_;
  Callback callback_;
};

enum MessageType {
  CREATE,
  DESTROY,
  MESSAGE
};

struct Message
{
  MessageType type;
  ComponentBase *sender;
  boost::any p;
  Message(MessageType t) : type(t) {};
  Message(MessageType t, ComponentBase *c) : type(t), sender(c) {};
  Message(MessageType t, ComponentBase *c, boost::any payload) : type(t), sender(c), p(payload) {};
};

} // namespace noah

#endif

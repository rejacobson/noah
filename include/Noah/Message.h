#ifndef _MESSAGE_H
#define _MESSAGE_H

#include <Noah/Component.h>
#include <boost/function.hpp>

typedef boost::function<void(Message const &)> Callback;

struct Handler
{
  Component *component_;
  Callback callback_;
};

enum MessageType {
  CREATE,
  DELETE,
  MESSAGE
};

struct Message
{
  MessageType type;
  Component *sender;
  boost::any p;
  Message(MessageType t) : type(t) {};
  Message(MessageType t, Component *c) : type(t), sender(c) {};
  Message(MessageType t, Component *c, boost::any payload) : type(t), sender(c), p(payload) {};
};


#endif

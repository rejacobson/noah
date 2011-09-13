#ifndef _ATTRIBUTE_H
#define _ATTRIBUTE_H

#include <string>
#include <hash_map>
#include <iostream>

#include <Noah/Callback.h>

#include <boost/any.hpp>

namespace noah
{

struct AttributeBase;
struct Message;

typedef unsigned int AttrId;
typedef std::string AttrName;
typedef stdext::hash_map<AttrName, AttrId> AttrIdHash;
typedef std::vector<AttributeBase*> AttributeList;

struct AttributeBase
{
  virtual void Set( void *value ) { }
  virtual void const *Get() { return 0; }
  virtual void AddWatcher( Handler handler ) { }

  bool empty_;
};

template <typename Type>
struct Attribute : public AttributeBase
{
  Attribute( void )
  {
    empty_ = true;
  }

  Attribute( Type *value ) : value_( value )
  {
    empty_ = false;
  }

  void Set( void *value )
  {
    Notify( value );
    
    value_ = (Type*)value;
  }

  void const *Get( void )
  {
    return value_;
  }

  void AddWatcher( Handler handler )
  {
    handlers_.push_back( handler );
  }

  void Notify( void *value )
  {
    if ( handlers_.size() == 0 ) return;

    for ( std::vector<Handler>::iterator it = handlers_.begin(); it != handlers_.end(); ++it )
    {
      (*it).callback_( value );
    }
  }

 private:
  Type *value_;
  std::vector<Handler> handlers_;
};

class Attributes
{
  public:
    AttrId FetchAttrId( AttrName name );
    AttrId FindAttrId( AttrName name );

    //void WatchAttr( AttrName name );
    void WatchAttr( AttrName name, Callback callback )
    {
      Handler handler;
      handler.callback_ = callback;
      
      AttributeBase *a = GetAttr( name );

      if ( a == 0 ) return;

      a->AddWatcher( handler );
    }

    template <typename T>
    void RegisterAttr( AttrName name, T *value )
    {
      //std::cerr << "Register: \"" << name << "\"" << std::endl;

      AttrId id = FetchAttrId( name );

      if ( attributes_.size() <= id ) attributes_.resize( id + 1 );

      AttributeBase *a = new Attribute<T>( value );

      attributes_[ id ] = a;
    }

    template <typename T>
    void RegisterAttr( AttrName name, T *value, Callback callback )
    {
      RegisterAttr( name, value );

      WatchAttr( name, callback );
    }

    template <typename T>
    void SetAttr( AttrName name, T const &value )
    {
      AttrId id = FetchAttrId( name );

      SetAttr( id, value );
    }

    template <typename T>
    void SetAttr( AttrId id, T const &value )
    {
      if ( attributes_.size() <= id ) return;

      attributes_[ id ]->Set( new T(value) );
    }

    template <typename T>
    T *GetAttr( AttrName name )
    {
      AttrId id = FindAttrId( name );

      return GetAttr<T>( id );
    }

    template <typename T>
    T *GetAttr( AttrId id )
    {
      if ( 0 == id || attributes_.size() <= id || attributes_[ id ]->empty_ ) return 0;

      return (T*)attributes_[ id ]->Get();
      //return dynamic_cast<T*>( attributes_[ id ]->Get() );
    }

    AttributeBase *GetAttr( AttrName name );

  private:
    AttributeList attributes_;

    static AttrId attribute_counter_;
    static AttrIdHash attribute_ids_;
};

}

#endif
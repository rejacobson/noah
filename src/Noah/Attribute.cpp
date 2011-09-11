#include <Noah/Attribute.h>

namespace noah
{

AttrId Attributes::attribute_counter_ = 0;
AttrIdHash Attributes::attribute_ids_;

AttrId Attributes::FetchAttrId( AttrName name )
{
  if ( attribute_ids_.find( name ) == attribute_ids_.end() )
  {
    attribute_ids_[ name ] = ++attribute_counter_;
    return attribute_counter_;
  }

  return attribute_ids_[ name ];
}

AttrId Attributes::FindAttrId( AttrName name )
{
  if ( attribute_ids_.find( name ) == attribute_ids_.end() ) return 0;

  return attribute_ids_[ name ];
}

AttributeBase *Attributes::GetAttr( AttrName name )
{
  AttrId id = FindAttrId( name );

  if ( 0 == id ) return 0;

  return attributes_[ id ];
}

}
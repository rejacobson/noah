#ifndef _RESOURCE_MANAGER_H
#define _RESOURCE_MANAGER_H

#include <hash_map>
#include <Noah/Singleton.h>
#include <Noah/SafePtr.h>

namespace noah
{

// The following class defines a hash function for strings 
class stringhasher : public stdext::hash_compare <std::string>
{
  public:
    // Required by 
    // Inspired by the java.lang.String.hashCode() algorithm 
    // (it's easy to understand, and somewhat processor cache-friendly)
    // @param The string to be hashed
    // @return The hash value of s
    size_t operator() ( const std::string& s ) const
    {
      size_t h = 0;
      std::string::const_iterator p, p_end;
      for ( p = s.begin(), p_end = s.end(); p != p_end; ++p )
      {
        h = 31 * h + (*p);
      }
      return h;
    }

    
    
    // @param s1 The first string
    // @param s2 The second string
    // @return true if the first string comes before the second in lexicographical order
    bool operator() ( const std::string& s1, const std::string& s2 ) const
    {
      return s1 < s2;
    }
};


template <typename T> class ResourceType;

template <typename T>
class ResourceTypeBase : public Singleton<ResourceType<T>>
{
  public:
    std::hash_map<std::string, SafePtr<T>, stringhasher> list;
    typename std::hash_map<std::string, SafePtr<T>>::iterator iterator;
};

template <typename T>
class ResourceType : public ResourceTypeBase<T>
{
  public:
    ~ResourceType( void );

    SafePtr<T> Load( std::string path ) { return 0; }
};

class ResourceManager
{
  public:
    template <class T>
    static SafePtr<T> Load( std::string path )
    {
      ResourceType<T> *c = GetResourceType<T>();

      return c->Load( path );
    }

  private:
    template <class T>
    static ResourceType<T> *GetResourceType( void )
    {
      ResourceType<T> *p = ResourceType<T>::GetSingletonPtr();

      if ( p == 0 )
      {
        p = new ResourceType<T>();
      }

      return p;
    }
};

} // namespace noah

#endif
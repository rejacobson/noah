#ifndef _RESOURCES_H
#define _RESOURCES_H

#include <SFML/Graphics.hpp>
#include <Noah/ResourceManager.h>

namespace noah
{

template <>
class ResourceType<sf::Image> : public noah::ResourceTypeBase<sf::Image>
{
  public:
    ~ResourceType( void ) { }

    SafePtr<sf::Image> Load( std::string path )
    {
      iterator = list.find( path );

      if ( iterator != list.end() )
        return iterator->second;
      
      std::cerr << "Loading Image: " << path << std::endl;

      sf::Image *img = new sf::Image();
      
      if ( ! img->LoadFromFile( path ) )
        return 0;

      SafePtr<sf::Image> i(img);

      list[ path ] = i;

      return i;
    }
};

template <>
class ResourceType<sf::Sprite> : public ResourceTypeBase<sf::Sprite>
{
  public:
    ~ResourceType( void ) { }

    SafePtr<sf::Sprite> Load( std::string path )
    {
      iterator = list.find( path );

      if ( iterator != list.end() )
        return iterator->second;
      
      std::cerr << "Loading Sprite: " << path << std::endl;

      SafePtr<sf::Image> img = ResourceManager::Load<sf::Image>( path );
      
      if ( img == 0 )
        return 0;

      SafePtr<sf::Sprite> s( new sf::Sprite( *img ) );

      list[ path ] = s;

      return s;
    }
};

template <>
class ResourceType<sf::Font> : public noah::ResourceTypeBase<sf::Font>
{
  public:
    ~ResourceType( void ) { }

    SafePtr<sf::Font> Load( std::string path )
    {
      iterator = list.find( path );

      if ( iterator != list.end() )
        return iterator->second;
      
      std::cerr << "Loading Font: " << path << std::endl;

      sf::Font *font = new sf::Font();
      
      if ( ! font->LoadFromFile( path ) )
        return 0;

      SafePtr<sf::Font> f(font);

      list[ path ] = f;

      return f;
    }
};

} // namespace noah

#endif
#include <Noah/Json.h>

namespace noah
{
  Json::Value ReadJson( std::string filename )
  {
    std::ifstream t( filename );
    std::stringstream buffer;
    buffer << t.rdbuf();

    std::string file_contents = buffer.str();

    Json::Value root;   // will contains the root value after parsing.
    Json::Reader reader;
    bool parsingSuccessful = reader.parse( file_contents, root );

    if ( ! parsingSuccessful )
    {
        // report to the user the failure and their locations in the document.
        std::cout  << "Failed to parse json file: " << filename << "\n"
                   << reader.getFormattedErrorMessages();
    }

    return root;
  }
}
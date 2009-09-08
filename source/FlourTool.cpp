#include "FlourTool.h"
#include "Flour.h"
#include <iostream>


FlourTool::FlourTool(const std::string& name, const std::string& description)
: mVariablesMap(), mOptionsDescription(description), mName(name), mDescription(description), mHasPositionalOptions(false)
{
}

FlourTool::~FlourTool()
{
}

void FlourTool::parse(int argc, char** argv)
{
// try
// {
  
  
  boost::program_options::command_line_parser   optionsParser(argc, argv);
  optionsParser.options(mOptionsDescription);
  optionsParser.allow_unregistered();
  //if (mHasPositionalOptions)
  optionsParser.positional(mPositionalOptions);

  boost::program_options::parsed_options options = optionsParser.run();
  boost::program_options::store(options, mVariablesMap);
  
/*
  boost::program_options::command_line_parser parser(argc, argv);
  parser.options(mOptionsDescription);
  parser.allow_unregsitered
*/
  //boost::program_options::parsed_options options = boost::program_options::command_line_parser(argc, argv).options(mOptionsDescription).allow_unregistered().run();

 // boost::program_options::store(options, mVariablesMap);
  
  if (mVariablesMap.count("help"))
  {
   std::cout << mOptionsDescription << std::endl;
   return;
   }
  
  process();
// }
// catch(...)
// {
//  std::cout << "AAA" << std::endl;
// }

}

void FlourTool::process()
{
 std::cout << "Not Bound." << std::endl;
}

void FlourTool::add_error(unsigned int id, const std::string &description)
{
 mErrors[id] = description;
}

void FlourTool::error(unsigned int id, bool aborted)
{
 
 std::map<unsigned int, std::string>::iterator it = mErrors.find(id);
 std::string error_string;
 
 if (it == mErrors.end())
  std::cout << "Error! " << "Unknown Error. (" << id << ")" << std::endl;
 else
  std::cout << "Error! " << (*it).second << " (" << id <<")" << std::endl;
 
 if (aborted)
  std::cout << "Aborted." << std::endl;
}
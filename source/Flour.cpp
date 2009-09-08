/** \brief Flour
*/

#include "Flour.h"
#include "FlourTool.h"

#include <iostream>
#include <boost/program_options.hpp> 
#include <boost/algorithm/string.hpp>

Flour* Flour::mFlourInstance = 0;

Flour::Flour()
: mOptionsDescription(getVersion()), mVariablesMap(), mWorld(0)
{
 
 mFlourInstance = this;
 
 registerTools();
 
}

Flour::~Flour()
{
 if (mWorld)
  NxOgre::World::destroyWorld();
}

Flour* Flour::getInstance()
{
 return mFlourInstance;
}

std::string Flour::getVersion() const
{
 return "Flour 0.4.0";
}

void Flour::fromCommandLine(int argc, char** argv)
{
 
 boost::program_options::parsed_options options
  = boost::program_options::command_line_parser(argc, argv).options(mOptionsDescription).allow_unregistered().run();
 
 boost::program_options::store(options, mVariablesMap);


 if (mVariablesMap.size() == 0)
 {
  std::cout << mOptionsDescription << "\n";
  return;
 }
 
 if (mVariablesMap.count("help") && mVariablesMap.count("tool") == 0)
 {
  std::cout << mOptionsDescription << "\n";
  return;
 }
 
 for (std::map<std::string, FlourTool*>::iterator it = mTools.begin(); it != mTools.end(); it++)
 {
  if (mVariablesMap.count((*it).first))
  {
   (*it).second->parse(argc, argv);
   return;
  }
 }
  
 std::cout << "Error: Unrecongised tool" << std::endl;
}

std::string Flour::getTools()
{
 
 if (mTools.size() == 0)
  return "No tools registered";
 
 if (mTools.size() == 1)
  return (*mTools.begin()).first;
 
 std::stringstream ss;
 
 unsigned int i = 0;
 for (std::map<std::string, FlourTool*>::iterator it = mTools.begin(); it != mTools.end(); it++)
 {
  ss << (*it).first;
  if (i == mTools.size() - 2)
   ss << " or ";
  else if (i == mTools.size() - 1)
   ;
  else
   ss << ", ";
  i++;
 }
 
 return ss.str();
}
  
void Flour::registerTool(const std::string& name, FlourTool* tool)
{
 std::string tool_name = name;
 boost::to_lower(tool_name);
 mTools[tool_name] = tool;
 mOptionsDescription.add_options()
  (name.c_str(), tool->getDescription().c_str());
}

FlourTool* Flour::getTool(const std::string& name)
{
 std::string tool_name = name;
 boost::to_lower(tool_name);
 std::map<std::string, FlourTool*>::iterator it = mTools.find(tool_name);
 if (it == mTools.end())
  return 0;
 return (*it).second;
}

void Flour::initNxOgre()
{
 
 if (mWorld)
  return;
 
 mWorld = NxOgre::World::createWorld();
 
}
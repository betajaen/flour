/** File: Flour.cpp
    Created on: 06-Sept-09
    Author: Robin Southern "betajaen"

    Copyright (c) 2009 Robin Southern

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#include "Flour.h"
#include "FlourTool.h"
#include "FlourFile.h"

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

void Flour::registerFile(FlourFile* file)
{
 std::string ext = file->getExtension();
 boost::to_lower(ext);
 mFiles[ext] = file;
}

void Flour::initNxOgre()
{
 
 if (mWorld)
  return;
 
 mWorld = NxOgre::World::createWorld();
 
}
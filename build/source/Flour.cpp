/** 
    
    This file is part of Flour.
    
    Copyright (c) 2010 Robin Southern, http://www.nxogre.org
    
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

int main(int argc, char **argv) 
{
 try
 {
  NxOgre::Flour* flour = new NxOgre::Flour();
  bool result = flour->fromCommandLine(argc, argv);
  if (result)
   std::cout << "OK.\n";
  delete flour;
 }
 catch(const NxOgre::Exception& e)
 {
  std::cout << "Exception Called.\n";
 }
}

NxOgre::Flour* NxOgre::Flour::sSingleton = 0;

void registerTools();

namespace NxOgre
{


namespace N = NxOgre;
namespace S = NxOgre::Strings;
using NxOgre::String;


Flour::Flour() : mWorld(0)
{
 registerTools();
}
 
Flour::~Flour()
{
}
 
NxOgre::World* Flour::getWorld()
{
 if (mWorld == 0)
 {
  NxOgre::WorldDescription description;
  mWorld = NxOgre::World::createWorld(description);
 }
 return mWorld;
}

std::string Flour::getVersion() const
{
 return "Flour 0.4.8";
}

void Flour::registerTool(const std::string& name, Tool* tool)
{
 mTools.insert(name, tool);
}

void Flour::printTools()
{
  std::cout << getVersion() << "\n\nTools available are:\n";
  
  for (NxOgre::map_iterator<std::string, Tool*> it = mTools.elements(); it != it.end(); it++)
   std::cout << it.key() << "\n";

}

bool Flour::fromCommandLine(int argc, char** argv)
{
 
 Args args(argc, argv);

 // > flour
 if (args.tool.length() == 0)
 {
  printTools();
  return false;
 }
 
 // > flour --help
 // > flour --h
 if (args.args.size() && NxOgre::Strings::matches_insensitive(args.tool, "help") || NxOgre::Strings::matches_insensitive(args.tool, "h"))
 {
  printTools();
  return false;
 }
 
 Tool* tool = NxOgre::Maps::get_or_null<NxOgre::String, Tool>(mTools, args.tool); 
 
 if(tool == 0)
 {
  puts << "Cannot understand '" << args.tool << "'.";
  return false;
 }

 return tool->process(args);

}

Args::Args(int argc, char** argv)
{
 
 String working;
 
 for (int i = 1; i < argc;++i)
 {
  working.assign(argv[i]);
  args.push_back(working);

  if (i==1)
  {
   tool.assign(working);
   S::slice_after_last_of(tool, '-'); // chop off the any - characters at the beginning.
   S::lower(tool);
   continue;
  }
  
  // Anything without a - in front and no properties, flags defined yet is a file.
  if (working[0] != '-' && (flags.size() == 0 || properties.size() == 0))
  {
   files.push_back(working);
   continue;
  }
  
  if (working[0] == '-') // a flag or a property.
  {
   if (S::index(working, '=') != String::npos)
   {
    working = working.substr(working.find_first_not_of('-'));
    S::split(working, properties, '=');
   }
   else
   {
    working = working.substr(working.find_first_not_of('-'));
    S::lower(working);
    flags.push_back(working);
   }
   

  }
  
 }
 
}

} // namespace
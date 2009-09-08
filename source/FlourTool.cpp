/** File: FlourTool.cpp
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
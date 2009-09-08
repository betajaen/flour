/** File: FlourTool.h
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

#ifndef FLOUR_TOOL_H
#define FLOUR_TOOL_H

#include <boost/program_options.hpp>
#include <iostream> 
#include <string>

class FlourTool
{
  
 public:
  
  FlourTool(const std::string& name, const std::string& description);
  
  virtual         ~FlourTool();

  std::string      getName() const { return mName; }
  
  std::string      getDescription() const { return mDescription; }
  
  void             parse(int argc, char** argv);
  
  virtual void     process();
  
  virtual void     error(unsigned int id, bool aborted);
  
  virtual void     add_error(unsigned int id, const std::string& description);
  
  
 protected:
  
  std::string                                             mName;
  
  std::string                                             mDescription;
  
  std::map<unsigned int, std::string>                     mErrors;
  

  boost::program_options::variables_map                   mVariablesMap;
  
  boost::program_options::options_description             mOptionsDescription;
  
  bool                                                    mHasPositionalOptions;
  
  boost::program_options::positional_options_description  mPositionalOptions;
  
};

#endif
/** File: Flour.h
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

#ifndef FLOUR_H
#define FLOUR_H

#include "NxOgre.h"
#include <string>
#include <boost/program_options.hpp> 

class FlourTool;
class FlourFile;

class Flour
{
 public:
  
  Flour();
  
 ~Flour();
  
  static Flour* getInstance();
  
  void registerTools();
  
  void registerTool(const std::string& name, FlourTool*);

  void registerFiles();
  
  void registerFile(FlourFile*);
  
  void fromCommandLine(int argc, char** argv);
  
  void fromString(const std::string&);
  
  FlourTool* getTool(const std::string&);
  
  std::string getVersion() const;
  
  std::string getTools();
  
  FlourFile*  getFile(const std::string&);
  
  void        initNxOgre();
  
 protected:
  
  static Flour*                                mFlourInstance;
  
  std::map<std::string, FlourTool*>            mTools;
  
  std::map<std::string, FlourFile*>            mFiles;
  
  boost::program_options::variables_map        mVariablesMap;
  
  boost::program_options::options_description  mOptionsDescription;
  
  NxOgre::World*                               mWorld;
  
};

#endif
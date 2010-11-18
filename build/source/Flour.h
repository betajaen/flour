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

#ifndef FLOUR_H
#define FLOUR_H

#include "NxOgre.h"
#include <string>

namespace NxOgre
{

class outstream;
class Tool;

class Args
{
 public:
  
  Args(int argc, char** argv);
  void inspect()
  {
   std::cout << "----\n" << tool << "\nfiles = " << files.to_s() << "\nflags = " << flags.to_s() << "\nargs = " << args.to_s() << "\n";
  }
  std::string                           tool;
  NxOgre::map<std::string, std::string> properties;
  NxOgre::vector<std::string>           files;
  NxOgre::vector<std::string>           flags;
  NxOgre::vector<std::string>           args;
};

class Flour : public NxOgre::Singleton<Flour>
{
 
 public:
  
  Flour();
  
 ~Flour();
  
  NxOgre::World* getWorld();
  
  std::string getVersion() const;
  
  void registerTool(const std::string& name, Tool*);
  
  bool fromCommandLine(int argc, char** argv);
  
  void printTools();

 protected:
  
  NxOgre::World* mWorld;
  
  NxOgre::map<std::string, Tool*> mTools;
};

class outstream
{
  
 public:
  
  outstream& operator<<(const std::string& str)
  {
   std::cout << str;
   return *this;
  }
  
  outstream& operator<<(int val)
  {
   std::cout << val;
   return *this;
  }
  
  void end()
  {
   std::cout << "\n";
  }
  
};

static outstream puts;

class Tool
{
 
 public:
  
  virtual ~Tool() {}
  
  virtual bool process(Args& args) {return false;}
  
};

} // namespace


#endif

/** \brief
*/
#ifndef FLOUR_H
#define FLOUR_H

#include "NxOgre.h"
#include <string>
#include <boost/program_options.hpp> 

class FlourTool;

class Flour
{
 public:
  
  Flour();
  
 ~Flour();
  
  static Flour* getInstance();
  
  void registerTools();
  
  void registerTool(const std::string& name, FlourTool*);
  
  void fromCommandLine(int argc, char** argv);
  
  void fromString(const std::string&);
  
  FlourTool* getTool(const std::string&);
  
  std::string getVersion() const;
  
  std::string getTools();
  
  void        initNxOgre();
  
 protected:
  
  static Flour*                                mFlourInstance;
  
  std::map<std::string, FlourTool*>            mTools;
  
  boost::program_options::variables_map        mVariablesMap;
  
  boost::program_options::options_description  mOptionsDescription;
  
  NxOgre::World*                               mWorld;
  
};

#endif
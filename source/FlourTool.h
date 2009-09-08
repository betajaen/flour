/** \brief Tool.h
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
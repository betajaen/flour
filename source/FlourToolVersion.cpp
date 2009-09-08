#include "FlourToolVersion.h"
#include "Flour.h"

#include <iostream>

#include <NxOgre.h>


FlourVersion::FlourVersion() : FlourTool("version", "Prints version information")
{
 
 mOptionsDescription.add_options()
    ("help", "Show this")
    ("verbose", "Show library version information")
 ;
  
}

FlourVersion::~FlourVersion()
{
}

void FlourVersion::process()
{

 std::cout << Flour::getInstance()->getVersion();

 if (mVariablesMap.count("verbose"))
 {
  std::cout << " is compiled" 
#ifdef _DEBUG
 " in debug mode"
#endif
 " with the following libraries" << std::endl
           << " - NxOgre " << NxOgreVersionMajor << "." << NxOgreVersionMinor << "." << NxOgreVersionRelease << " " << NxOgreCodeName << std::endl
           << " - Boost " << BOOST_VERSION / 100000 << "." << BOOST_VERSION / 100 % 1000 << "." << BOOST_VERSION % 100 << std::endl
           << std::endl << "Visit: http://nxogre.org/flour";
 }
 
 std::cout << std::endl;
}

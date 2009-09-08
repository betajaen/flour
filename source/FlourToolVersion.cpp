/** File: FlourToolVersion.cpp
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

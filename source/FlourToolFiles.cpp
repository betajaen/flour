/** File: FlourToolFiles.cpp
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

#include "FlourToolFiles.h"
#include "Flour.h"
#include "FlourFile.h"

#include <iostream>

#include <NxOgre.h>


FlourFiles::FlourFiles() : FlourTool("files", "Prints files readable and writable by flour")
{
}

FlourFiles::~FlourFiles()
{
}

void FlourFiles::process()
{
 Flour* flour = Flour::getInstance();
 std::cout << "Flour can load and save the following file-formats: " << std::endl;
 for (std::map<std::string, FlourFile*>::iterator it = flour->mFiles.begin(); it != flour->mFiles.end(); it++)
 {
  std::cout << " * " << (*it).first << " : " << (*it).second->getDescription() << std::endl;
 }

 std::cout << "Flour can perform the following conversions: " << std::endl;
 for (std::map<std::string, FlourFile*>::iterator it = flour->mFiles.begin(); it != flour->mFiles.end(); it++)
 {
  std::cout << " * " << (*it).first << " to " << (*it).second->getOpposite() << std::endl;
 }

}
